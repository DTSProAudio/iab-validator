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

#ifndef DLC_FULLDECODER_H
#define DLC_FULLDECODER_H

#include "DLCCodec.h"

namespace dlc
{

    /**
    * Implements the DLC decoder specified in ST 2098-2.
    */
    class FullDecoder : public Decoder
    {
    public:

        enum StatusCode
        {
            StatusCode_OK,
            StatusCode_FATAL_ERROR /**< Unrecoverable error occurred. */
        };

        /**
        * @throws std::invalid_argument if sampleRateCode is not supported
        *                               if sampleCount is not supported
        * @throws std::runtime_error if there is any inconsistency in dlcData
        */
        void decode(int32_t * oSamples
			, uint32_t iSampleCount
			, SampleRate iSampleRateCode
			, const AudioData& iDLCData);

        /**
        * Identical semantics to decode(), but catches all exceptions and returns an error code instead.
        *
        * @return Status following the decoding operation
        */
        StatusCode decode_noexcept(int32_t * oSamples
			, uint32_t iSampleCount
			, SampleRate iSampleRateCode
			, const AudioData& iDLCData);
    };

} // namespace dlc

#endif // #ifndef DLC_FULLDECODER_H
