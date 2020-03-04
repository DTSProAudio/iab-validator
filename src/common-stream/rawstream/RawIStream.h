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

#ifndef COMMON_STREAM_SRC_RAWSTREAM_RAWISTREAM_H_
#define COMMON_STREAM_SRC_RAWSTREAM_RAWISTREAM_H_

#include "Namespace.h"
#include "RawStream.h"

namespace CommonStream
{
    class RawIStream : public RawStream
    {
        using RawStream::eofbit_;
        using RawStream::index_;
        using RawStream::buffer_;
        using RawStream::buffer_size_;
        using RawStream::gcount_;
        using RawStream::state_; // eof | fail | bad

    public:
    
        RawIStream();
        RawIStream(const char *buf, BitCount_t bufsize); ///< constructor with outside storage
        RawIStream(BitCount_t buf_size); ///< construct with own storage
        RawIStream(const RawIStream &s2); ///< copy constructor
        RawIStream &operator=(const RawIStream &s2); ///< assignment

        ReturnCode read(void *buf, BitCount_t nbytes);
        ReturnCode get(char &c);
        BitCount_t tellg() const;
        ReturnCode seekg(std::ios_base::streampos pos);
        ReturnCode seekg(std::ios_base::streamoff offs, std::ios_base::seekdir way);
    };

}   // namespace CommonStream

#endif // COMMON_STREAM_SRC_RAWSTREAM_RAWISTREAM_H_

