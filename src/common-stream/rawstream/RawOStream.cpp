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

#include "RawOStream.h"

namespace CommonStream
{

    RawOStream::RawOStream():RawStream()
    {
    }

    RawOStream::RawOStream(char *buf, BitCount_t bufsize)
    :
    RawStream(buf, bufsize)
    {
    }

    RawOStream::RawOStream(BitCount_t bufsize)
    :
    RawStream(bufsize)
    {
    }

    RawOStream::RawOStream(const RawOStream &s2)
    :
    RawStream(s2)
    {
    }

    RawOStream &RawOStream::operator=(const RawOStream &s2)
    {
        RawStream::operator=(s2);
        return *this;
    }

    ReturnCode RawOStream::write(const void *buf, uint32_t nbytes)
    {
        ReturnCode rc = OK;
        if ( index_ + nbytes > buffer_size_ )
        {
            BitCount_t copySize = buffer_size_ - index_;

            // We are overflowing the ability of memcpy
            //
            nbytes = static_cast<uint32_t>(copySize);

            // Should this be failbit_ instead of eofbit_?
            //
            state_ |= eofbit_;
            rc = IO_EOF;
        }
        memcpy(buffer_+index_, buf, nbytes);
        gcount_ = nbytes;
        index_ += nbytes;
        return rc;
    }

    BitCount_t RawOStream::tellp() const
    {
        return RawStream::tell();
    }

    ReturnCode RawOStream::seekp(std::ios_base::streampos pos)
    {
        BitCount_t bpos = static_cast<BitCount_t>(pos);
        return RawStream::seek(bpos);
    }

    ReturnCode RawOStream::seekp(std::ios_base::streamoff offs, std::ios_base::seekdir way)
    {
        SBitCount_t boffs = static_cast<SBitCount_t>(offs);
        return RawStream::seek(boffs, way);
    }

} // namespace CommonStream
