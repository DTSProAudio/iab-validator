/* Copyright (c) 2020 Xperi Corporation (and its subsidiaries). All rights reserved.
 
Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:
 
The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.
 
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/**
* MDA Packer Implementation
*
* @file
*/

#include <assert.h>
#include <stack>
#include <vector>
#include <stdlib.h>

#include "IABDataTypes.h"
#include "IABElements.h"
#include "IABUtilities.h"
#include "IABParser.h"
#include "IABVersion.h"

#define IABParserAPIHighVersion     0
#define IABParserAPILowVersion      0

namespace SMPTE
{
namespace ImmersiveAudioBitstream
{
    // Embed legal text in Parser library binary and application executables that contain the Parser library
    const char *g_xperi_copyright_parser = "IABParserLib, Copyright (c) 2020 Xperi Corporation (and its subsidiaries). All rights reserved. Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.";

    // ****************************************************************************
	// Factory methods for creating instances of IABPacker
	// ****************************************************************************

	// Create IABParser instance
	IABParserInterface* IABParserInterface::Create(std::istream* iInputStream)
	{
        if (nullptr == iInputStream)
        {
            return nullptr;
        }
        
		IABParser* iabParser = nullptr;
		iabParser = new IABParser(iInputStream);
		return iabParser;
	}

	// Create IABParser instance without associated IAB data stream
	IABParserInterface* IABParserInterface::Create()
	{
		IABParser* iabParser = new IABParser();
		return iabParser;
	}

	// Deletes an IABParser instance
    void IABParserInterface::Delete(IABParserInterface* iInstance)
    {
        delete iInstance;
    }

    /****************************************************************************
    *
    * IABParser
    *
    *****************************************************************************/

	IABParser::IABParser(std::istream* iInputStream)
	{
		iabStream_ = iInputStream;
		iabParserFrame_ = nullptr;
		unAllowedFrameSubElementsCount_ = 0;
	}

	IABParser::IABParser()
	{
		iabStream_ = nullptr;
		iabParserFrame_ = nullptr;
		unAllowedFrameSubElementsCount_ = 0;
	}

	IABParser::~IABParser()
	{
		// delete any iabParserFrame_
		if (NULL != iabParserFrame_)
		{
			IABFrameInterface::Delete(iabParserFrame_);
		}
	}

    void IABParser::GetAPIVersion(IABAPIVersionType& oVersion)
    {
        oVersion.fHigh_ = IABParserAPIHighVersion;
        oVersion.fLow_  = IABParserAPILowVersion;
    }

    iabError IABParser::GetBitstreamVersion(IABVersionNumberType& oVersion) const
    {
        IABVersionNumberType version = 0;
        
        if (nullptr != iabParserFrame_)
        {
            iabParserFrame_->GetVersion(version);            
            oVersion = version;
            return kIABNoError;
        }
        else
        {
            oVersion = 0;
            return kIABParserNoParsedFrameError;
        }
    }

    // Parse an IAB frame
    iabError IABParser::ParseIABFrame()
    {
		// Save existing frame pointer, decide whether delete is needed later
		IABFrameInterface* olderParsedFrame = iabParserFrame_;

		// Create/"new" IABFrameInterface instance for the frame to be parsed in..
		// (This sequence forces instance to be created at a different address, though less optimised.)
		iabParserFrame_ = IABFrameInterface::Create(iabStream_);

		if (nullptr == iabParserFrame_)
		{
			return kIABMemoryError;
		}

		// Now delete the old parsed frame object to avoid memory leak if necessary
		if (nullptr != olderParsedFrame)
		{
			IABFrameInterface::Delete(olderParsedFrame);
		}
        
		// Parse        
        iabError returnCode = kIABNoError;
        returnCode = iabParserFrame_->DeSerialize();

		// Update total number of unallowed + undefined frame subelements encountered during parsing
		// Cumulative over frames.
		unAllowedFrameSubElementsCount_ += iabParserFrame_->GetNumUnallowedSubElements();
		unAllowedFrameSubElementsCount_ += iabParserFrame_->GetNumUndefinedSubElements();

        return returnCode;
    }
    
	// Parse an IAB frame from buffer iIABFrameDataBuffer
	iabError IABParser::ParseIABFrame(char* iIABFrameDataBuffer, uint32_t iBufferSize)
	{
		// Check input parameter
		if ((NULL == iIABFrameDataBuffer) || (iBufferSize == 0))
		{
			return kIABBadArgumentsError;
		}

		iabError returnCode = kIABNoError;

		iabStream_ = new std::istringstream(std::string(iIABFrameDataBuffer, iBufferSize));

		if (!iabStream_)
		{
			return kIABBadArgumentsError;
		}

		returnCode = ParseIABFrame();

		delete iabStream_;
		iabStream_ = NULL;

		return returnCode;
	}

	iabError IABParser::GetIABFrame(const IABFrameInterface*& oIABFrame)
    {
        if (nullptr != iabParserFrame_)
        {
            oIABFrame = iabParserFrame_;
            return kIABNoError;
        }
        
        oIABFrame = nullptr;
        return kIABParserNoParsedFrameError;
    }

	iabError IABParser::GetIABFrameReleased(IABFrameInterface*& oIABFrame)
	{
		if (iabParserFrame_ != nullptr)
		{
			oIABFrame = iabParserFrame_;

			// Detach from internal pointer and release frame to caller
			iabParserFrame_ = nullptr;

			return kIABNoError;
		}

		oIABFrame = nullptr;
		return kIABParserNoParsedFrameError;
	}

    IABSampleRateType IABParser::GetSampleRate()
    {
        IABSampleRateType sampleRate;
        iabParserFrame_->GetSampleRate(sampleRate);
        return sampleRate;
        
    }
    
    IABFrameRateType IABParser::GetFrameRate()
    {
        IABFrameRateType frameRate;
        iabParserFrame_->GetFrameRate(frameRate);
        return frameRate;
    }
    
    uint32_t IABParser::GetFrameSampleCount()
    {
        uint32_t frameSampleCount = 0;
		IABFrameRateType frameRate;
		IABSampleRateType sampleRate;

		iabParserFrame_->GetFrameRate(frameRate);
        iabParserFrame_->GetSampleRate(sampleRate);
		frameSampleCount = GetIABNumFrameSamples(frameRate, sampleRate);

        return frameSampleCount;
        
    }
    
    IABElementCountType IABParser::GetFrameSubElementCount()
    {
        IABElementCountType count = 0;
    
        if (nullptr != iabParserFrame_)
        {
            iabParserFrame_->GetSubElementCount(count);
        }
        
        return count;

    }
    
	uint32_t IABParser::GetUnallowedFrameSubElementCount()
	{
		return unAllowedFrameSubElementsCount_;
	}

	IABMaxRenderedRangeType IABParser::GetMaximumAssetsToBeRendered()
    {
        IABMaxRenderedRangeType maxRendered = 0;
        
        if (nullptr != iabParserFrame_)
        {
            iabParserFrame_->GetMaxRendered(maxRendered);
        }

        return maxRendered;
    }

    iabError IABParser::GetAudioAssetFromDLC(IABAudioDataIDType iAudioDataID, uint32_t iNumSamples, int32_t *oAudioSamples)
    {
        if (nullptr == iabParserFrame_)
        {
            return kIABParserNoParsedFrameError;
        }

        if ((nullptr == oAudioSamples) || (0 == iNumSamples))
        {
            return kIABBadArgumentsError;
        }
        
        std::vector<IABElement*> frameSubElements;
        std::vector<IABElement*>::const_iterator iterFSE;
        IABAudioDataIDType audioDataID;
        
        iabParserFrame_->GetSubElements(frameSubElements);
        IABAudioDataDLC *dlcElement = nullptr;
        
        for (iterFSE = frameSubElements.begin(); iterFSE != frameSubElements.end(); iterFSE++)
        {
            dlcElement = dynamic_cast<IABAudioDataDLC*>(*iterFSE);
            if (dlcElement)
            {
                dlcElement->GetAudioDataID(audioDataID);
                if (audioDataID == iAudioDataID)
                {
                    // found DLC element
                    IABSampleRateType dlcSampleRate;
                    dlcElement->GetDLCSampleRate(dlcSampleRate);

                    if (kIABNoError != dlcElement->DecodeDLCToMonoPCM(oAudioSamples, iNumSamples, dlcSampleRate))
                    {
                        return kIABParserDLCDecodingError;
                    }
                    
                    break;
                }
            }
            
        }
        
        return kIABNoError;
        
    }

    const std::string& IABParser::getStringForErrorCode(commonErrorCodes iErrorCode)
    {

        std::map<commonErrorCodes, std::string> errorCodeMap;

        errorCodeMap[kIABBadArgumentsError] = "IABParser has been given an invalid argument.\n";
        errorCodeMap[kIABAlreadyInitError] = "Attempting to re-initialize an already initialized variable or data structure.\n";
        errorCodeMap[kIABMemoryError] = "Memory allocation failed";
        errorCodeMap[kIABNoSuchParameterError] = "Attempting to access a non-existing parameter.\n";
        errorCodeMap[kIABNotImplementedError] = "IABParser has encountered a bitstream feature that is not supported by the current parser library.\n";
        errorCodeMap[kIABParserGeneralError] = "IABParser has encountered an unspecified error while parsing the bitstream.\n";
        errorCodeMap[kIABParserParsingError] = "IABParser has encountered an unspecified error while parsing the bitstream.\n";
        errorCodeMap[kIABParserBitstreamReaderNotPresentError] = "IABParser does not have a bitstream reader to parse the bitstream.\n";
        errorCodeMap[kIABParserEndOfStreamReached] = "IABParser has encountered an end of stream condition before a complete frame has beed parsed.\n";
        errorCodeMap[kIABParserMissingPreambleError] = "IABParser has encountered an error while parsing. The bitstream preamble subframe is missing.\n";
        errorCodeMap[kIABParserIASubFrameHeaderError] = "IABParser has encountered an error while parsing the bitstream subFrame header field.\n";
        errorCodeMap[kIABParserIABElementHeaderError] = "IABParser has encountered an error while parsing the header field of an element.\n";
        errorCodeMap[kIABParserIABFrameError] = "IABParser has encountered an error while parsing a IAB frame.\n";
        errorCodeMap[kIABParserIABBedDefinitionError] = "IABParser has encountered an error while parsing a IAB bed definition element.\n";
        errorCodeMap[kIABParserIABChannelError] = "IABParser has encountered an error while parsing a IAB bed definition channel.\n";
        errorCodeMap[kIABParserIABObjectDefinitionError] = "IABParser has encountered an error while parsing a IAB object definition element.\n";
        errorCodeMap[kIABParserIABObjectSubBlockError] = "IABParser has encountered an error while parsing a IAB object definition subBlock.\n";
        errorCodeMap[kIABParserDLCDecodingError] = "IABParser has encountered an error while parsing or decoding a IAB DLC element.\n";
        errorCodeMap[kIABParserIABBedRemapSubBlockError] = "IABParser has encountered an error while parsing a IAB bed definition remap subBlock.\n";
        errorCodeMap[kIABParserIABBedRemapError] = "IABParser has encountered an error while parsing a IAB bed definition remap.\n";
        errorCodeMap[kIABParserIABZone19SubBlockError] = "IABParser has encountered an error while parsing a IAB object definition zone19 subBlock.\n";
        errorCodeMap[kIABParserIABObjectZone19Error] = "IABParser has encountered an error while parsing a IAB object definition zone19.\n";
        errorCodeMap[kIABParserIABAuthoringToolInfoError] = "IABParser has encountered an error while parsing the authoring tool information element.\n";
        errorCodeMap[kIABParserIABUserDataError] = "IABParser has encountered an error while parsing the user data element.\n";
                
        // ***********************
        // Validation related error
        // ***********************

        errorCodeMap[kIABDataFieldFrameHeaderChanged] = "IABParser has found version number, frame rate or sample rate field in frame header changed between two frames.\n";
        errorCodeMap[kIABDataFieldInvalidFrameHeader] = "IABParser has found invalid data field(s) in the frame header.\n";
        errorCodeMap[kIABDataFieldFrameSubElementsNotConsistent] = "IABParser has found frame sub-elements containing invalid or inconsistent data field(s) while validating a frame.\n";
        errorCodeMap[kIABDataFieldInvalidBedDefinition] = "IABParser has found invalid contents in a bed definition while validating a frame.\n";
        errorCodeMap[kIABDataFieldInvalidBedChannel] = "IABParser has found invalid contents in a bed channel while validating a frame.\n";
        errorCodeMap[kIABDataFieldInvalidBedRemap] = "IABParser has found invalid contents in a bed remap while validating a frame.\n";
        errorCodeMap[kIABDataFieldInvalidObjectDefinition] = "IABParser has found invalid contents in a object definition while validating a frame.\n";
        errorCodeMap[kIABDataFieldInvalidObjectSubBlock] = "IABParser has found invalid contents in a object subBlock while validating a frame.\n";
        errorCodeMap[kIABDataFieldInvalidZone19] = "IABParser has found invalid contents in a object zone19 while validating a frame.\n";
        errorCodeMap[kIABDataFieldInvalidDLC] = "IABParser has found invalid contents in a DLC element while validating a frame.\n";
        errorCodeMap[kIABDataFieldInvalidPCM] = "IABParser has found invalid contents in a PCM element while validating a frame.\n";
        errorCodeMap[kIABDataFieldInvalidAuthoringToolInfo] = "IABParser has found invalid contents in a AuthoringToolInfo element while validating a frame.\n";
        errorCodeMap[kIABDataFieldInvalidUserData] = "IABParser has found invalid contents in a UserData element while validating a frame.\n";
        errorCodeMap[kIABGeneralError] = "IABParser has encountered an unspecified error while parsing the bitstream.\n";
        
        std::map<commonErrorCodes, std::string>::iterator iter;
        iter = errorCodeMap.find(iErrorCode);
        
        if (iter != errorCodeMap.end())
        {
            return iter->second;
        }
        else
        {
            return errorCodeMap[kIABGeneralError];
        }
    }

} // namespace ImmersiveAudioBitstream
} // namespace SMPTE
