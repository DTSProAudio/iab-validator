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
* Header file for common IAB utility functions.
*
* @file
*/

#ifndef __IABUTILITIES_H__
#define	__IABUTILITIES_H__

#include <string.h>
#include <math.h>
#include <string>

#include "IABDataTypes.h"
#include "IABElements.h"

/**************************************************************************/
/**
 *
 * Utilities for IAB parameter values that are defined, or preset, 
 * or calculated using methods as specified by IAB specification.
 *
 *****************************************************************************/

namespace SMPTE
{
namespace ImmersiveAudioBitstream
{
     /**
     * Function to return "number of sub blocks".
     *
     * @param[in] iFrameRate Frame rate, input.
     * @returns uint8_t, Number of sub blocks.
     */
    uint8_t GetIABNumSubBlocks(IABFrameRateType iFrameRate);
    
    /**
     * Function to return "number of samples in a frame".
     *
     * @param[in] iFrameRate Frame rate, input.
     * @param[in] iSampleRate Sample rate, input.
     * @returns uint32_t, Number of sample in an IAB frame.
     */
    uint32_t GetIABNumFrameSamples(IABFrameRateType iFrameRate, IABSampleRateType iSampleRate);

	/**
	* Calculate MaxRendered parameter value from a list of frame sub-elements, per ST2098-2 spec
	* definition of MaxRendered.
	*
	* @param[in] iFrameSubElements Frame sub-element list.
	* @param[out] oMaxRendered Calculated MaxRendered value.
	* @returns \link kIABNoError \endlink if no errors. Other values indicate an error.
	*/
	iabError CalculateFrameMaxRendered(const std::vector<IABElement*> iFrameSubElements, IABMaxRenderedRangeType &oMaxRendered);

}
}

#endif // __IABUTILITIES_H__
