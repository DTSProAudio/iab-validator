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
* Header file for the IAB Validator implementation.
*
* @file
*/

#ifndef __IABVALIDATOR_H__
#define	__IABVALIDATOR_H__

#include "IABElements.h"
#include "IABValidatorAPI.h"

namespace SMPTE
{
namespace ImmersiveAudioBitstream
{

	/*****************************************************************************
	* Validator internal data types
	*****************************************************************************/

	/**
	* Indicates the state of validation against a constraint set.
	*/
	struct ValidationState
	{
		ValidationState()
		{
			hasError = false;
			hasWarning = false;
		}

		void reset()
		{
			hasError = false;
			hasWarning = false;
		}

		bool hasError : 1;         /**< Indicates validator has encountered an IAB element item not meeting the constraint set */
		bool hasWarning : 1;       /**< Indicates validator has encountered an IAB element item that requires to issue a warning */
	};

	/*****************************************************************************
	* Validator classes
	*****************************************************************************/

	/**
	*
	* Simple event handler that keeps track of ValidationStates across calls to IABValidatorInterface::ValidateIABFrame
	*
	* @class IABCumulativeEventHandler
	*/
	class IABCumulativeEventHandler : public IABEventHandler
	{
	public:

		IABCumulativeEventHandler();
		virtual ~IABCumulativeEventHandler() {};

		bool Handle(const ValidationIssue& iIssue);

		/**
		* Retrieves validation result for CS iConstraintSet.
		*
		* @memberof IABCumulativeEventHandler
		*
		* @returns Validation result accumulated so far
		*/
		const ValidationResult& GetValidationResult(SupportedConstraintsSet iConstraintSet);

		/** Gets a list of validation issues for CS iConstraintSet.
		* The issue list combines those reported under the profile with dependency 
		* hierarchical constraint set(s).
		*
		* @memberof IABCumulativeEventHandler
		*
		* @returns List of validation issues accumulated so far
		*/
		const std::vector<ValidationIssue>& GetValidationIssues(SupportedConstraintsSet iConstraintSet);

		/** Gets a list of validation issues reported under the single set CS iConstraintSet.
		* Unlike GetValidationIssues() above, the issue list does not contain dependency constraint set(s).
		*
		* @memberof IABCumulativeEventHandler
		*
		* @returns List of validation issues accumulated so far
		*/
		const std::vector<ValidationIssue>& GetValidationIssuesSingleSetOnly(SupportedConstraintsSet iConstraintSet);

		/** Clear list of validation issues sofar for CS iConstraintSet.
		*
		* @memberof IABCumulativeEventHandler
		*/
		void ClearIssues(SupportedConstraintsSet iConstraintSet);

		/** Reset handler states and empty all issue lists, for all constraint sets.
		*
		* @memberof IABCumulativeEventHandler
		*/
		void reset();

	private:

		/// Combined/aggregated validation result variables for each of the constraint sets. Client facing.
		ValidationResult result_Cinema_ST2098_2_2018_;
		ValidationResult result_Cinema_ST429_18_2019_;
		ValidationResult result_DbyCinema_;
		ValidationResult result_IMF_ST2098_2_2019_;
		ValidationResult result_IMF_ST2067_201_2019_;
		ValidationResult result_DbyIMF_;

		/// internal validation state variables, tracking element and frame validation in progression. (Internal)
		ValidationState state_Cinema_ST2098_2_2018_;
		ValidationState state_Cinema_ST429_18_2019_;
		ValidationState state_DbyCinema_;
		ValidationState state_IMF_ST2098_2_2019_;
		ValidationState state_IMF_ST2067_201_2019_;
		ValidationState state_DbyIMF_;

		/// Lists of reported issues for each of the constraint sets.
		std::vector<ValidationIssue> issues_Cinema_ST2098_2_2018_;
		std::vector<ValidationIssue> issues_Cinema_ST429_18_2019_;
		std::vector<ValidationIssue> issues_DbyCinema_;
		std::vector<ValidationIssue> issues_IMF_ST2098_2_2019_;
		std::vector<ValidationIssue> issues_IMF_ST2067_201_2019_;
		std::vector<ValidationIssue> issues_DbyIMF_;

		// Combined issue list.
		std::vector<ValidationIssue> issues_Combined_;
	};

    /**
     *
     * IAB Validator class to validate IAB frames.
     *
     */
    class IABValidator : public IABValidatorInterface
    {
    public:

		// Constructor
		IABValidator();

		// Destructor
		~IABValidator();

        /** Validate an IAB frame.
         *
         * @sa IABValidatorInterface
         *
         */
        iabError ValidateIABFrame(const IABFrameInterface *iIABFrame, uint32_t iFrameIndex);
        
        /** Gets validation result for CS iConstraintSet.
         *
         * @sa IABValidatorInterface
         *
         */
		const ValidationResult& GetValidationResult(SupportedConstraintsSet iConstraintSet);
        
        /** Gets a list of validation issues for CS iConstraintSet.
		 * The issue list combines those reported under the profile with dependency
		 * hierarchical constraint set(s).
		 *
         * @sa IABValidatorInterface
         *
         */
		const std::vector<ValidationIssue>& GetValidationIssues(SupportedConstraintsSet iConstraintSet);

		/** Gets a list of validation issues reported under the single set CS iConstraintSet.
		* Unlike GetValidationIssues() above, the issue list does not contain dependency constraint set(s).
		*
		* @sa IABValidatorInterface
		*
		*/
		const std::vector<ValidationIssue>& GetValidationIssuesSingleSetOnly(SupportedConstraintsSet iConstraintSet);

    private:

        /**
         * Structure containing bed or object parameters that are subject to cross-frame persistence 
		 * validation at frame sub-element level, in some constraint sets.
         */
        struct PersistenceParameters
        {
            PersistenceParameters()
            {
                conditional_ = 0;
                useCase_ = kIABUseCase_NoUseCase;
                channelIDList_ .clear();
            }
            
            uint1_t                 conditional_;       /**< Intended for BedDEfinition and ObjectDefinition. Set to 0 for all other element types */
            IABUseCaseType          useCase_;           /**< Intended for use with isConditional_ = 1. Set to kIABUseCase_NoUseCase when not active */
            std::set<int32_t>       channelIDList_;     /**< Intended for BedDefinition use only */
        };

		// Handler instance
		IABCumulativeEventHandler	validatorHandler_;

		// validation issue, use as a work variable for set/change/update to handler
		ValidationIssue				validationIssue_;

		// **************************************************************************
		// Class internal member variables for supporting in-frame cross-element 
		// and cross-frame persistence validations
		//

		// Pointer to IAFrame that is currently being validated.
		// (For easier ref during frame validation. IABValidator does NOT own the frame, and have no manage resposibility.)
		const IABFrame* frameUnderValidation_;

		// isFirstFarme: set to true before 1st frame validation, and false afterwards
		bool isFirstFrame_;

		// Stats variables for current frame
		//
		// Internal mutable work var. No ownership.
		std::vector<IABElement*> frameSubElements_;

		// Internal mutable work var. No ownership.
		std::vector<IABChannel*> bedChannels_;

		// Bed MetaID set for current frame
		std::set<IABMetadataIDType> frameBedMetaIDSet_;

        // BedRemap MetaID set for current frame
        std::set<IABMetadataIDType> frameBedRemapMetaIDSet_;

		// Object MetaID set for current frame
		std::set<IABMetadataIDType> frameObjectMetaIDSet_;

		// AudioDataID set for current frame, collected from both DLC and PCM elements
		std::set<IABAudioDataIDType> frameEssenceAudioDataIDSet_;

		// AudioDataID set for current frame, collected from DLC elements only
		std::set<IABAudioDataIDType> frameDLCEssenceAudioDataIDSet_;

		// AudioDataID set for current frame, collected from PCM elements only
		std::set<IABAudioDataIDType> framePCMEssenceAudioDataIDSet_;

		// Referenced AudioDataID set for current frame, collected from AudioDataIDs used in objects and bed channels
		std::set<IABAudioDataIDType> frameReferredAudioDataIDSet_;

		// Bed channelID composition set, for a single bed. Mutable work var.
		// Note that the set is defined on data type "int32_t", which is a superset of
		// "enum IABChannelIDType" type, so to be able to stored code beyond spec-defined
		// code range. 
		std::set<int32_t> bedChannelIDCompositionSet_;

		// BedDefinition element count for current frame
		int32_t frameBedDefinitionElementCount_;

		// ObjectDefinition element count for current frame
		int32_t frameObjectDefinitionElementCount_;

		// AudioDataDLC element count for current frame
		int32_t frameAudioDataDLCElementCount_;

		// AudioDataPCM element count for current frame
		int32_t frameAudioDataPCMElementCount_;

		// AuthoringToolInfo element count for current frame
		int32_t frameAuthoringToolInfoElementCount_;

		// UserData element count for current frame
		int32_t frameUserDataElementCount_;

		// Undefined/unrecognised element count for current frame
		int32_t frameUndefinedElementCount_;
        
		// BedDefinition meta ID, PersistenceParameters map for frame being validated
		std::map<IABMetadataIDType, PersistenceParameters> frameBedsPersistenceParameters_;

		// ObjectDefinition meta ID, PersistenceParameters map for frame being validated
		std::map<IABMetadataIDType, PersistenceParameters> frameObjectsPersistenceParameters_;

		// To store a sequence of MetaIDs or AudioIDs from 3 types of frame sub-elements only, 
		// bed, object or PCM. This is used to check continuous audio sequence in DbyIMF.
		// To work in sync with frameContinuousAudioElementIDs_ below.
		std::vector<uint32_t> frameContinuousAudioMetaIDAudioIDs_;

		// To store a sequence of ElementIDs from 3 types of frame sub-elements only, 
		// bed, object or PCM. This is used to check continuous audio sequence in DbyIMF.
		// To work in sync with frameContinuousAudioMetaIDAudioIDs_ above.
		std::vector<IABElementIDType> frameContinuousAudioElementIDs_;

		// Stats variables for global persistence checking
		//

		// Global bitstream version for persistence checking
		IABVersionNumberType globalVersion_;

		// Global sampling rate for persistence checking
        IABSampleRateType globalSampleRate_;

		// Global bit depth for persistence checking
        IABBitDepthType globalBitDepth_;

		// Global frame rate for persistence checking
		IABFrameRateType globalFrameRate_;

		// Global maximum rendered for persistence checking
        IABMaxRenderedRangeType globalMaxRendered_;

		// Global frame sub-element count for persistence checking
        IABElementCountType globalFrameSubElementCount_;
        
        // Global BedDefinition element count for persistence checking
        int32_t globalBedDefinitionElementCount_;
        
		// Global Bed MetaID set for persistence checking
		std::set<IABMetadataIDType> globalBedMetaIDSet_;

		// Global BedDefinition meta ID, PersistenceParameters map for persistence validation
		std::map<IABMetadataIDType, PersistenceParameters> globalBedsPersistenceParameters_;

		// Global ObjectDefinition meta ID, PersistenceParameters map for persistence validation
		std::map<IABMetadataIDType, PersistenceParameters> globalObjectsPersistenceParameters_;

		// Global variable for a sequence of MetaIDs or AudioIDs from 3 types of frame sub-elements, 
		// bed, object or PCM. For persistence validation.
		std::vector<uint32_t> globalContinuousAudioMetaIDAudioIDs_;

		//Golbal variable for a sequence of ElementIDs from 3 types of frame sub-elements only, 
		// bed, object or PCM. For persistence validation.
		std::vector<IABElementIDType> globalContinuousAudioElementIDs_;

		// **************************************************************************
		// Class internal member functions for supporting in-frame cross-element 
		// and cross-frame persistence validations
		//

		// Collect and check stats from BedDefinition for cross-element (& cross-frame) validation
		// (Note: Validation on meta & channel ID duplication is also carried out during collection)
		bool CollectAndCheckBedDefinitionStats(const IABBedDefinition *iBedDefinition);

		// Collect and check stats from ObjectDefinition for cross-element (& cross-frame) validation
		// (Note: Validation on meta ID duplication is also carried out during collection)
		bool CollectAndCheckObjectDefinitionStats(const IABObjectDefinition *iObjectDefinition);

		// Collect and check stats from AudioDataDLC for cross-element (& cross-frame) validation
		// (Note: Validation on audio ID duplication is also carried out during collection)
		bool CollectAndCheckAudioDataDLCStats(const IABAudioDataDLC *iAudioDataDLC);

		// Collect and check stats from AudioDataPCM for cross-element (& cross-frame) validation
		// (Note: Validation on audio ID duplication is also carried out during collection)
		bool CollectAndCheckAudioDataPCMStats(const IABAudioDataPCM *iAudioDataPCM);

		// Collect and check MetaID/AudioID sequence and ElementID sequence.
		// iSubElements must be frame sub-element list (not object/bed sub-element)
		bool CollectAndCheckFrameContinuousAudioSequences(const std::vector<IABElement*> &iSubElements);

		// Save program/frame parameter values as global, for persistence validation
		void SaveFrameParameterToGlobal();

		// Validate presence of referred-to essence elements
		bool ValidateEssenceElementsPresence();

		// Check and log all occurrence of unused/unreferenced DLC elements
		bool CheckAndLogUnusedDLCElements();

		// Check and log all occurrence of unused/unreferenced PCM elements
		bool CheckAndLogUnusedPCMElements();

		// Validate program/frame parameter values persistence
		bool ValidateFrameParametersPersistence();

        // Validate frame sub-element persistence
        bool ValidateFrameSubElementPersistence();
	};

} // namespace ImmersiveAudioBitstream
} // namespace SMPTE

#endif // __IABVALIDATOR_H__
