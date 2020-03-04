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
* IAB Validator Implementation
*
* @file
*/

#include <vector>
#include <stdlib.h>

#include "IABValidator.h"

namespace SMPTE
{
namespace ImmersiveAudioBitstream
{
    // Embed legal text in Validator library binary and application executables that contain the Validator library
    const char *g_xperi_copyright_validator = "IABValidatorLib, Copyright (c) 2020 Xperi Corporation (and its subsidiaries). All rights reserved. Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"),to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.";
    
	// Validator handler implementation

	// Constructor
	IABCumulativeEventHandler::IABCumulativeEventHandler()
	{
		result_Cinema_ST2098_2_2018_ = kInvalid;
		result_Cinema_ST429_18_2019_ = kInvalid;
		result_DbyCinema_ = kInvalid;
		result_IMF_ST2098_2_2019_ = kInvalid;
		result_IMF_ST2067_201_2019_ = kInvalid;
		result_DbyIMF_ = kInvalid;
	}

	// IABCumulativeEventHandler::Handle() implementation
	bool IABCumulativeEventHandler::Handle(const ValidationIssue& iIssue)
	{
		switch (iIssue.isBeingValidated_)
		{
		case kConstraints_set_Cinema_ST2098_2_2018:
			// log issue
			issues_Cinema_ST2098_2_2018_.push_back(iIssue);
			// Combining issue into aggregated state
			switch (iIssue.event_)
			{
			case ErrorEvent:
				state_Cinema_ST2098_2_2018_.hasError = true;
				break;
			case WarningEvent:
				state_Cinema_ST2098_2_2018_.hasWarning = true;
				break;
			default:
				break;
			}
			break;

		case kConstraints_set_Cinema_ST429_18_2019:
			// log issue
			issues_Cinema_ST429_18_2019_.push_back(iIssue);
			// Combining issue into aggregated state
			switch (iIssue.event_)
			{
			case ErrorEvent:
				state_Cinema_ST429_18_2019_.hasError = true;
				break;
			case WarningEvent:
				state_Cinema_ST429_18_2019_.hasWarning = true;
				break;
			default:
				break;
			}
			break;

		case kConstraints_set_DbyCinema:
			// log issue
			issues_DbyCinema_.push_back(iIssue);
			// Combining issue into aggregated state
			switch (iIssue.event_)
			{
			case ErrorEvent:
				state_DbyCinema_.hasError = true;
				break;
			case WarningEvent:
				state_DbyCinema_.hasWarning = true;
				break;
			default:
				break;
			}
			break;

		case kConstraints_set_IMF_ST2098_2_2019:
			// log issue
			issues_IMF_ST2098_2_2019_.push_back(iIssue);
			// Combining issue into aggregated state
			switch (iIssue.event_)
			{
			case ErrorEvent:
				state_IMF_ST2098_2_2019_.hasError = true;
				break;
			case WarningEvent:
				state_IMF_ST2098_2_2019_.hasWarning = true;
				break;
			default:
				break;
			}
			break;

		case kConstraints_set_IMF_ST2067_201_2019:
			// log issue
			issues_IMF_ST2067_201_2019_.push_back(iIssue);
			// Combining issue into aggregated state
			switch (iIssue.event_)
			{
			case ErrorEvent:
				state_IMF_ST2067_201_2019_.hasError = true;
				break;
			case WarningEvent:
				state_IMF_ST2067_201_2019_.hasWarning = true;
				break;
			default:
				break;
			}
			break;

		case kConstraints_set_DbyIMF:
			// log issue
			issues_DbyIMF_.push_back(iIssue);
			// Combining issue into aggregated state
			switch (iIssue.event_)
			{
			case ErrorEvent:
				state_DbyIMF_.hasError = true;
				break;
			case WarningEvent:
				state_DbyIMF_.hasWarning = true;
				break;
			default:
				break;
			}
			break;

		default:
			break;
		}

		// Deal with what are defined as "validation-stop"/fatal level of issues (errors)
		// If any of these happens, return false to "suggest/recommend" validation termination.

		// ....



		// Return true for now
		return true;
	}

	// IABCumulativeEventHandler::GetValidationResult() implementation
	const ValidationResult& IABCumulativeEventHandler::GetValidationResult(SupportedConstraintsSet iConstraintSet)
	{
		static ValidationResult resultForUndefinedConstraintSets = kInvalid;

		// ********************************
		// General workflow - convert internal state to result before return it to callers
		//

		// ********** Update validation result for kConstraints_set_Cinema_ST2098_2_2018
		//
		if (state_Cinema_ST2098_2_2018_.hasError)
		{
			result_Cinema_ST2098_2_2018_ = kInvalid;
		}
		else if (state_Cinema_ST2098_2_2018_.hasWarning)
		{
			result_Cinema_ST2098_2_2018_ = kValidWithWarning;
		}
		else
		{
			result_Cinema_ST2098_2_2018_ = kValid;
		}

		if (iConstraintSet == kConstraints_set_Cinema_ST2098_2_2018)
		{
			return result_Cinema_ST2098_2_2018_;
		}

		// ********** Update validation result for kConstraints_set_Cinema_ST429_18_2019
		// 
		if (state_Cinema_ST429_18_2019_.hasError
			|| (result_Cinema_ST2098_2_2018_ == kInvalid))
		{
			result_Cinema_ST429_18_2019_ = kInvalid;
		}
		else if (state_Cinema_ST429_18_2019_.hasWarning
			|| (result_Cinema_ST2098_2_2018_ == kValidWithWarning))
		{
			result_Cinema_ST429_18_2019_ = kValidWithWarning;
		}
		else
		{
			result_Cinema_ST429_18_2019_ = kValid;
		}

		if (iConstraintSet == kConstraints_set_Cinema_ST429_18_2019)
		{
			return result_Cinema_ST429_18_2019_;
		}

		// ********** Update validation result for kConstraints_set_DbyCinema
		// 
		if (state_DbyCinema_.hasError
			|| (result_Cinema_ST2098_2_2018_ == kInvalid)
			|| (result_Cinema_ST429_18_2019_ == kInvalid))
		{
			result_DbyCinema_ = kInvalid;
		}
		else if (state_DbyCinema_.hasWarning
			|| (result_Cinema_ST2098_2_2018_ == kValidWithWarning)
			|| (result_Cinema_ST429_18_2019_ == kValidWithWarning))
		{
			result_DbyCinema_ = kValidWithWarning;
		}
		else
		{
			result_DbyCinema_ = kValid;
		}

		if (iConstraintSet == kConstraints_set_DbyCinema)
		{
			return result_DbyCinema_;
		}

		// ********** Update validation result for kConstraints_set_IMF_ST2098_2_2019
		// 
		if (state_IMF_ST2098_2_2019_.hasError)
		{
			result_IMF_ST2098_2_2019_ = kInvalid;
		}
		else if (state_IMF_ST2098_2_2019_.hasWarning)
		{
			result_IMF_ST2098_2_2019_ = kValidWithWarning;
		}
		else
		{
			result_IMF_ST2098_2_2019_ = kValid;
		}

		if (iConstraintSet == kConstraints_set_IMF_ST2098_2_2019)
		{
			return result_IMF_ST2098_2_2019_;
		}

		// ********** Update validation result for kConstraints_set_IMF_ST2067_201_2019
		//
		if (state_IMF_ST2067_201_2019_.hasError
			|| (result_IMF_ST2098_2_2019_ == kInvalid))
		{
			result_IMF_ST2067_201_2019_ = kInvalid;
		}
		else if (state_IMF_ST2067_201_2019_.hasWarning
			|| (result_IMF_ST2098_2_2019_ == kValidWithWarning))
		{
			result_IMF_ST2067_201_2019_ = kValidWithWarning;
		}
		else
		{
			result_IMF_ST2067_201_2019_ = kValid;
		}

		if (iConstraintSet == kConstraints_set_IMF_ST2067_201_2019)
		{
			return result_IMF_ST2067_201_2019_;
		}

		// ********** Update validation result for kConstraints_set_DbyIMF
		//
		if (state_DbyIMF_.hasError
			|| (result_IMF_ST2098_2_2019_ == kInvalid)
			|| (result_IMF_ST2067_201_2019_ == kInvalid))
		{
			result_DbyIMF_ = kInvalid;
		}
		else if (state_DbyIMF_.hasWarning
			|| (result_IMF_ST2098_2_2019_ == kValidWithWarning)
			|| (result_IMF_ST2067_201_2019_ == kValidWithWarning))
		{
			result_DbyIMF_ = kValidWithWarning;
		}
		else
		{
			result_DbyIMF_ = kValid;
		}

		if (iConstraintSet == kConstraints_set_DbyIMF)
		{
			return result_DbyIMF_;
		}

		// If getting here, unspecified iConstraintSet, return kInvalid
		return resultForUndefinedConstraintSets;
	}

	// IABCumulativeEventHandler::GetValidationIssues() implementation
	const std::vector<ValidationIssue>& IABCumulativeEventHandler::GetValidationIssues(SupportedConstraintsSet iConstraintSet)
	{
		if (iConstraintSet == kConstraints_set_Cinema_ST2098_2_2018)
		{
			return issues_Cinema_ST2098_2_2018_;
		}
		else if (iConstraintSet == kConstraints_set_Cinema_ST429_18_2019)
		{
			// Need to combine issues reported in 2 lists, see below

			// clear combo list first
			issues_Combined_.clear();

			// Pre-allocate to sizes
			issues_Combined_.reserve(issues_Cinema_ST2098_2_2018_.size() + issues_Cinema_ST429_18_2019_.size());

			// Copy 1st list
			issues_Combined_.insert(issues_Combined_.end(), issues_Cinema_ST2098_2_2018_.begin(), issues_Cinema_ST2098_2_2018_.end());

			// Copy 2nd list
			issues_Combined_.insert(issues_Combined_.end(), issues_Cinema_ST429_18_2019_.begin(), issues_Cinema_ST429_18_2019_.end());

			// Return combo list
			return issues_Combined_;
		}
		else if (iConstraintSet == kConstraints_set_DbyCinema)
		{
			// Need to combine issues reported in 3 lists, see below

			// clear combo list first
			issues_Combined_.clear();

			// Pre-allocate to sizes
			issues_Combined_.reserve(issues_Cinema_ST2098_2_2018_.size() 
				+ issues_Cinema_ST429_18_2019_.size()
				+ issues_DbyCinema_.size());

			// Copy 1st list
			issues_Combined_.insert(issues_Combined_.end(), issues_Cinema_ST2098_2_2018_.begin(), issues_Cinema_ST2098_2_2018_.end());

			// Copy 2nd list
			issues_Combined_.insert(issues_Combined_.end(), issues_Cinema_ST429_18_2019_.begin(), issues_Cinema_ST429_18_2019_.end());

			// Copy 3rd list
			issues_Combined_.insert(issues_Combined_.end(), issues_DbyCinema_.begin(), issues_DbyCinema_.end());

			// Return combo list
			return issues_Combined_;
		}
		else if (iConstraintSet == kConstraints_set_IMF_ST2098_2_2019)
		{
			return issues_IMF_ST2098_2_2019_;
		}
		else if (iConstraintSet == kConstraints_set_IMF_ST2067_201_2019)
		{
			// Need to combine issues reported in 2 lists, see below

			// clear combo list first
			issues_Combined_.clear();

			// Pre-allocate to sizes
			issues_Combined_.reserve(issues_IMF_ST2098_2_2019_.size() + issues_IMF_ST2067_201_2019_.size());

			// Copy 1st list
			issues_Combined_.insert(issues_Combined_.end(), issues_IMF_ST2098_2_2019_.begin(), issues_IMF_ST2098_2_2019_.end());

			// Copy 2nd list
			issues_Combined_.insert(issues_Combined_.end(), issues_IMF_ST2067_201_2019_.begin(), issues_IMF_ST2067_201_2019_.end());

			// Return combo list
			return issues_Combined_;
		}
		else if (iConstraintSet == kConstraints_set_DbyIMF)
		{
			// Need to combine issues reported in 3 lists, see below

			// clear combo list first
			issues_Combined_.clear();

			// Pre-allocate to sizes
			issues_Combined_.reserve(issues_IMF_ST2098_2_2019_.size() 
				+ issues_IMF_ST2067_201_2019_.size()
				+ issues_DbyIMF_.size());

			// Copy 1st list
			issues_Combined_.insert(issues_Combined_.end(), issues_IMF_ST2098_2_2019_.begin(), issues_IMF_ST2098_2_2019_.end());

			// Copy 2nd list
			issues_Combined_.insert(issues_Combined_.end(), issues_IMF_ST2067_201_2019_.begin(), issues_IMF_ST2067_201_2019_.end());

			// Copy 3rd list
			issues_Combined_.insert(issues_Combined_.end(), issues_DbyIMF_.begin(), issues_DbyIMF_.end());

			// Return combo list
			return issues_Combined_;
		}
		else
		{
			// Unrecognized iConstraintSet. Return empty list.
			issues_Combined_.clear();
			return issues_Combined_;
		}
	}
	
	// IABCumulativeEventHandler::GetValidationIssuesSingleSetOnly() implementation
	const std::vector<ValidationIssue>& IABCumulativeEventHandler::GetValidationIssuesSingleSetOnly(SupportedConstraintsSet iConstraintSet)
	{
		if (iConstraintSet == kConstraints_set_Cinema_ST2098_2_2018)
		{
			return issues_Cinema_ST2098_2_2018_;
		}
		else if (iConstraintSet == kConstraints_set_Cinema_ST429_18_2019)
		{
			return issues_Cinema_ST429_18_2019_;
		}
		else if (iConstraintSet == kConstraints_set_DbyCinema)
		{
			return issues_DbyCinema_;
		}
		else if (iConstraintSet == kConstraints_set_IMF_ST2098_2_2019)
		{
			return issues_IMF_ST2098_2_2019_;
		}
		else if (iConstraintSet == kConstraints_set_IMF_ST2067_201_2019)
		{
			return issues_IMF_ST2067_201_2019_;
		}
		else if (iConstraintSet == kConstraints_set_DbyIMF)
		{
			return issues_DbyIMF_;
		}
		else
		{
			// Unrecognized iConstraintSet. Return empty list.
			issues_Combined_.clear();
			return issues_Combined_;
		}
	}

	// IABCumulativeEventHandler::ClearIssues() implementation
	void IABCumulativeEventHandler::ClearIssues(SupportedConstraintsSet iConstraintSet)
	{
		switch (iConstraintSet)
		{
		case kConstraints_set_Cinema_ST2098_2_2018:
			issues_Cinema_ST2098_2_2018_.clear();
			break;

		case kConstraints_set_Cinema_ST429_18_2019:
			issues_Cinema_ST429_18_2019_.clear();
			break;

		case kConstraints_set_DbyCinema:
			issues_DbyCinema_.clear();
			break;

		case kConstraints_set_IMF_ST2098_2_2019:
			issues_IMF_ST2098_2_2019_.clear();
			break;

		case kConstraints_set_IMF_ST2067_201_2019:
			issues_IMF_ST2067_201_2019_.clear();
			break;

		case kConstraints_set_DbyIMF:
			issues_DbyIMF_.clear();
			break;

		default:
			break;
		}
	}

	// IABCumulativeEventHandler::reset() implementation
	void IABCumulativeEventHandler::reset()
	{
		result_Cinema_ST2098_2_2018_ = kInvalid;
		result_Cinema_ST429_18_2019_ = kInvalid;
		result_DbyCinema_ = kInvalid;
		result_IMF_ST2098_2_2019_ = kInvalid;
		result_IMF_ST2067_201_2019_ = kInvalid;
		result_DbyIMF_ = kInvalid;

		state_Cinema_ST2098_2_2018_.reset();
		state_Cinema_ST429_18_2019_.reset();
		state_DbyCinema_.reset();
		state_IMF_ST2098_2_2019_.reset();
		state_IMF_ST2067_201_2019_.reset();
		state_DbyIMF_.reset();

		issues_Cinema_ST2098_2_2018_.clear();
		issues_Cinema_ST429_18_2019_.clear();
		issues_DbyCinema_.clear();
		issues_IMF_ST2098_2_2019_.clear();
		issues_IMF_ST2067_201_2019_.clear();
		issues_DbyIMF_.clear();
	}

	// ****************************************************************************
	// Factory methods for creating instances of IABValidator
	// ****************************************************************************

	// Create IABValidator instance
	IABValidatorInterface* IABValidatorInterface::Create()
	{
		IABValidator* iabValidator = nullptr;
		iabValidator = new IABValidator();
		return iabValidator;
	}

    // Deletes an IABValidator instance
    void IABValidatorInterface::Delete(IABValidatorInterface* iInstance)
    {
        delete iInstance;
    }

    /****************************************************************************
    *
    * IABValidator
    *
    *****************************************************************************/

	// Constructor
	IABValidator::IABValidator()
    {
		// Init for first frame. No persistence checking
		isFirstFrame_ = true;
		globalSampleRate_ = kIABSampleRate_48000Hz;
		globalBitDepth_ = kIABBitDepth_24Bit;
		globalFrameRate_ = kIABFrameRate_24FPS;
		globalMaxRendered_ = 0;
		globalFrameSubElementCount_ = 0;

		frameUnderValidation_ = NULL;

		// init validationIssue_: start at default.
    }

	// Destructor
	IABValidator::~IABValidator()
    {
    }

	// IABValidator::GetValidationResult() implementation
	const ValidationResult& IABValidator::GetValidationResult(SupportedConstraintsSet iConstraintSet)
	{
		return validatorHandler_.GetValidationResult(iConstraintSet);
	}

	// IABValidator::GetValidationIssues() implementation
	const std::vector<ValidationIssue>& IABValidator::GetValidationIssues(SupportedConstraintsSet iConstraintSet)
	{
		return validatorHandler_.GetValidationIssues(iConstraintSet);
	}

	// IABValidator::GetValidationIssuesSingleSetOnly() implementation
	const std::vector<ValidationIssue>& IABValidator::GetValidationIssuesSingleSetOnly(SupportedConstraintsSet iConstraintSet)
	{
		return validatorHandler_.GetValidationIssuesSingleSetOnly(iConstraintSet);
	}

	// IABValidator::ValidateIABFrame() implementation
	iabError IABValidator::ValidateIABFrame(const IABFrameInterface *iIABFrame, uint32_t iFrameIndex)
	{
		// The NULL handle likely is the only error that stops validation processing
		if (iIABFrame == NULL)
		{
			return kIABBadArgumentsError;
		}

		// Save a copy of pointer ref.
		frameUnderValidation_ = dynamic_cast<const IABFrame*>(iIABFrame);

		if (frameUnderValidation_ == NULL)
		{
			return kIABBadArgumentsError;
		}

		// Update issue carrier variable
		validationIssue_.reset();
		// Update frameIndex to client input frame #
		validationIssue_.frameIndex_ = iFrameIndex;

		bool isToContinue = true;

		// Validate frame
		isToContinue = iIABFrame->Validate(dynamic_cast<IABEventHandler&>(validatorHandler_), validationIssue_);

		if (isToContinue == false)
		{
			return kIABValidatorTerminationError;
		}

		// Use kIssueID_IAFrame as issue id_ for cross-frame validation
		validationIssue_.id_ = kIssueID_IAFrame;

		// Initialize frame-wise stats variables before collecting data for current frame
		//
		frameSubElements_.clear();
		frameBedMetaIDSet_.clear();
        frameBedRemapMetaIDSet_.clear();
		frameObjectMetaIDSet_.clear();
		frameEssenceAudioDataIDSet_.clear();
		frameDLCEssenceAudioDataIDSet_.clear();
		framePCMEssenceAudioDataIDSet_.clear();
		frameReferredAudioDataIDSet_.clear();
		bedChannelIDCompositionSet_.clear();
		frameBedsPersistenceParameters_.clear();
		frameObjectsPersistenceParameters_.clear();
		frameContinuousAudioMetaIDAudioIDs_.clear();
		frameContinuousAudioElementIDs_.clear();

		frameBedDefinitionElementCount_ = 0;
		frameObjectDefinitionElementCount_ = 0;
		frameAudioDataDLCElementCount_ = 0;
		frameAudioDataPCMElementCount_ = 0;
		frameAuthoringToolInfoElementCount_ = 0;
		frameUserDataElementCount_ = 0;
		frameUndefinedElementCount_ = 0;

		// Collect stats on beds, object, DLC, etc, for current frame
		//

		// Note: In-frame MetaID duplication will be reported during collection, as duplicated IDs
		// are saved as 1 in std::set

		// Retrieve frame sub-element list
		frameUnderValidation_->GetSubElements(frameSubElements_);

		// Iterate through frame sub-element list to collate info
		//
		std::vector<IABElement*>::const_iterator iterFrameSubElement;
		IABElementIDType elementID;

		for (iterFrameSubElement = frameSubElements_.begin(); iterFrameSubElement != frameSubElements_.end(); iterFrameSubElement++)
		{
			// Retrieve elementID
			(*iterFrameSubElement)->GetElementID(elementID);

			// Collect relevant info for each sub-element type
			//
			switch (elementID)
			{
			case kIABElementID_BedDefinition:
				isToContinue = CollectAndCheckBedDefinitionStats(dynamic_cast<const IABBedDefinition*>(*iterFrameSubElement));
				break;

			case kIABElementID_ObjectDefinition:
				isToContinue = CollectAndCheckObjectDefinitionStats(dynamic_cast<const IABObjectDefinition*>(*iterFrameSubElement));
				break;

			case kIABElementID_AudioDataDLC:
				isToContinue = CollectAndCheckAudioDataDLCStats(dynamic_cast<const IABAudioDataDLC*>(*iterFrameSubElement));
				break;

			case kIABElementID_AudioDataPCM:
				isToContinue = CollectAndCheckAudioDataPCMStats(dynamic_cast<const IABAudioDataPCM*>(*iterFrameSubElement));
				break;

			case kIABElementID_AuthoringToolInfo:
				frameAuthoringToolInfoElementCount_ += 1;
				break;

			case kIABElementID_UserData:
				frameUserDataElementCount_ += 1;
				break;

			default:
				frameUndefinedElementCount_ += 1;
				break;
			}

			if (isToContinue == false)
			{
				return kIABValidatorTerminationError;
			}
		}

        // #################################################
        // Cinema constraint sets validation starts
        // #################################################

		// Issue id_ may have been set to other values during CollectAndCheckxxx calls.
		// Restore validationIssue_.id_ back to reflect Frame level
		//
		validationIssue_.id_ = kIssueID_IAFrame;

		// Cross-element & cross-frame validation below
		//
		if (frameAuthoringToolInfoElementCount_ > 1)
		{
			// Update issue variable and log
            validationIssue_.isBeingValidated_ = kConstraints_set_Cinema_ST2098_2_2018;
			validationIssue_.event_ = WarningEvent;
			validationIssue_.errorCode_ = kValidateWarningAuthoringToolInfoMultipleElements;

			if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
			{
				return kIABValidatorTerminationError;
			}
		}

		// Check all referred essence elements are present for both Cinema and IMF
		if (isToContinue = ValidateEssenceElementsPresence(), isToContinue == false)
		{
			return kIABValidatorTerminationError;
		}

		// Check for unused DLC elements, for both Cinema and IMF
		if (isToContinue = CheckAndLogUnusedDLCElements(), isToContinue == false)
		{
			return kIABValidatorTerminationError;
		}

		// Check for unused PCM elements, for both Cinema and IMF
		if (isToContinue = CheckAndLogUnusedPCMElements(), isToContinue == false)
		{
			return kIABValidatorTerminationError;
		}

        // Restore validationIssue_.id_ back to reflect Frame level
        validationIssue_.id_ = kIssueID_IAFrame;

		// Has parser encountered any sub-element types that are outside of ST2098-2 specification? 
		uint32_t numUndefinedParsed = frameUnderValidation_->GetNumUndefinedSubElements();

		if (numUndefinedParsed > 0)
		{
			// Update issue variable and log
            validationIssue_.isBeingValidated_ = kConstraints_set_Cinema_ST2098_2_2018;
			validationIssue_.event_ = WarningEvent;
			validationIssue_.errorCode_ = kValidateWarningFrameContainUndefinedSubElement;

			if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
			{
				return kIABValidatorTerminationError;
			}
            
            // Register undefined element as error for ST429_18_201
            // This is also an error for DbyCinema

            // Update issue variable and log
            validationIssue_.isBeingValidated_ = kConstraints_set_Cinema_ST429_18_2019;
            validationIssue_.event_ = ErrorEvent;
            validationIssue_.errorCode_ = kValidateErrorIAFrameUndefinedElementType;

            if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
            {
                return kIABValidatorTerminationError;
            }
		}
        
        // Check number of BedDefinition elements in current frame
        if (frameBedDefinitionElementCount_ > 1)
        {
            // Only one allowed
            // Update issue variable and log
            validationIssue_.isBeingValidated_ = kConstraints_set_DbyCinema;
            validationIssue_.event_ = ErrorEvent;
            validationIssue_.errorCode_ = kValidateErrorDolCinBedDefinitionMultipleBedsNotAllowed;
            
            if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
            {
                return kIABValidatorTerminationError;
            }
        }
        else if ((frameBedDefinitionElementCount_ == 1) && (bedChannels_.size() > 10))
        {
            // Up to 10 channels max
            // Update issue variable and log
            validationIssue_.isBeingValidated_ = kConstraints_set_DbyCinema;
            validationIssue_.event_ = ErrorEvent;
            validationIssue_.errorCode_ = kValidateErrorDolCinBedDefinitionMaxChannelCountExceeded;
            
            if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
            {
                return kIABValidatorTerminationError;
            }            
        }
        
        // Check object count limit
        if (frameObjectDefinitionElementCount_ > ConstraintSet_DbyCinema.maxObjectCount_)
        {
            // Update issue variable and log
            validationIssue_.isBeingValidated_ = kConstraints_set_DbyCinema;
            validationIssue_.event_ = ErrorEvent;
            validationIssue_.errorCode_ = kValidateErrorDolCinObjectDefinitionMaxObjectCountExceeded;
            
            if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
            {
                return kIABValidatorTerminationError;
            }
        }
        
        // Check object meta ID against DbyCinema constraint
        if (frameObjectMetaIDSet_.size() > 0)
        {
            std::set<IABMetadataIDType>::iterator iterMetaID;
            IABMetadataIDType nextMetaID = 1;
            
            for (iterMetaID = frameObjectMetaIDSet_.begin(); iterMetaID != frameObjectMetaIDSet_.end(); iterMetaID++)
            {
                if (*iterMetaID != nextMetaID)
                {
                    // Update issue variable and log
                    validationIssue_.isBeingValidated_ = kConstraints_set_DbyCinema;
                    validationIssue_.event_ = ErrorEvent;
                    validationIssue_.errorCode_ = kValidateErrorDolCinObjectDefinitionNonSequenctialMetaID;
                    
                    if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
                    {
                        return kIABValidatorTerminationError;
                    }
                    
                    // Skip remaining objects
                    break;
                }
                
                nextMetaID++;
            }
        }

        // #################################################
        // Cinema constraint sets validation ends
        // #################################################

        
        // #################################################
        // IMF constraint sets validation starts
        // #################################################

        // Restore validationIssue_.id_ back to reflect Frame level
        validationIssue_.id_ = kIssueID_IAFrame;
        
        // Cross-element & cross-frame validation below
        //
        if (frameAuthoringToolInfoElementCount_ > 1)
        {
            // Update issue variable and log
            validationIssue_.isBeingValidated_ = kConstraints_set_IMF_ST2098_2_2019;
            validationIssue_.event_ = WarningEvent;
            validationIssue_.errorCode_ = kValidateWarningAuthoringToolInfoMultipleElements;
            
            if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
            {
                return kIABValidatorTerminationError;
            }
        }
        
        // Has parser encountered any sub-element types that are outside of ST2098-2 specification?
        if (numUndefinedParsed > 0)
        {
            // Update issue variable and log
            validationIssue_.isBeingValidated_ = kConstraints_set_IMF_ST2098_2_2019;
            validationIssue_.event_ = WarningEvent;
            validationIssue_.errorCode_ = kValidateWarningFrameContainUndefinedSubElement;
            
            if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
            {
                return kIABValidatorTerminationError;
            }
        }

		// Check continuous audio track index sequence as required by DbyIMF
		isToContinue = CollectAndCheckFrameContinuousAudioSequences(frameSubElements_);

		if (isToContinue == false)
		{
			return kIABValidatorTerminationError;
		}
        
        // #################################################
        // IMF constraint sets validation ends
        // #################################################


        // #################################################
        // Validation checks common to Cinema and IMF
        // #################################################

		// If it is the first frame for validation, save selected parameters as initial global values
		// for persistence validation where necessary
		//
		if (isFirstFrame_)
		{
			// save frame-level parameters as global
			SaveFrameParameterToGlobal();

			// First frame already done. Set flag accordingly
			isFirstFrame_ = false;
		}
		else
		{
			// verify against saved global values for persistence check
            if (isToContinue = ValidateFrameParametersPersistence(), isToContinue == false)
            {
                return kIABValidatorTerminationError;
            }

            // verify frame sub-element against saved global values for persistence check
            if (isToContinue = ValidateFrameSubElementPersistence(), isToContinue == false)
            {
                return kIABValidatorTerminationError;
            }
            
			// verify continuous audio sequence persistence
			if ( (globalContinuousAudioMetaIDAudioIDs_ != frameContinuousAudioMetaIDAudioIDs_)
				|| (globalContinuousAudioElementIDs_ != frameContinuousAudioElementIDs_) )
			{
				// Update issue variable and log
				validationIssue_.isBeingValidated_ = kConstraints_set_DbyIMF;
				validationIssue_.event_ = ErrorEvent;
				validationIssue_.errorCode_ = kValidateErrorDolIMFContinuousAudioSequenceNotPersistent;

				if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
				{
					return kIABValidatorTerminationError;
				}
			}
			
			// update global with frame-level parameters for next frame
			// (TODO: This is a "save", not an update. Need a separate update global
			// function to truly update and aggrgate to global values, not over-write.)
            SaveFrameParameterToGlobal();
		}

		// Prepare to exit function
		if (isToContinue == true)
		{
			return kIABNoError;
		}
		else
		{
			return kIABValidatorTerminationError;
		}
	}

	// IABValidator::CollectAndCheckBedDefinitionStats() implementation
	bool IABValidator::CollectAndCheckBedDefinitionStats(const IABBedDefinition *iBedDefinition)
	{
		static uint32_t bedDefinitionSubElementLevel = 0;

		IABMetadataIDType metaID;
		size_t isDuplicated = 0;
		bool isToContinue = true;
        
        // Note: Only need to collect data for frame level BedDefinition and not for their sub-elements,
        // since ST2067-201-2019 does not allow sub-elements in BedDefinition, presence of sub-elements will be processed seprately.
		PersistenceParameters bedPersistenceParameters;

		iBedDefinition->GetMetadataID(metaID);

		// Update validationIssue_.id_ so that any issues reported from this function
		// is for BedDefinition element with metaID value.
		//
		validationIssue_.id_ = metaID;

		// If metaID is already in frameBedMetaIDSet_, it's a duplicate
		if ((isDuplicated = frameBedMetaIDSet_.count(metaID)) == 1)
		{
			// Update issue variable and log for Cinema ST2098_2_2018
            validationIssue_.isBeingValidated_ = kConstraints_set_Cinema_ST2098_2_2018;
			validationIssue_.event_ = ErrorEvent;
			validationIssue_.errorCode_ = kValidateErrorBedDefinitionDuplicateMetaID;

			if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
			{
				return isToContinue;
			}
            
            // Update issue variable and log for IMF ST2098_2_2019
            validationIssue_.isBeingValidated_ = kConstraints_set_IMF_ST2098_2_2019;
            validationIssue_.event_ = ErrorEvent;
            validationIssue_.errorCode_ = kValidateErrorBedDefinitionDuplicateMetaID;
            
            if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
            {
                return isToContinue;
            }
		}

		// Insert MetaID into frame set
		frameBedMetaIDSet_.insert(metaID);
        
		// Collect bed channelID, etc for the bed
		// Run Validate() on all bed channels
		std::vector<IABChannel*>::const_iterator iterChannel;
		iBedDefinition->GetBedChannels(bedChannels_);
		IABChannelIDType channelID;

		// Clear channel composition list first
		bedChannelIDCompositionSet_.clear();
		for (iterChannel = bedChannels_.begin(); iterChannel != bedChannels_.end(); iterChannel++)
		{
			(*iterChannel)->GetChannelID(channelID);

			// If channelID is already in bedChannelIDCompositionSet_, it's a duplicate
			if ((isDuplicated = bedChannelIDCompositionSet_.count(static_cast<int32_t>(channelID))) == 1)
			{
				// Update issue variable and log for Cinema ST2098_2_2018
                validationIssue_.isBeingValidated_ = kConstraints_set_Cinema_ST2098_2_2018;
				validationIssue_.event_ = ErrorEvent;
				validationIssue_.errorCode_ = kValidateErrorBedDefinitionDuplicateChannelID;

				if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
				{
					return isToContinue;
				}
                
                // Update issue variable and log for IMF ST2098_2_2019
                validationIssue_.isBeingValidated_ = kConstraints_set_IMF_ST2098_2_2019;
                validationIssue_.event_ = ErrorEvent;
                validationIssue_.errorCode_ = kValidateErrorBedDefinitionDuplicateChannelID;
                
                if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
                {
                    return isToContinue;
                }
			}

			// Insert channelID into bedChannelIDCompositionSet_
			bedChannelIDCompositionSet_.insert(channelID);
            
			// Retrieve audio data ID for the channel and add it to frameReferredAudioDataIDSet_
			IABAudioDataIDType audioDataID;
			(*iterChannel)->GetAudioDataID(audioDataID);
			frameReferredAudioDataIDSet_.insert(audioDataID);
		}

        // Save list for cross frame persistence check
		bedPersistenceParameters.channelIDList_ = bedChannelIDCompositionSet_;
        
		// Check sub-elements
		std::vector<IABElement*> bedSubElements;
		std::vector<IABElement*>::const_iterator iterBedSubElement;
		iBedDefinition->GetSubElements(bedSubElements);
		IABElementIDType elementID;

		std::set<IABUseCaseType> subElementUseCases;
		size_t foundCount = 0;

		// Collect and check sub-elements if (bedDefinitionSubElementLevel == 0)
		//
		if (bedDefinitionSubElementLevel == 0)
		{
			// Save bedConditional for persistence check
			uint1_t bedConditional;
            iBedDefinition->GetConditionalBed(bedConditional);
			bedPersistenceParameters.conditional_ = bedConditional;

            if (bedConditional)
            {
				// Save bedUseCase for persistence check
				IABUseCaseType bedUseCase;
                iBedDefinition->GetBedUseCase(bedUseCase);
				bedPersistenceParameters.useCase_ = bedUseCase;
            }
            
            // Data information collected for bed persistence check, save.
			frameBedsPersistenceParameters_[metaID] = bedPersistenceParameters;

			// Increment frameBedDefinitionElementCount_ when level == 0 
			frameBedDefinitionElementCount_ += 1;

			// Current BedDefinition element is a frame/root-level sub-element 
			for (iterBedSubElement = bedSubElements.begin(); iterBedSubElement != bedSubElements.end(); iterBedSubElement++)
			{
				// Retrieve elementID
				(*iterBedSubElement)->GetElementID(elementID);
                
                // ID of any issues reported for the sub-elements will be set to parent BedDefinition meta ID,
                // excpet for BedRemap duplicate ID case, which will be set to the BedRemap's meta ID.
                validationIssue_.id_ = metaID;

				// Validate sub-elements
				//
				switch (elementID)
				{
				case kIABElementID_BedDefinition:
					const IABBedDefinition* bedSubElement;
					bedSubElement = dynamic_cast<const IABBedDefinition*>(*iterBedSubElement);

					uint1_t subBedConditional;
					bedSubElement->GetConditionalBed(subBedConditional);

					// Check if a bed use case is repeated among all sub-elements
					// Check only when subelement conditionalBed_ is enabled
					if (subBedConditional)
					{
						IABUseCaseType subBedUseCase;
						bedSubElement->GetBedUseCase(subBedUseCase);

						// Is it already in subBedUseCase
						foundCount = subElementUseCases.count(subBedUseCase);

						// If yes, duplicate
						if (foundCount == 1)
						{
                            // Update issue variable and log for Cinema ST2098_2_2018
                            validationIssue_.isBeingValidated_ = kConstraints_set_Cinema_ST2098_2_2018;
							validationIssue_.event_ = ErrorEvent;
							validationIssue_.errorCode_ = kValidateErrorBedDefinitionMultiActiveSubElements;

							if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
							{
								return isToContinue;
							}
                            
                            // Update issue variable and log for IMF ST2098_2_2019
                            validationIssue_.isBeingValidated_ = kConstraints_set_IMF_ST2098_2_2019;
                            validationIssue_.event_ = ErrorEvent;
                            validationIssue_.errorCode_ = kValidateErrorBedDefinitionMultiActiveSubElements;
                            
                            if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
                            {
                                return isToContinue;
                            }
						}
						else
						{
							subElementUseCases.insert(subBedUseCase);
						}
					}
					else
					{
                        // Update issue variable and log for Cinema ST2098_2_2018
                        validationIssue_.isBeingValidated_ = kConstraints_set_Cinema_ST2098_2_2018;
						validationIssue_.event_ = WarningEvent;
						validationIssue_.errorCode_ = kValidateWarningBedDefinitionAlwaysActiveSubElement;

						if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
						{
							return isToContinue;
						}
                        
                        // Update issue variable and log for IMF ST2098_2_2019
                        validationIssue_.isBeingValidated_ = kConstraints_set_IMF_ST2098_2_2019;
                        validationIssue_.event_ = WarningEvent;
                        validationIssue_.errorCode_ = kValidateWarningBedDefinitionAlwaysActiveSubElement;
                        
                        if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
                        {
                            return isToContinue;
                        }
					}

					// Collect sub-element BedDefinition stats
					// Before going into sub-element, set bedDefinitionSubElementLevel to 1
					bedDefinitionSubElementLevel = 1;
					isToContinue = CollectAndCheckBedDefinitionStats(bedSubElement);

					// set bedDefinitionSubElementLevel back to 0
					bedDefinitionSubElementLevel = 0;
					break;

				case kIABElementID_BedRemap:
					const IABBedRemap* bedRemapSubElement;
					bedRemapSubElement = dynamic_cast<const IABBedRemap*>(*iterBedSubElement);

					IABUseCaseType subBedRemapUseCase;
					bedRemapSubElement->GetRemapUseCase(subBedRemapUseCase);

					// Is it already in subBedUseCase
					foundCount = subElementUseCases.count(subBedRemapUseCase);

					// If yes, duplicate
					if (foundCount == 1)
					{
                        // Update issue variable and log for Cinema ST2098_2_2018
                        validationIssue_.isBeingValidated_ = kConstraints_set_Cinema_ST2098_2_2018;
						validationIssue_.event_ = ErrorEvent;
						validationIssue_.errorCode_ = kValidateErrorBedDefinitionMultiActiveSubElements;

						if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
						{
							return isToContinue;
						}
                        
                        // Update issue variable and log for IMF ST2098_2_2019
                        validationIssue_.isBeingValidated_ = kConstraints_set_IMF_ST2098_2_2019;
                        validationIssue_.event_ = ErrorEvent;
                        validationIssue_.errorCode_ = kValidateErrorBedDefinitionMultiActiveSubElements;
                        
                        if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
                        {
                            return isToContinue;
                        }
					}
					else
					{
						subElementUseCases.insert(subBedRemapUseCase);
					}
                    
                    IABMetadataIDType bedRemapMetaID;

                    bedRemapSubElement->GetMetadataID(bedRemapMetaID);
                    
                    // If metaID is already in frameBedRemapMetaIDSet_, it's a duplicate
                    if ((isDuplicated = frameBedRemapMetaIDSet_.count(bedRemapMetaID)) == 1)
                    {
                        // Update validationIssue_.id_ so that any issues reported from this function
                        // is for BedRemap element with metaID value.
                        //
                        validationIssue_.id_ = bedRemapMetaID;

                        // Update issue variable and log for Cinema ST2098_2_2018
                        validationIssue_.isBeingValidated_ = kConstraints_set_Cinema_ST2098_2_2018;
                        validationIssue_.event_ = ErrorEvent;
                        validationIssue_.errorCode_ = kValidateErrorBedRemapDuplicateMetaID;
                        
                        if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
                        {
                            return isToContinue;
                        }
                        
                        // Update issue variable and log for IMF ST2098_2_2019
                        validationIssue_.isBeingValidated_ = kConstraints_set_IMF_ST2098_2_2019;
                        validationIssue_.event_ = ErrorEvent;
                        validationIssue_.errorCode_ = kValidateErrorBedRemapDuplicateMetaID;
                        
                        if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
                        {
                            return isToContinue;
                        }
                    }
                    
                    // Insert MetaID into frame set
                    frameBedRemapMetaIDSet_.insert(bedRemapMetaID);

					break;

				default:
					// Ignore here
					break;
				}

				// Check if early termination has been called
				if (!isToContinue)
				{
					return isToContinue;
				}
			}
		}
		else
		{
			// Current BedDefinition element is already a sub-element of another parent BedDefinition element
			// In this case, further sub-element of BedDefinition type is not allowed (constrained) by 
			// ST2098-2:2018 specification.
			for (iterBedSubElement = bedSubElements.begin(); iterBedSubElement != bedSubElements.end(); iterBedSubElement++)
			{
				// Retrieve elementID
				(*iterBedSubElement)->GetElementID(elementID);

				// Validate sub-elements
				//
				switch (elementID)
				{
				case kIABElementID_BedDefinition:
					// Error condition per ST2098-2:2018 constraint
                    validationIssue_.isBeingValidated_ = kConstraints_set_Cinema_ST2098_2_2018;
					validationIssue_.event_ = ErrorEvent;
					validationIssue_.errorCode_ = kValidateErrorBedDefinitionHierarchyLevelExceeded;

					if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
					{
						return isToContinue;
					}
                        
                    // Error condition per ST2098-2:2019 constraint
                    validationIssue_.isBeingValidated_ = kConstraints_set_IMF_ST2098_2_2019;
                    validationIssue_.event_ = ErrorEvent;
                    validationIssue_.errorCode_ = kValidateErrorBedDefinitionHierarchyLevelExceeded;
                    
                    if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
                    {
                        return isToContinue;
                    }

					break;

				case kIABElementID_BedRemap:
					// Ok
					break;

				default:
					// Ignore here
					break;
				}

				// Check if early termination has been called
				if (!isToContinue)
				{
					return isToContinue;
				}
			}
		}

		return isToContinue;
	}

	// IABValidator::CollectAndCheckObjectDefinitionStats() implementation
	bool IABValidator::CollectAndCheckObjectDefinitionStats(const IABObjectDefinition *iObjectDefinition)
	{
		static uint32_t objectDefinitionSubElementLevel = 0;

		IABMetadataIDType metaID;
		IABAudioDataIDType audioDataID;
		size_t isDuplicated = 0;
		bool isToContinue = true;
        
        // Note: Only need to collect data for frame level ObjectDefinition and not for their sub-elements,
        // since ST2067-201-2019 does not allow sub-elements in ObjectDefinition, presence of sub-elements will be processed seprately.
		PersistenceParameters objectPersistenceParameters;					// member "objectPersistenceParameters.channelIDList_" not used for object

		iObjectDefinition->GetMetadataID(metaID);
        validationIssue_.id_ = metaID;

		// If metaID is already in frameObjectMetaIDSet_, it's a duplicate
		if ((isDuplicated = frameObjectMetaIDSet_.count(metaID)) == 1)
		{
            // Update issue variable and log for Cinema ST2098_2_2018
            validationIssue_.isBeingValidated_ = kConstraints_set_Cinema_ST2098_2_2018;
			validationIssue_.event_ = ErrorEvent;
			validationIssue_.errorCode_ = kValidateErrorObjectDefinitionDuplicateMetaID;

			if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
			{
				return isToContinue;
			}
            
            // Update issue variable and log for IMF ST2098_2_2019
            validationIssue_.isBeingValidated_ = kConstraints_set_IMF_ST2098_2_2019;
            validationIssue_.event_ = ErrorEvent;
            validationIssue_.errorCode_ = kValidateErrorObjectDefinitionDuplicateMetaID;
            
            if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
            {
                return isToContinue;
            }
		}

		// Insert MetaID into frame set
		frameObjectMetaIDSet_.insert(metaID);
        
		// Retrieve audio data ID and add it to frameReferredAudioDataIDSet_
		iObjectDefinition->GetAudioDataID(audioDataID);
		frameReferredAudioDataIDSet_.insert(audioDataID);

		// Check sub-elements
		std::vector<IABElement*> objectSubElements;
		std::vector<IABElement*>::const_iterator iterObjectSubElement;
		iObjectDefinition->GetSubElements(objectSubElements);
		IABElementIDType elementID;

		std::set<IABUseCaseType> subElementUseCases;
		size_t foundCount = 0;

		// Collect and check sub-elements if (objectDefinitionSubElementLevel == 0)
		//
		if (objectDefinitionSubElementLevel == 0)
		{
			// Save objectConditional for persistence check
            uint1_t objectConditional;
            iObjectDefinition->GetConditionalObject(objectConditional);
			objectPersistenceParameters.conditional_ = objectConditional;

            if (objectConditional)
            {
				// Save objectUseCase for persistence check
				IABUseCaseType objectUseCase;
                iObjectDefinition->GetObjectUseCase(objectUseCase);
				objectPersistenceParameters.useCase_ = objectUseCase;
            }
            
            // Data collected for object persistence check, save.
			frameObjectsPersistenceParameters_[metaID] = objectPersistenceParameters;

            uint32_t numZone19SubElementsFound = 0;

			// Increment frameObjectDefinitionElementCount_ when level == 0
			frameObjectDefinitionElementCount_ += 1;

			// Current ObjectDefinition element is a frame/root-level sub-element 
			for (iterObjectSubElement = objectSubElements.begin(); iterObjectSubElement != objectSubElements.end(); iterObjectSubElement++)
			{
				// Retrieve elementID
				(*iterObjectSubElement)->GetElementID(elementID);

				// Validate sub-elements
				//
				switch (elementID)
				{
				case kIABElementID_ObjectDefinition:
					const IABObjectDefinition* objectSubElement;
					objectSubElement = dynamic_cast<const IABObjectDefinition*>(*iterObjectSubElement);

					uint1_t subObjectConditional;
					objectSubElement->GetConditionalObject(subObjectConditional);

					// Check if an object use case is repeated among all sub-elements
					// Check only when subelement conditionalObject_ is enabled
					if (subObjectConditional)
					{
						IABUseCaseType subObjectUseCase;
						objectSubElement->GetObjectUseCase(subObjectUseCase);

						// Is it already in subBedUseCase
						foundCount = subElementUseCases.count(subObjectUseCase);

						// If yes, duplicate
						if (foundCount == 1)
						{
							// Update issue variable and log for Cinema ST2098_2_2018
                            validationIssue_.isBeingValidated_ = kConstraints_set_Cinema_ST2098_2_2018;
							validationIssue_.event_ = ErrorEvent;
							validationIssue_.errorCode_ = kValidateErrorObjectDefinitionMultiActiveSubElements;

							if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
							{
								return isToContinue;
							}
                            
                            // Update issue variable and log for IMF ST2098_2_2019
                            validationIssue_.isBeingValidated_ = kConstraints_set_IMF_ST2098_2_2019;
                            validationIssue_.event_ = ErrorEvent;
                            validationIssue_.errorCode_ = kValidateErrorObjectDefinitionMultiActiveSubElements;
                            
                            if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
                            {
                                return isToContinue;
                            }
						}
					}
					else
					{
                        // Update issue variable and log for Cinema ST2098_2_2018
                        validationIssue_.isBeingValidated_ = kConstraints_set_Cinema_ST2098_2_2018;
						validationIssue_.event_ = WarningEvent;
						validationIssue_.errorCode_ = kValidateWarningObjectDefinitionAlwaysActiveSubElement;

						if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
						{
							return isToContinue;
						}
                        
                        // Update issue variable and log for IMF ST2098_2_2019
                        validationIssue_.isBeingValidated_ = kConstraints_set_IMF_ST2098_2_2019;
                        validationIssue_.event_ = WarningEvent;
                        validationIssue_.errorCode_ = kValidateWarningObjectDefinitionAlwaysActiveSubElement;
                        
                        if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
                        {
                            return isToContinue;
                        }
					}

					// Collect sub-element ObjectDefinition stats
					// Before going into sub-element, set objectDefinitionSubElementLevel to 1
					objectDefinitionSubElementLevel = 1;
					isToContinue = CollectAndCheckObjectDefinitionStats(objectSubElement);

					// set objectDefinitionSubElementLevel back to 0
					objectDefinitionSubElementLevel = 0;
					break;

				case kIABElementID_ObjectZoneDefinition19:
                        
                    numZone19SubElementsFound++;
                        
                    if (numZone19SubElementsFound > 1)
                    {
                        // Update issue variable and log for Cinema ST2098_2_2018
                        validationIssue_.isBeingValidated_ = kConstraints_set_Cinema_ST2098_2_2018;
                        validationIssue_.event_ = WarningEvent;
                        validationIssue_.errorCode_ = kValidateWarningObjectDefinitionMultipleZone19SubElements;
                        
                        if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
                        {
                            return isToContinue;
                        }
                        
                        // Update issue variable and log for IMF ST2098_2_2019
                        validationIssue_.isBeingValidated_ = kConstraints_set_IMF_ST2098_2_2019;
                        validationIssue_.event_ = WarningEvent;
                        validationIssue_.errorCode_ = kValidateWarningObjectDefinitionMultipleZone19SubElements;
                        
                        if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
                        {
                            return isToContinue;
                        }                        
                    }
                        
					// No stats to be collected from IABObjectZoneDefinition19
					break;

				default:
					// Ignore here
					break;
				}

				// Check if early termination has been called
				if (!isToContinue)
				{
					return isToContinue;
				}
			}
		}
		else
		{
            uint32_t numZone19SubElementsFound = 0;

			// Current ObjectDefinition element is already a sub-element of another parent ObjectDefinition element
			// In this case, further sub-element of ObjectDefinition type is not allowed (constrained) by 
			// ST2098-2:2018 specification.
			for (iterObjectSubElement = objectSubElements.begin(); iterObjectSubElement != objectSubElements.end(); iterObjectSubElement++)
			{
				// Retrieve elementID
				(*iterObjectSubElement)->GetElementID(elementID);

				// Validate sub-elements
				//
				switch (elementID)
				{
				case kIABElementID_ObjectDefinition:
					// Error condition per ST2098-2:2018 constraint
                    validationIssue_.isBeingValidated_ = kConstraints_set_Cinema_ST2098_2_2018;
					validationIssue_.event_ = ErrorEvent;
					validationIssue_.errorCode_ = kValidateErrorObjectDefinitionHierarchyLevelExceeded;

					if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
					{
						return isToContinue;
					}
                        
                    // Error condition per ST2098-2:2019 constraint
                    validationIssue_.isBeingValidated_ = kConstraints_set_IMF_ST2098_2_2019;
                    validationIssue_.event_ = ErrorEvent;
                    validationIssue_.errorCode_ = kValidateErrorObjectDefinitionHierarchyLevelExceeded;
                    
                    if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
                    {
                        return isToContinue;
                    }

                    break;

				case kIABElementID_ObjectZoneDefinition19:
                        
                        numZone19SubElementsFound++;
                        
                        if (numZone19SubElementsFound > 1)
                        {
                            // Update issue variable and log for Cinema ST2098_2_2018
                            validationIssue_.isBeingValidated_ = kConstraints_set_Cinema_ST2098_2_2018;
                            validationIssue_.event_ = WarningEvent;
                            validationIssue_.errorCode_ = kValidateWarningObjectDefinitionMultipleZone19SubElements;
                            
                            if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
                            {
                                return isToContinue;
                            }
                            
                            // Update issue variable and log for IMF ST2098_2_2019
                            validationIssue_.isBeingValidated_ = kConstraints_set_IMF_ST2098_2_2019;
                            validationIssue_.event_ = WarningEvent;
                            validationIssue_.errorCode_ = kValidateWarningObjectDefinitionMultipleZone19SubElements;
                            
                            if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
                            {
                                return isToContinue;
                            }                        
                        }
					break;

				default:
					// Ignore here
					break;
				}

				// Check if early termination has been called
				if (!isToContinue)
				{
					return isToContinue;
				}
			}
		}

		return isToContinue;
	}

	// IABValidator::CollectAndCheckAudioDataDLCStats() implementation
	bool IABValidator::CollectAndCheckAudioDataDLCStats(const IABAudioDataDLC *iAudioDataDLC)
	{
		IABAudioDataIDType audioDataID;
		size_t isDuplicated = 0;
		bool isToContinue = true;

		iAudioDataDLC->GetAudioDataID(audioDataID);

		// Update validationIssue_.id_ so that any issues reported from this function
		// is for DLCAudioData element with audioDataID value.
		//
		validationIssue_.id_ = audioDataID;

		// If audioDataID is already in frameEssenceAudioDataIDSet_, it's a duplicate
		if ((isDuplicated = frameEssenceAudioDataIDSet_.count(audioDataID)) == 1)
		{
			// Update issue variable and log for Cinema ST2098_2_2018
            validationIssue_.isBeingValidated_ = kConstraints_set_Cinema_ST2098_2_2018;
			validationIssue_.event_ = ErrorEvent;
			validationIssue_.errorCode_ = kValidateErrorAudioDataDLCDuplicateAudioDataID;

			if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
			{
				return isToContinue;
			}
            
            // Update issue variable and log for IMF ST2098_2_2019
            validationIssue_.isBeingValidated_ = kConstraints_set_IMF_ST2098_2_2019;
            validationIssue_.event_ = ErrorEvent;
            validationIssue_.errorCode_ = kValidateErrorAudioDataDLCDuplicateAudioDataID;
            
            if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
            {
                return isToContinue;
            }
		}

		// Insert AudioDataID into frame sets
		frameEssenceAudioDataIDSet_.insert(audioDataID);
		frameDLCEssenceAudioDataIDSet_.insert(audioDataID);
		frameAudioDataDLCElementCount_ += 1;

		IABSampleRateType frameSampleRate;
		IABSampleRateType dlcSampleRate;

		// Validate frame sample rate and DLC element sample frate consistency
		frameUnderValidation_->GetSampleRate(frameSampleRate);
		iAudioDataDLC->GetDLCSampleRate(dlcSampleRate);

		// Report if disagree
		if (frameSampleRate != dlcSampleRate)
		{
			// Update issue variable and log for Cinema ST2098_2_2018
            validationIssue_.isBeingValidated_ = kConstraints_set_Cinema_ST2098_2_2018;
			validationIssue_.event_ = ErrorEvent;
			validationIssue_.errorCode_ = kValidateErrorAudioDataDLCSampleRateConflict;

			if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
			{
				return isToContinue;
			}
            
            // Update issue variable and log for IMF ST2098_2_2019
            validationIssue_.isBeingValidated_ = kConstraints_set_IMF_ST2098_2_2019;
            validationIssue_.event_ = ErrorEvent;
            validationIssue_.errorCode_ = kValidateErrorAudioDataDLCSampleRateConflict;
            
            if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
            {
                return isToContinue;
            }

		}

		return isToContinue;
	}

	// IABValidator::CollectAndCheckAudioDataPCMStats() implementation
	bool IABValidator::CollectAndCheckAudioDataPCMStats(const IABAudioDataPCM *iAudioDataPCM)
	{
		IABAudioDataIDType audioDataID;
		size_t isDuplicated = 0;
		bool isToContinue = true;

		iAudioDataPCM->GetAudioDataID(audioDataID);

		// If audioDataID is already in frameEssenceAudioDataIDSet_, it's a duplicate
		if ((isDuplicated = frameEssenceAudioDataIDSet_.count(audioDataID)) == 1)
		{
            validationIssue_.id_ = audioDataID;
            
			// Update issue variable and log for Cinema ST2098_2_2018
            validationIssue_.isBeingValidated_ = kConstraints_set_Cinema_ST2098_2_2018;
			validationIssue_.event_ = ErrorEvent;
			validationIssue_.errorCode_ = kValidateErrorAudioDataPCMDuplicateAudioDataID;

			if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
			{
				return isToContinue;
			}
            
            // Update issue variable and log for IMF ST2098_2_2019
            validationIssue_.isBeingValidated_ = kConstraints_set_IMF_ST2098_2_2019;
            validationIssue_.event_ = ErrorEvent;
            validationIssue_.errorCode_ = kValidateErrorAudioDataPCMDuplicateAudioDataID;
            
            if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
            {
                return isToContinue;
            }

		}

		// Insert AudioDataID into frame set
		frameEssenceAudioDataIDSet_.insert(audioDataID);
		framePCMEssenceAudioDataIDSet_.insert(audioDataID);
		frameAudioDataPCMElementCount_ += 1;

		return isToContinue;
	}

	// IABValidator::CollectAndCheckFrameContinuousAudioSequences() implementation
	bool IABValidator::CollectAndCheckFrameContinuousAudioSequences(const std::vector<IABElement*> &iSubElements)
	{
		bool isToContinue = true;

		bool hasObjectAppeared = false;						// Init to false. Set to true once first object appears in sub-element list
		bool isErrorReported = false;						// Set to true once the error is reported, so to avoid duplication
		uint32_t vectorSize = 0;							// To store current size of 2 sequence vectors (their sizes are the same/in sync) 

		IABMetadataIDType metaID;							// To collect MetaIDs from bed and object elements
		IABAudioDataIDType audioDataID;						// To collect AudioIDs from PCM essence elements
		IABElementIDType elementID;							// To collect ElementID from bed, object and linked PCM essence elements

		// Continuous Audio sequence error is reported as a frame level issue.
		validationIssue_.id_ = kIssueID_IAFrame;

		// Clear frame sequence lists.
		frameContinuousAudioMetaIDAudioIDs_.clear();
		frameContinuousAudioElementIDs_.clear();

		// Check through input frame sub-elements
		std::vector<IABElement*>::const_iterator iterFrameSubElement;
		for (iterFrameSubElement = iSubElements.begin(); iterFrameSubElement != iSubElements.end(); iterFrameSubElement++)
		{
			if (isErrorReported)
			{
				// Break further iterations in loop if error is already reported.
				// (Imcomplete sequence list is ok, as this type of error already occurred.)
				break;
			}

			// Retrieve elementID
			(*iterFrameSubElement)->GetElementID(elementID);

			// The following tasks are carried out below.
			// 1) Contruct frameContinuousAudioMetaIDAudioIDs_ sequence from bed, object and PCM types
			// 2) Contruct frameContinuousAudioElementIDs_ sequence from bed, object and PCM types
			// 3) Check continuous audio sequence and report error (only once)
			//
			switch (elementID)
			{
			case kIABElementID_BedDefinition:
				// First, check if any object has already appeared in frame sub-element list.
				// If yes, the sequence is already violated.
				if (hasObjectAppeared == true)
				{
					// Update issue variable and log for for DbyIMF
					validationIssue_.isBeingValidated_ = kConstraints_set_DbyIMF;
					validationIssue_.event_ = ErrorEvent;
					validationIssue_.errorCode_ = kValidateErrorDolIMFNotMeetingContinuousAudioSequence;

					// Report error. Set flag to avoid repeated reporting.
					isErrorReported = true;

					if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
					{
						return isToContinue;
					}

					break;								// Break off switch case
				}

				const IABBedDefinition* bedSubElement;
				bedSubElement = dynamic_cast<const IABBedDefinition*>(*iterFrameSubElement);

				// Retrieve MetaID
				bedSubElement->GetMetadataID(metaID);

				// Retrieve linked AudioDataIDs for each of the channels
				// bedSubElement->GetMetadataID(audioDataID);

				// Get number of channels in the bed, for checking linked PCM elements
				IABChannelCountType numberChannels;
				bedSubElement->GetChannelCount(numberChannels);

				// Get current sequence vector size, to assist continuous audio sequence check
				vectorSize = static_cast<uint32_t>(frameContinuousAudioElementIDs_.size());

				// Check if each of the linked PCM elements are present, right before the bed element
				//
				// The check is carried out in 3 logic steps.
				// 1) Current vectorSize must be greater or equal to numberChannels, as otherwise
				//    there are not enough PCM elements for the number of bed channels
				// 2) If pass step 1), the ElementID for the "numberChannels" of elements, before this bed, 
				//    must all be kIABElementID_AudioDataPCM, as otherwise the sequence requirement will not be met
				// 3) If pass step 2), the AudioDataIDs for the "numberChannels" of elements, 
				//    before this bed, must be uniquely linked to referred AudioDataID from the channels

				// Step 1)
				if (vectorSize < numberChannels)
				{
					// Not enough PCM elements for all channels in bed. Error.

					// Update issue variable and log for for DbyIMF
					validationIssue_.isBeingValidated_ = kConstraints_set_DbyIMF;
					validationIssue_.event_ = ErrorEvent;
					validationIssue_.errorCode_ = kValidateErrorDolIMFNotMeetingContinuousAudioSequence;

					// Report error. Set flag to avoid repeated reporting.
					isErrorReported = true;

					if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
					{
						return isToContinue;
					}
				}
				else
				{
					// Step 2)
					for (uint16_t i = 0; i < numberChannels; i++)
					{
						// Check ElementIDs
						if (frameContinuousAudioElementIDs_[vectorSize - 1 - i] != kIABElementID_AudioDataPCM)
						{
							// One of the preceding "numberChannels" of sub-elements is not a PCM, error.

							// Update issue variable and log for for DbyIMF
							validationIssue_.isBeingValidated_ = kConstraints_set_DbyIMF;
							validationIssue_.event_ = ErrorEvent;
							validationIssue_.errorCode_ = kValidateErrorDolIMFNotMeetingContinuousAudioSequence;

							// Report error. Set flag to avoid repeated reporting.
							isErrorReported = true;

							if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
							{
								return isToContinue;
							}

							break;					// Once reported, break the for loop
						}
					}

					// Step 3), run this step if error has not been reported so far
					if (isErrorReported == false)
					{
						// Retieve bed channel objects
						std::vector<IABChannel*> bedChannels;
						bedSubElement->GetBedChannels(bedChannels);

						// Work index variable used for locating linked PCM audio data IDs
						std::vector<uint32_t>::iterator foundIter;
						uint32_t rangeStartForCheck = vectorSize - numberChannels;

						// Loop through each of channels and check audi data IDs presence in 
						// the frameContinuousAudioMetaIDAudioIDs_ list
						for (uint16_t i = 0; i < numberChannels; i++)
						{
							// Retrieve referred audio data ID for the ith channel
							bedChannels[i]->GetAudioDataID(audioDataID);

							// Start checking at rangeStartForCheck from the beginning
							foundIter = std::find((frameContinuousAudioMetaIDAudioIDs_.begin() + rangeStartForCheck),
								frameContinuousAudioMetaIDAudioIDs_.end(), audioDataID);

							if (foundIter == frameContinuousAudioMetaIDAudioIDs_.end())
							{
								// Not found the channel-referred audio data ID

								// Update issue variable and log for DbyIMF
								validationIssue_.isBeingValidated_ = kConstraints_set_DbyIMF;
								validationIssue_.event_ = ErrorEvent;
								validationIssue_.errorCode_ = kValidateErrorDolIMFNotMeetingContinuousAudioSequence;

								// Report error. Set flag to avoid repeated reporting.
								isErrorReported = true;

								if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
								{
									return isToContinue;
								}

								break;					// Once reported, break the for loop
							}
						}
					}
				}

				if (isErrorReported == false)
				{
					// No error. Save bed metaID and ElementID to relevant sequence list
					frameContinuousAudioMetaIDAudioIDs_.push_back(metaID);
					frameContinuousAudioElementIDs_.push_back(elementID);
				}

				break;

			case kIABElementID_ObjectDefinition:
				hasObjectAppeared = true;				// Mark the appearance of an object.
														// Any appearance of bed after this violates continuous audio sequence.

				const IABObjectDefinition* objectSubElement;
				objectSubElement = dynamic_cast<const IABObjectDefinition*>(*iterFrameSubElement);

				// Retrieve MetaID
				objectSubElement->GetMetadataID(metaID);

				// Retrieve linked AudioDataID
				objectSubElement->GetAudioDataID(audioDataID);

				// Per continuous audio sequence requirement, the linked PCM audio element must be
				// in frame sub-element list, positioned right before this object element.
				// (The sequence element also implies that referred AudioDataID must not be 0. This case
				// is also covered in code below.)
				//
				vectorSize = static_cast<uint32_t>(frameContinuousAudioElementIDs_.size());

				if (vectorSize == 0)
				{
					// Object is the first in sub-element list. No linked PCM before it. Error.

					// Update issue variable and log for for DbyIMF
					validationIssue_.isBeingValidated_ = kConstraints_set_DbyIMF;
					validationIssue_.event_ = ErrorEvent;
					validationIssue_.errorCode_ = kValidateErrorDolIMFNotMeetingContinuousAudioSequence;

					// Report error. Set flag to avoid repeated reporting.
					isErrorReported = true;

					if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
					{
						return isToContinue;
					}
				}
				else if ((frameContinuousAudioMetaIDAudioIDs_[vectorSize - 1] != audioDataID)
					|| (frameContinuousAudioElementIDs_[vectorSize - 1] != kIABElementID_AudioDataPCM))
				{
					// The preceding element is not a PCM, or its AudioDataID does not match the
					// linked AudioDataID, error.

					// Update issue variable and log for for DbyIMF
					validationIssue_.isBeingValidated_ = kConstraints_set_DbyIMF;
					validationIssue_.event_ = ErrorEvent;
					validationIssue_.errorCode_ = kValidateErrorDolIMFNotMeetingContinuousAudioSequence;

					// Report error. Set flag to avoid repeated reporting.
					isErrorReported = true;

					if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
					{
						return isToContinue;
					}
				}
				else
				{
					// No error. Save each to relevant sequence list
					frameContinuousAudioMetaIDAudioIDs_.push_back(metaID);
					frameContinuousAudioElementIDs_.push_back(elementID);
				}

				break;

			case kIABElementID_AudioDataPCM:
				const IABAudioDataPCM* audioPCMSubElement;
				audioPCMSubElement = dynamic_cast<const IABAudioDataPCM*>(*iterFrameSubElement);

				// Retrieve AudioDataID
				audioPCMSubElement->GetAudioDataID(audioDataID);

				// For audioDataPCM sub-element, simply save them into relevant sequence lists.
				frameContinuousAudioMetaIDAudioIDs_.push_back(audioDataID);
				frameContinuousAudioElementIDs_.push_back(elementID);

				break;

			default:
				// Ignore all other element types for continuous audio sequence check.
				break;
			}

			// Check if early termination has been called
			if (!isToContinue)
			{
				return isToContinue;
			}
		}

		return isToContinue;
	}

	// IABValidator::SaveFrameParameterToGlobal() implementation
	void IABValidator::SaveFrameParameterToGlobal()
	{
		// Global bitstream version for persistence checking
		frameUnderValidation_->GetVersion(globalVersion_);

		// Global sampling rate for persistence checking
		frameUnderValidation_->GetSampleRate(globalSampleRate_);

		// Global bit depth for persistence checking
		frameUnderValidation_->GetBitDepth(globalBitDepth_);

		// Global frame rate for persistence checking
		frameUnderValidation_->GetFrameRate(globalFrameRate_);

		// Global maximum rendered for persistence checking
		frameUnderValidation_->GetMaxRendered(globalMaxRendered_);

		// Global frame sub-element count for persistence checking
		frameUnderValidation_->GetSubElementCount(globalFrameSubElementCount_);
        
        // Global BedDefinition element count for persistence checking
        globalBedDefinitionElementCount_ = frameBedDefinitionElementCount_;

		// Global BedDefinition element MetaID set for persistence checking
		globalBedMetaIDSet_ = frameBedMetaIDSet_;
        
		// Save global beds parameters for persistence checking
		globalBedsPersistenceParameters_ = frameBedsPersistenceParameters_;

		// Save global objects parameters for persistence checking
		globalObjectsPersistenceParameters_ = frameObjectsPersistenceParameters_;

		// Save global continuous audio MetaID/AudioID sequence
		globalContinuousAudioMetaIDAudioIDs_ = frameContinuousAudioMetaIDAudioIDs_;

		// Save global continuous audio ElementID sequence
		globalContinuousAudioElementIDs_ = frameContinuousAudioElementIDs_;

	}

	// IABValidator::ValidateEssenceElementsPresence() implementation
	bool IABValidator::ValidateEssenceElementsPresence()
	{
		bool isToContinue = true;

		std::set<IABAudioDataIDType>::const_iterator iterReferredAudioID;

		size_t foundCount = 0;

		// Check presence of IABAudioDataDLC or IABAudioDataPCM element for each of the
		// "referred-to" audioID from channels and objects
		for (iterReferredAudioID = frameReferredAudioDataIDSet_.begin(); iterReferredAudioID != frameReferredAudioDataIDSet_.end(); iterReferredAudioID++)
		{
			// "AudioID = 0" is ok, as silence. No need to check.
			if (*iterReferredAudioID == 0)
			{
				continue;
			}

			// Is it in frameEssenceAudioDataIDSet_?
			foundCount = frameEssenceAudioDataIDSet_.count(*iterReferredAudioID);

			// If not uniquely found/located, error
			if (foundCount != 1)
			{
				validationIssue_.id_ = *iterReferredAudioID;

				// Update issue variable and log for Cinema ST2098_2_2018
				validationIssue_.isBeingValidated_ = kConstraints_set_Cinema_ST2098_2_2018;
				validationIssue_.event_ = ErrorEvent;
				validationIssue_.errorCode_ = kValidateErrorMissingAudioDataEssenceElement;

				if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
				{
					return isToContinue;
				}

				// Update issue variable and log for IMF ST2098_2_2019
				validationIssue_.isBeingValidated_ = kConstraints_set_IMF_ST2098_2_2019;
				validationIssue_.event_ = ErrorEvent;
				validationIssue_.errorCode_ = kValidateErrorMissingAudioDataEssenceElement;

				if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
				{
					return isToContinue;
				}
			}
		}

		return isToContinue;
	}

	// IABValidator::CheckAndLogUnusedDLCElements() implementation
	bool IABValidator::CheckAndLogUnusedDLCElements()
	{
		bool isToContinue = true;

		std::set<IABAudioDataIDType>::const_iterator iterDLCAudioID;

		size_t foundCount = 0;

		// Check if each of the DLC audio data IDs are actually used by bed channels or objects
		for (iterDLCAudioID = frameDLCEssenceAudioDataIDSet_.begin(); iterDLCAudioID != frameDLCEssenceAudioDataIDSet_.end(); iterDLCAudioID++)
		{
			// Is it in frameReferredAudioDataIDSet_?
			foundCount = frameReferredAudioDataIDSet_.count(*iterDLCAudioID);

			// If not uniquely found/located, warning on unused DLC element
			if (foundCount != 1)
			{
				validationIssue_.id_ = *iterDLCAudioID;

				// Update issue variable and log for Cinema ST2098_2_2018
				validationIssue_.isBeingValidated_ = kConstraints_set_Cinema_ST2098_2_2018;
				validationIssue_.event_ = WarningEvent;
				validationIssue_.errorCode_ = kValidateWarningUnreferencedAudioDataDLCElement;

				if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
				{
					return isToContinue;
				}

				// Update issue variable and log for IMF ST2098_2_2019
				validationIssue_.isBeingValidated_ = kConstraints_set_IMF_ST2098_2_2019;
				validationIssue_.event_ = WarningEvent;
				validationIssue_.errorCode_ = kValidateWarningUnreferencedAudioDataDLCElement;

				if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
				{
					return isToContinue;
				}
			}
		}

		return isToContinue;
	}

	// IABValidator::CheckAndLogUnusedPCMElements() implementation
	bool IABValidator::CheckAndLogUnusedPCMElements()
	{
		bool isToContinue = true;

		std::set<IABAudioDataIDType>::const_iterator iterPCMAudioID;

		size_t foundCount = 0;

		// Check if each of the PCM audio data IDs are actually used by bed channels or objects
		for (iterPCMAudioID = framePCMEssenceAudioDataIDSet_.begin(); iterPCMAudioID != framePCMEssenceAudioDataIDSet_.end(); iterPCMAudioID++)
		{
			// Is it in frameReferredAudioDataIDSet_?
			foundCount = frameReferredAudioDataIDSet_.count(*iterPCMAudioID);

			// If not uniquely found/located, warning on unused PCM element
			if (foundCount != 1)
			{
				validationIssue_.id_ = *iterPCMAudioID;

				// Update issue variable and log for Cinema ST2098_2_2018
				validationIssue_.isBeingValidated_ = kConstraints_set_Cinema_ST2098_2_2018;
				validationIssue_.event_ = WarningEvent;
				validationIssue_.errorCode_ = kValidateWarningUnreferencedAudioDataPCMElement;

				if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
				{
					return isToContinue;
				}

				// Update issue variable and log for IMF ST2098_2_2019
				validationIssue_.isBeingValidated_ = kConstraints_set_IMF_ST2098_2_2019;
				validationIssue_.event_ = WarningEvent;
				validationIssue_.errorCode_ = kValidateWarningUnreferencedAudioDataPCMElement;

				if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
				{
					return isToContinue;
				}
			}
		}

		return isToContinue;
	}

	// IABValidator::ValidateFrameParametersPersistence() implementation
	bool IABValidator::ValidateFrameParametersPersistence()
	{
		IABVersionNumberType version;
		IABSampleRateType sampleRate;
		IABBitDepthType bitDepth;
		IABFrameRateType frameRate;

		bool isToContinue = true;
        
        // Set validationIssue_.id_ to reflect Frame level
        validationIssue_.id_ = kIssueID_IAFrame;

		frameUnderValidation_->GetVersion(version);
		frameUnderValidation_->GetSampleRate(sampleRate);
		frameUnderValidation_->GetBitDepth(bitDepth);
		frameUnderValidation_->GetFrameRate(frameRate);

		if (version != globalVersion_)
		{
			// Update issue variable and log for Cinema ST2098_2_2018
            validationIssue_.isBeingValidated_ = kConstraints_set_Cinema_ST2098_2_2018;
			validationIssue_.event_ = ErrorEvent;
			validationIssue_.errorCode_ = kValidateErrorIAFrameBitstreamVersionNotPersistent;

			if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
			{
				return isToContinue;
			}
            
            // Update issue variable and log for IMF ST2098_2_2019
            validationIssue_.isBeingValidated_ = kConstraints_set_IMF_ST2098_2_2019;
            validationIssue_.event_ = ErrorEvent;
            validationIssue_.errorCode_ = kValidateErrorIAFrameBitstreamVersionNotPersistent;
            
            if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
            {
                return isToContinue;
            }

		}

		if (sampleRate != globalSampleRate_)
		{
			// Update issue variable and log for Cinema ST2098_2_2018
            validationIssue_.isBeingValidated_ = kConstraints_set_Cinema_ST2098_2_2018;
			validationIssue_.event_ = ErrorEvent;
			validationIssue_.errorCode_ = kValidateErrorIAFrameSampleRateNotPersistent;

			if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
			{
				return isToContinue;
			}
            
            // Update issue variable and log for IMF ST2098_2_2019
            validationIssue_.isBeingValidated_ = kConstraints_set_IMF_ST2098_2_2019;
            validationIssue_.event_ = ErrorEvent;
            validationIssue_.errorCode_ = kValidateErrorIAFrameSampleRateNotPersistent;
            
            if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
            {
                return isToContinue;
            }
		}

		if (bitDepth != globalBitDepth_)
		{
			// Update issue variable and log for Cinema ST2098_2_2018
            validationIssue_.isBeingValidated_ = kConstraints_set_Cinema_ST2098_2_2018;
			validationIssue_.event_ = ErrorEvent;
			validationIssue_.errorCode_ = kValidateErrorIAFrameBitDepthNotPersistent;

			if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
			{
				return isToContinue;
			}
            
            // Update issue variable and log for IMF ST2098_2_2019
            validationIssue_.isBeingValidated_ = kConstraints_set_IMF_ST2098_2_2019;
            validationIssue_.event_ = ErrorEvent;
            validationIssue_.errorCode_ = kValidateErrorIAFrameBitDepthNotPersistent;
            
            if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
            {
                return isToContinue;
            }
		}

		if (frameRate != globalFrameRate_)
		{
			// Update issue variable and log for Cinema ST2098_2_2018
            validationIssue_.isBeingValidated_ = kConstraints_set_Cinema_ST2098_2_2018;
			validationIssue_.event_ = ErrorEvent;
			validationIssue_.errorCode_ = kValidateErrorIAFrameFrameRateNotPersistent;

			if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
			{
				return isToContinue;
			}
            
            // Update issue variable and log for IMF ST2098_2_2019
            validationIssue_.isBeingValidated_ = kConstraints_set_IMF_ST2098_2_2019;
            validationIssue_.event_ = ErrorEvent;
            validationIssue_.errorCode_ = kValidateErrorIAFrameFrameRateNotPersistent;
            
            if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
            {
                return isToContinue;
            }
		}

		return isToContinue;
	}

    // IABValidator::ValidateFrameSubElementPersistence() implementation
    bool IABValidator::ValidateFrameSubElementPersistence()
    {
        bool isToContinue = true;

		std::map<IABMetadataIDType, PersistenceParameters>::iterator iterFrameBed;
		std::map<IABMetadataIDType, PersistenceParameters>::iterator iterGlobalBed;

        // #################################################
        // Cinema constraint sets validation starts
        // #################################################

		if ((globalBedsPersistenceParameters_.size() == frameBedsPersistenceParameters_.size())
			&& (frameBedsPersistenceParameters_.size() <= 1) )
		{
			if (frameBedsPersistenceParameters_.size() == 1)
			{
				validationIssue_.id_ = (frameBedsPersistenceParameters_.begin())->first;
			
				if ((globalBedsPersistenceParameters_.begin())->first 
					!= (frameBedsPersistenceParameters_.begin())->first)
                {
                    // Meta ID changed
                    // Update issue variable and log
                    validationIssue_.isBeingValidated_ = kConstraints_set_DbyCinema;
                    validationIssue_.event_ = ErrorEvent;
                    validationIssue_.errorCode_ = kValidateErrorDolCinBedDefinitionMetaIDNotPersistent;
                    if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
                    {
                        return isToContinue;
                    }                    
                }
				else if ((globalBedsPersistenceParameters_.begin())->second.channelIDList_
					!= (frameBedsPersistenceParameters_.begin())->second.channelIDList_)
				{
					// Bed composition changed
					// Update issue variable and log
					validationIssue_.isBeingValidated_ = kConstraints_set_DbyCinema;
					validationIssue_.event_ = ErrorEvent;
					validationIssue_.errorCode_ = kValidateErrorDolCinBedDefinitionChannelListNotPersistent;
					if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
					{
						return isToContinue;
					}
				}
			}
		}
		else
		{
			// Update issue variable and log
            validationIssue_.id_ = (frameBedsPersistenceParameters_.begin())->first;
			validationIssue_.isBeingValidated_ = kConstraints_set_DbyCinema;
			validationIssue_.event_ = ErrorEvent;
			validationIssue_.errorCode_ = kValidateErrorDolCinBedDefinitionCountNotPersistent;
			if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
			{
				return isToContinue;
			}
		}

        // #################################################
        // Cinema constraint sets validation ends
        // #################################################

        // #################################################
        // IMF constraint sets validation starts
        // #################################################

        // Check BedDefinition persistence
        if (globalBedDefinitionElementCount_ != frameBedDefinitionElementCount_)
        {
            // Number of BedDefinition elements changed
            // Update issue variable and log
            validationIssue_.id_ = kIssueID_IAFrame;
            validationIssue_.isBeingValidated_ = kConstraints_set_IMF_ST2067_201_2019;
            validationIssue_.event_ = ErrorEvent;
            validationIssue_.errorCode_ = kValidateErrorBedDefinitionCountNotPersistent;
            if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
            {
                return isToContinue;
            }
        }
		else if (globalBedMetaIDSet_ != frameBedMetaIDSet_)
		{
			// Bed with the same meta ID not found. frameBedMetaIDSet_ may contain IDs
			// from sub-element beds. Sub-element beds are included in comparison intentionally.
			// Update issue variable and log
			validationIssue_.isBeingValidated_ = kConstraints_set_IMF_ST2067_201_2019;
			validationIssue_.event_ = ErrorEvent;
			validationIssue_.errorCode_ = kValidateErrorBedDefinitionMetaIDNotPersistent;
			if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
			{
				return isToContinue;
			}
		}
		else
        {
            // Number of BedDefinition elements and ID set are both equal.
			// Check bed element parameter persistence
			//
			for (iterFrameBed = frameBedsPersistenceParameters_.begin(); iterFrameBed 
				!= frameBedsPersistenceParameters_.end(); iterFrameBed++)
			{
				validationIssue_.id_ = iterFrameBed->first;

				// Find corresponding key in the global map
				iterGlobalBed = globalBedsPersistenceParameters_.find(iterFrameBed->first);

				if (iterFrameBed->second.conditional_ != iterGlobalBed->second.conditional_)
				{
					// conditional flags are not persistent, update issue variable and log
					validationIssue_.isBeingValidated_ = kConstraints_set_IMF_ST2067_201_2019;
					validationIssue_.event_ = ErrorEvent;
					validationIssue_.errorCode_ = kValidateErrorBedDefinitionConditionalStateNotPersistent;
					if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
					{
						return isToContinue;
					}
				}

				if (iterFrameBed->second.channelIDList_.size() != iterGlobalBed->second.channelIDList_.size())
				{
					// Bed channel count is not persistent
					// Update issue variable and log
					validationIssue_.isBeingValidated_ = kConstraints_set_IMF_ST2067_201_2019;
					validationIssue_.event_ = ErrorEvent;
					validationIssue_.errorCode_ = kValidateErrorBedDefinitionChannelCountNotPersistent;
					if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
					{
						return isToContinue;
					}
				}
				else if (iterFrameBed->second.channelIDList_ != iterGlobalBed->second.channelIDList_)
				{
					// Check use case, only when beds conditional flags are also the same
					// Channel ID compositions are not persistent, update issue variable and log
					validationIssue_.isBeingValidated_ = kConstraints_set_IMF_ST2067_201_2019;
					validationIssue_.event_ = ErrorEvent;
					validationIssue_.errorCode_ = kValidateErrorBedDefinitionChannelIDsNotPersistent;
					if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
					{
						return isToContinue;
					}
				}
			}
        }

		std::map<IABMetadataIDType, PersistenceParameters>::iterator iterFrameObject;
		std::map<IABMetadataIDType, PersistenceParameters>::iterator iterGlobalObject;

		// Check ObjectDefinition persistence
		for (iterFrameObject = frameObjectsPersistenceParameters_.begin(); iterFrameObject != frameObjectsPersistenceParameters_.end(); iterFrameObject++)
		{
			validationIssue_.id_ = iterFrameObject->first;

			// Find corresponding key in the global map
			iterGlobalObject = globalObjectsPersistenceParameters_.find(iterFrameObject->first);

			if (iterGlobalObject == globalObjectsPersistenceParameters_.end())
			{
				// Not in global, continue loop
				continue;
			}

			if (iterFrameObject->second.conditional_ != iterGlobalObject->second.conditional_)
			{
				// conditional flags are not persistent, update issue variable and log
				validationIssue_.isBeingValidated_ = kConstraints_set_IMF_ST2067_201_2019;
				validationIssue_.event_ = ErrorEvent;
				validationIssue_.errorCode_ = kValidateErrorObjectDefinitionConditionalStateNotPersistent;
				if (isToContinue = validatorHandler_.Handle(validationIssue_), isToContinue == false)
				{
					return isToContinue;
				}
			}
		}

        return isToContinue;
    }
    
} // namespace ImmersiveAudioBitstream
} // namespace SMPTE
