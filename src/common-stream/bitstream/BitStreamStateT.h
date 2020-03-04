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

#ifndef COMMON_STREAM_SRC_BITSTREAM_BITSTREAMSTATET_H_
#define COMMON_STREAM_SRC_BITSTREAM_BITSTREAMSTATET_H_

#include <climits>
#include <fstream>

#include "Namespace.h"
#include "StreamDefines.h"

namespace CommonStream
{

    template<typename StreamType>
    class BitStreamStateT
    {
    public:
        std::ios_base::iostate streamState() const
        {
            return streamState_;
        }

        std::ios_base::iostate streamState(std::ios_base::iostate st)
        {
            return streamState_ = st;
        }

        std::ios_base::streampos streamPosition() const
        {
            return streamPosition_;
        }
        
        std::ios_base::streampos streamPosition(std::ios_base::streampos sp)
        {
            return streamPosition_ = sp;
        }
        
        int32_t bitBufferFill() const
        {
            return bitBufferFill_;
        }

        int32_t bitBufferFill(int32_t bbf)
        {
            return bitBufferFill_ = bbf;
        }

        uint8_t bitBuffer() const
        {
            return bitBuffer_;
        }

        uint8_t bitBuffer(uint8_t bb)
        {
            return bitBuffer_ = bb;
        }

        BitCount_t  bitCount() const
        {
            return bitCount_;
        }

        BitCount_t  bitCount(BitCount_t bc)
        {
            return bitCount_ = bc;
        }

        ReturnCode error() const
        {
            return error_;
        }

        ReturnCode error(ReturnCode er)
        {
            return error_ = er;
        }

        BitCount_t buffer_size()
        {
            return bufferLengthInBits_/CHAR_BIT;
        }

        BitCount_t bufferLengthInBits() const
        {
            return bufferLengthInBits_;
        }

        BitCount_t bufferLengthInBits(BitCount_t bl)
        {
            return (bufferLengthInBits_ = bl);
        }

        BitCount_t bitsLeft()
        {
            return bufferLengthInBits_ - bitCount_;
        }

    protected:
        std::ios_base::iostate      streamState_;
        std::ios_base::streampos    streamPosition_;

        int32_t                     bitBufferFill_;
        uint8_t                     bitBuffer_;
        BitCount_t                  bitCount_;
        BitCount_t                  bufferLengthInBits_;
        ReturnCode                  error_;
    };

} // namespace CommonStream

#endif // COMMON_STREAM_SRC_BITSTREAM_BITSTREAMSTATET_H_
