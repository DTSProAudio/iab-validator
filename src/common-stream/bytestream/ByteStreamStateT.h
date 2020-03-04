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

#ifndef COMMON_STREAM_SRC_BYTESTREAM_BYTESTREAMSTATET_H
#define COMMON_STREAM_SRC_BYTESTREAM_BYTESTREAMSTATET_H

#include <climits>
#include <fstream>

#include "Namespace.h"
#include "StreamDefines.h"

namespace CommonStream
{

    template<typename StreamType>
    class ByteStreamStateT
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

        ByteCount_t byteCount() const
        {
            return byteCount_;
        }

        ByteCount_t byteCount(ByteCount_t bc)
        {
            return byteCount_ = bc;
        }

        ReturnCode error() const
        {
            return error_;
        }

        ReturnCode error(ReturnCode er)
        {
            return error_ = er;
        }

        ByteCount_t buffer_size()
        {
            return bufferLengthInBytes_ / CHAR_BIT;
        }

        ByteCount_t bufferLengthInBytes() const
        {
            return bufferLengthInBytes_;
        }

        ByteCount_t bufferLengthInBytes(ByteCount_t bl)
        {
            return (bufferLengthInBytes_ = bl);
        }

        ByteCount_t bytesLeft()
        {
            return bufferLengthInBytes_ - byteCount_;
        }

    protected:
        std::ios_base::iostate      streamState_;
        std::ios_base::streampos    streamPosition_;

        ByteCount_t                 byteCount_;
        ByteCount_t                 bufferLengthInBytes_;
        ReturnCode                  error_;
    };

} // namespace CommonStream

#endif // COMMON_STREAM_SRC_BYTESTREAM_BYTESTREAMSTATET_H
