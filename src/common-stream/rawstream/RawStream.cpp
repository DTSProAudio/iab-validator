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

#include "RawStream.h"
#include <limits>

namespace CommonStream
{
    RawStream::RawStream()
    :
    index_(0),
    buffer_(0),
    buffer_size_(0),
    gcount_(0),
    state_(goodstate_),
    own_storage_(false)
    {
    }

    RawStream::RawStream(const char *buf, BitCount_t buf_size)
    {
        ReturnCode rc = Init(buf, buf_size);
        if ( rc )
        {
            // throw
        }
    }

    RawStream::RawStream(char *buf, BitCount_t buf_size)
    {
        ReturnCode rc = Init(buf, buf_size);
        if ( rc )
        {
            // throw
        }
    }

    RawStream::RawStream(BitCount_t buf_size)
    :
    index_(0),
    buffer_(0),
    buffer_size_(0),
    gcount_(0),
    state_(goodstate_),
    own_storage_(false)
    {
        ReturnCode rc = Init(buf_size);
        if ( rc )
        {
            // throw
        }
    }

    RawStream::RawStream(const RawStream &stream2)
    :
    index_(0),
    buffer_(0),
    buffer_size_(0),
    gcount_(0),
    state_(goodstate_),
    own_storage_(false)
    {
        int rc = Init(stream2);
        if ( rc )
        {
            // should throw
        }
    }

    RawStream::~RawStream()
    {
        Dealloc();
    }

    ReturnCode RawStream::Init()
    {
        buffer_ = 0;
        buffer_size_ = 0;
        state_ = goodstate_;
        index_ = 0;
        gcount_ = 0;
        own_storage_ = false;
        return OK;
    }

    void RawStream::Dealloc()
    {
        if ( own_storage_ )
        {
            delete [] buffer_;
            Init(); // clear everything
        }
    }

    /// \brief Init from a given buffer and buffer size.
    ///
    /// Does not allocate new storage.
    ///
    ReturnCode RawStream::Init(const char *buf, BitCount_t buf_size)
    {
        // ****TODO (gs): EVIL EVIL EVIL EVIL const_cast
        char *ncbuf = const_cast<char *>(buf);
        return Init(ncbuf, buf_size);
    }

    /// \brief Init from a given buffer and buffer size.
    ///
    /// Does not allocate new storage.
    ///
    ReturnCode RawStream::Init(char *buf, BitCount_t buf_size)
    {
        buffer_ = buf;
        buffer_size_ = buf_size;
        state_ = goodstate_;
        index_ = 0;
        gcount_ = 0;
        own_storage_ = false;
        return OK;
    }

    /// \brief Init stream of a given buffer size
    ///
    /// Allocates new storage.
    ///
    ReturnCode RawStream::Init(BitCount_t buf_size)
    {
        Dealloc();
        buffer_size_ = buf_size;
        state_ = goodstate_;
        index_ = 0;
        gcount_ = 0;
        buffer_ = new(std::nothrow) char[buf_size];
        if ( !buffer_ )
        {
            own_storage_ = false;
            state_ |= failbit_;
            return IO_FAIL;
        }
        own_storage_ = true;
        return OK;
    }

    /// \brief Init from another raw stream
    ///
    /// Always allocates new storage
    ///
    ReturnCode RawStream::Init(const RawStream &stream2)
    {
        BitCount_t newsize = stream2.buffer_size_;
        Dealloc();
        ReturnCode rc = Init(newsize);
        if ( rc )
        {
            // should throw
            Init(); // clear everything
            state_ = failbit_; // but set state to fail
            return IO_FAIL;
        }
        own_storage_ = true;
        buffer_size_ = newsize;
        
        if (buffer_size_ > std::numeric_limits<uint32_t>::max())
        {
            return IO_FAIL;
        }
        
        memcpy(buffer_, stream2.buffer_, static_cast<uint32_t>(buffer_size_*sizeof(*buffer_)));
        index_ = stream2.index_;
        gcount_ = stream2.gcount_;
        state_ = stream2.state_;
        return OK;
    }

    /// \brief Init from another raw stream
    ///
    /// Will try to reuse storage, if local buffer (whether
    /// owned or not) is large enough.
    ///
    ReturnCode RawStream::InitReuse(const RawStream &stream2)
    {
        BitCount_t newsize = stream2.buffer_size_;
        if ( buffer_size_ < newsize )
        {
            // cannot reuse
            Dealloc();
            ReturnCode rc = Init(newsize);
            if ( rc )
            {
                // should throw
                Init(); // clear everything
                state_ = failbit_; // but set state to fail
                return IO_FAIL;
            }
            own_storage_ = true;
        }
        buffer_size_ = newsize;
        
        if (buffer_size_ > std::numeric_limits<uint32_t>::max())
        {
            return IO_FAIL;
        }

        memcpy(buffer_, stream2.buffer_, static_cast<uint32_t>(buffer_size_*sizeof(*buffer_)));
        index_ = stream2.index_;
        gcount_ = stream2.gcount_;
        state_ = stream2.state_;
        return OK;
    }

    /// \brief Assignment operator.
    ///
    /// Will try to reuse storage, if local buffer is
    /// large enough (whether owned or not).
    ///
    RawStream & RawStream::operator=(const RawStream &stream2)
    {
        if ( this != &stream2 )
        {
            ReturnCode rc = InitReuse(stream2);
            if ( rc )
            {
                // throw
            }
        }
        return *this;
    }

    std::ios_base::iostate RawStream::rdstate() const
    {
        return state_;
    }

    std::ios_base::iostate RawStream::setstate(std::ios_base::iostate st)
    {
        if ( (st & ~(eofbit_ | badbit_ | failbit_)) != 0  )
        {
            return state_ = badbit_ | failbit_;
        }
        return state_ = st;
    }

    void RawStream::clear()
    {
        setstate(goodstate_);
    }

    bool RawStream::good() const
    {
        return !state_;
    }

    bool RawStream::fail() const
    {
        return ((state_ & failbit_) != 0);
    }

    bool RawStream::eof() const
    {
        return ((state_ & eofbit_) != 0);
    }

    bool RawStream::bad() const
    {
        return (state_ & badbit_);
    }

    BitCount_t RawStream::gcount() const
    {
        return gcount_;
    }

    BitCount_t RawStream::tell() const
    {
        return index_;
    }

    ReturnCode RawStream::seek(BitCount_t pos)
    {
        if ( pos <= buffer_size_ )
        {
            index_ = pos;
            return OK;
        }
        state_ |= failbit_;
        return IO_FAIL;
    }

    ReturnCode RawStream::seek(SBitCount_t offs, std::ios_base::seekdir way)
    {
        SBitCount_t base;
        switch ( way )
        {
        case std::ios_base::beg:
            base = 0;
            break;
        case std::ios_base::cur:
            base = static_cast<SBitCount_t>(index_);
            break;
        case std::ios_base::end:
            base = static_cast<SBitCount_t>(buffer_size_);
            break;
        default:
            state_ |= failbit_;
            return IO_FAIL;
        }
        base += offs;
        if ( base < 0 || base > static_cast<SBitCount_t>(buffer_size_))
        {
            state_ |= failbit_;
            return IO_FAIL;
        }
        index_ = static_cast<BitCount_t>(base);
        return OK;
    }

}   // namespace CommonStream
