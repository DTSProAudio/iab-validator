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

#include <cstring>

#include "RawIStream.h"

namespace CommonStream
{

    RawIStream::RawIStream():RawStream()
    {
    }

    RawIStream::RawIStream(const char *buf, BitCount_t bufsize)
    :
    RawStream(buf, bufsize)
    {
    }

    RawIStream::RawIStream(BitCount_t bufsize)
    :
    RawStream(bufsize)
    {
    }

    RawIStream::RawIStream(const RawIStream &s2)
    :
    RawStream(s2)
    {
    }

    RawIStream &RawIStream::operator=(const RawIStream &s2)
    {
        RawStream::operator=(s2);
        return *this;
    }

    ReturnCode RawIStream::read(void *buf, BitCount_t nbytes)
    {
        ReturnCode rc = OK;
        if ( index_ + nbytes > buffer_size_ )
        {
            nbytes = buffer_size_ - index_;
            state_ |= eofbit_;
            rc = IO_EOF;
        }
        memcpy(buf, buffer_+index_, static_cast<uint32_t>(nbytes));
        gcount_ = nbytes;
        index_ += nbytes;
        return rc;
    }

    ReturnCode RawIStream::get(char &c)
    {
        if ( index_ < buffer_size_ )
        {
            c = buffer_[index_];
            index_++;
            gcount_ = 1;
            return OK;
        }
        else
        {
            state_ |= eofbit_;
            return IO_EOF;
        }
    }

    BitCount_t RawIStream::tellg() const
    {
        return RawStream::tell();
    }

    ReturnCode RawIStream::seekg(std::ios_base::streampos pos)
    {
        BitCount_t bpos = static_cast<BitCount_t>(pos);
        return RawStream::seek(bpos);
    }

    ReturnCode RawIStream::seekg(std::ios_base::streamoff offs, std::ios_base::seekdir way)
    {
        SBitCount_t boffs = static_cast<SBitCount_t>(offs);
        return RawStream::seek(boffs, way);
    }

} // namespace CommonStream
