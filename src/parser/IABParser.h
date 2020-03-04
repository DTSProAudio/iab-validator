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
 * Header file for the IAB Packer implementation.
 *
 * @file
 */

#ifndef __IABPARSER_H__
#define	__IABPARSER_H__

#include "IABElements.h"
#include "IABParserAPI.h"

namespace SMPTE
{
namespace ImmersiveAudioBitstream
{
    /**
     *
     * IAB Parser class to parse IAB frame elements from input bitstream.
     *
     */
    
    class IABParser : public IABParserInterface
    {
    public:

		// Constructor
		IABParser(std::istream* iInputStream);
		IABParser();

		// Destructor
		~IABParser();

        /**
         *
         * Returns the API version of the Packer.
         *
         * @sa IABParserInterface
         *
         */
        void GetAPIVersion(IABAPIVersionType& oVersion);
        
        /**
         *
         * Gets bitstream version. Note that this API should only be used after ParseIABFrame() has been called
         * and without errors, otherwise, a value of 0 is returned.
         *
         * @sa IABParserInterface
         *
         */
        iabError  GetBitstreamVersion(IABVersionNumberType& oVersion) const;

        
		/** Parse an IABFrame
         *
         * @sa IABParserInterface
         *
         */
        iabError ParseIABFrame();

		/** Parse an IABFrame from buffer iIABFrameDataBuffer.
		*
		* @sa IABParserInterface
		*
		*/
		iabError ParseIABFrame(char* iIABFrameDataBuffer, uint32_t iBufferSize);

        /** Gets a constant reference to IABFrameInterface pointer
         *
         * @sa IABParserInterface
         *
         */
        iabError GetIABFrame(const IABFrameInterface*& oIABFrame);

		/** Get, take over and own IABFrameInterface pointer to parsed frame.
		* Note that caller takes over ownership of the parsed frame, and must delete object after use.
		*
		* @sa IABParserInterface
		*
		*/
		iabError GetIABFrameReleased(IABFrameInterface*& oIABFrame);

		/** Gets audio sample rate of IA bitstream
         *
         * @sa IABParserInterface
         *
         */
        IABSampleRateType GetSampleRate();
        
        /** Gets frame rate of IA bitstream
         *
         * @sa IABParserInterface
         *
         */
        IABFrameRateType GetFrameRate();
        
        /** Gets number of samples per IA bed channel or object in the frame
         *
         * @sa IABParserInterface
         *
         */
        uint32_t GetFrameSampleCount();
        
        /** Gets number of sub-elements in the frame
         *
         * @sa IABParserInterface
         *
         */
        IABElementCountType GetFrameSubElementCount();
        
		/** Get the cumulative count of unallowed (illegal/unknown) frame sub-elements encountered
		*
		* @sa IABParserInterface
		*
		*/
		uint32_t GetUnallowedFrameSubElementCount();

		/** Gets maximum number of bed channels and objects in the frame to render together
         *
         * @sa IABParserInterface
         *
         */
        IABMaxRenderedRangeType GetMaximumAssetsToBeRendered();
                
        /** Gets a string reference for the specified IAB error code.
         *
         * @sa IABParserInterface
         *
         */
        const std::string& getStringForErrorCode(commonErrorCodes iErrorCode);

    private:
        
        // GetAudioAsset assoicated with the given audio data ID.
        // The function calls DLC decoder to decode the channel into audioSample.
        iabError GetAudioAssetFromDLC(IABAudioDataIDType iAudioDataID, uint32_t iNumSamples, int32_t *oAudioSamples);
        
        // Pointer to the IAB frame
        IABFrameInterface*          iabParserFrame_;
        
        // Pointer to input stream that contains the IA bitstream to be parsed
        std::istream*               iabStream_;
        
		// Number of times unknown element IDs are found in parsing.
		// Cumulative over frames for the lifetime of IABParser instance.
		uint32_t unAllowedFrameSubElementsCount_;
	};

} // namespace ImmersiveAudioBitstream
} // namespace SMPTE

#endif // __IABPARSER_H__
