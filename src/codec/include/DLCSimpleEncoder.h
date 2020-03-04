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

#ifndef DLC_SIMPLEENCODER_H
#define DLC_SIMPLEENCODER_H

#include "DLCCodec.h"

namespace dlc
{

    /**
    * Implements a simple DLC encoder that supports only 48 kHz samples, uses no predictors, and uses only PCM residuals.
    * Supports the frame rates specified in ST 2098-2.
    */
    class SimpleEncoder : public Encoder
    {
    public:

		// Constructor
		SimpleEncoder();

		// Destructor
		virtual ~SimpleEncoder();

		enum StatusCode
        {
            StatusCode_OK,
            StatusCode_FATAL_ERROR	/**< Unrecoverable error occurred. */
        };

		// Set up per sample rate & frame rate
		StatusCode setup(SampleRate iSampleRate, FrameRate iFrameRate);

		/**
		* Note that this DLC encoder implementation minimally compress PCM data using a method as
		* "minimum width signed magnitude PCM" as specified in Annex B.11 of ST2098-2 specification.
		*
		* @throws std::invalid_argument if sampleRateCode is not supported
        *                               if sampleCount is not supported
        * @throws std::runtime_error if there is any inconsistency in dlcData
        */
        void encode(const int32_t * iSamples
			, uint32_t iSampleCount
			, SampleRate iSampleRateCode
			, AudioData& oDLCData);

        /**
        * Identical semantics to encode(), but catches all exceptions and returns an error code instead.
        *
        * @return Status following the encoding operation
        */
        StatusCode encode_noexcept(const int32_t * iSamples
			, uint32_t iSampleCount
			, SampleRate iSampleRateCode
			, AudioData& oDLCData);

		/**
		* Low pass filtering 96kHz input to band limit to 0~24kHz. This is called before down sampling
		* 96kHz input to base 48kHz, by 2.
		*
		* Input and output sample both in int32_t form. Unity gain for pass band. Stop band attenuation
		* approximately -70dB. LPF -3dB cutoff freq = ~23kHz.
		*
		* @param[in] iSamples Pointer to a sequence of iSampleCount PCM sample. 
		*            Must not be nullptr.
		* @param[in] iSampleCount Number of samples to encode. Must be larger than 0.
		* @param[in] iSampleRateCode Sample rate of the samples
		* @param[in/out] oSamples pointer to buffer for storing LPF-filtered output samples.
		* @throws std::invalid_argument If iSamples is NULL, or iSampleCount is 0, or iSampleRateCode is not 96k.
		*/
		StatusCode lpfFilter96k(int32_t * iSamples, uint32_t iSampleCount, SampleRate iSampleRateCode, int32_t * oSamples);

	private:

		// Downsampling to 48k integer
		StatusCode downSampling96kTo48k(int32_t * iSamples, uint32_t iSampleCount, int32_t * oSamples);

		// Upsampling back to 96k
		// This is a fixed-point processing to ensure lossless when decoding
		StatusCode upSampling48kTo96k(int32_t * iSamples, int32_t * oSamples);

		// Delay 96k sample by "LowPassFilterCoeffs + upsampling" filter group delay of 80 samples
		StatusCode delayPCM96k(const int32_t * iSamples, uint32_t iSampleCount, int32_t * oSamples);

		// *** Variables

		// Frame size in PCM samples
		uint32_t frameSize_;

		// Number of DLC sub-blocks
		uint8_t numDLCSubBlocks_;

		// DLC sub-blocks size, in PCM samples
		uint32_t subBlockSize_;

		// Delay buffer (interger) for accounting delays from LowPassFilterCoeffs and upsampling
		// 64 + 16 = 80.
		uint32_t delayBuffer96k_[kDLCTotal96kFiltGrpDelay];

		// 96k anti-aliasing filtering
		//

		// Delay line for running filter, with buffer size = (kDLCMaxFrameSize96k + kDLCLPF96kFiltOrder)
		int32_t lpfDelayline_[kDLCMaxFrameSize96k+ kDLCLPF96kFiltOrder];
	};

} // namespace dlc

#endif // #ifndef DLC_SIMPLEENCODER_H
