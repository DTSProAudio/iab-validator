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
* IABValidatorAPI.h
*
*/

#ifndef __IABVALIDATORAPI_H__
#define	__IABVALIDATORAPI_H__

#include <memory>
#include <vector>

#include "IABErrors.h"

namespace SMPTE
{
namespace ImmersiveAudioBitstream
{    
    /**
     *
     * Represent an interface for an IAB Validator. Must be defined by implementation.
     *
     * @class IABValidatorInterface
     */

    class IABValidatorInterface
    {
    public:
        
        /**
         * Creates an IABValidator instance.
         *
         * @memberof IABValidatorInterface
         *
		 * @returns a pointer to IABValidatorInterface instance created
         */
        static IABValidatorInterface* Create();
        
        /**
         * Deletes an IABValidator instance
         *
         * @memberof IABValidatorInterface
         *
         * @param[in] iInstance pointer to the instance of the IABValidatorInterface to be deleted
         */
        static void Delete(IABValidatorInterface* iInstance);

		/// Destructor
        virtual ~IABValidatorInterface() {}

        /**
         *
         * Validate an IAB frame.
         *
         * @memberof IABValidatorInterface
         *
         * @param[in] iIABFrame pointer to the IAFrame to be validated
         * @param[in] iFrameIndex index of the frame being validated.
         * @return \link kIABNoError \endlink if no errors. Other values indicate an error.
         */
        virtual iabError ValidateIABFrame(const IABFrameInterface *iIABFrame, uint32_t iFrameIndex) = 0;
        
        /** Gets the validation result for constraint set iConstraintSet.
         *
         * @memberof IABValidatorInterface
         *
		 * @param[in] iConstraintSet requested constraint set
		 * @return ValidationResult Result of validation against iConstraintSet, after processing IAB 
		 * frames passed in so far.
         */
		virtual const ValidationResult& GetValidationResult(SupportedConstraintsSet iConstraintSet) = 0;

		/** Gets a list of validation issues reported under the single set CS iConstraintSet.
		* The issue list combines those reported under the profile with dependency
		* hierarchical constraint set(s).
		*
		* @memberof IABValidatorInterface
		*
		* @param[in] iConstraintSet requested constraint set
		* @return std::vector<ValidationIssue> List of issues found during validation against iConstraintSet, 
		* after processing IAB frames passed in so far.
		*/
		virtual const std::vector<ValidationIssue>& GetValidationIssues(SupportedConstraintsSet iConstraintSet) = 0;
        
		/** Gets a list of validation issues reported under the single set CS iConstraintSet.
		* Unlike GetValidationIssues(), the issue list does not contain dependency constraint set(s).
		*
		* @memberof IABValidatorInterface
		*
		* @param[in] iConstraintSet requested constraint set
		* @return std::vector<ValidationIssue> List of issues found during validation against iConstraintSet 
		* only, after processing IAB frames passed in so far.
		*/
		virtual const std::vector<ValidationIssue>& GetValidationIssuesSingleSetOnly(SupportedConstraintsSet iConstraintSet) = 0;
	};

	/**
	* Interface for an event handler, which is called by IABEventHandler when encountering a validation event. Must be defined by implementation.
	*
	* @class IABEventHandler
	*/
	class IABEventHandler
	{
	public:

		virtual ~IABEventHandler() {};

		/**
		* Called by the validator every time an event is encountered.
		*
		* @memberof IABEventHandler
		*
		* @param[in] iIssue issue report by the validator
		*
		* @returns if false, the validator should stop processing the frame; otherwise it should continue processing the frame.
		*/
		virtual bool Handle(const ValidationIssue& iIssue) = 0;

	};

} // namespace ImmersiveAudioBitstream
} // namespace SMPTE


#endif // __IABVALIDATORAPI_H__
