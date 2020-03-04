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

#ifndef COMMON_STREAM_SRC_RAWSTREAM_RAWOSTREAM_H_
#define COMMON_STREAM_SRC_RAWSTREAM_RAWOSTREAM_H_

#include "Namespace.h"
#include "RawStream.h"

namespace CommonStream
{
    class RawOStream : public RawStream
    {
        using RawStream::eofbit_;
        using RawStream::index_;
        using RawStream::buffer_;
        using RawStream::buffer_size_;
        using RawStream::gcount_;
        using RawStream::state_; // eof | fail | bad

    public:
        
        RawOStream();
        RawOStream(char *buf, BitCount_t bufsize); ///< constructor with outside storage
        RawOStream(BitCount_t buf_size); ///< construct with own storage
        RawOStream(const RawOStream &stream2); ///< copy constructor
        RawOStream &operator=(const RawOStream &s2); ///< assignment

        ReturnCode write(const void *buf, uint32_t nbytes);
        BitCount_t tellp() const;
        ReturnCode seekp(std::ios_base::streampos pos);
        ReturnCode seekp(std::ios_base::streamoff offs, std::ios_base::seekdir way);
    };

}   // namespace CommonStream

#endif // COMMON_STREAM_SRC_RAWSTREAM_RAWOSTREAM_H_
