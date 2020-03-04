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

#include "ByteStreamReaderT.h"

#include <assert.h>
#include <iostream>
#include <locale>
#include <cstring>

#if defined(_WIN32) && _MSC_VER >= 1600
#include <codecvt>
#endif

#include "RawIStream.h"

namespace CommonStream
{

    template<>
    ByteStreamReaderT<std::ifstream>::ByteStreamReaderT(std::ifstream &iStream)
    {
        stream_ = &iStream;
        byteCount_ = 0;

#ifdef CMNSTRM_USE_CHECK_SUM
        hash_ = nullptr;
        CRC_ = nullptr;
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

        error_ = OK;
    }

    template<>
    ByteStreamReaderT<std::ifstream>::~ByteStreamReaderT()
    {
    }

    template<>
    ByteStreamReaderT<std::istream>::ByteStreamReaderT(std::istream &iStream)
    {
        stream_ = &iStream;
        byteCount_ = 0;

#ifdef CMNSTRM_USE_CHECK_SUM
        hash_ = nullptr;
        CRC_ = nullptr;
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

        error_ = OK;
    }

    template<>
    ByteStreamReaderT<std::istream>::~ByteStreamReaderT()
    {
    }

    template<>
    ReturnCode ByteStreamReaderT<RawIStream>::Init(const char *iBuffer, ByteCount_t iBufferLength)
    {
        if ( !iBuffer || !iBufferLength )
        {
            return error_ = PARAMS_BAD;
        }
        Init();
        stream_ = new(std::nothrow) RawIStream;
        if ( !stream_ )
        {
            return error_ = IO_FAIL;
        }
        ReturnCode rc = stream_->Init(iBuffer, iBufferLength);
        if ( rc )
        {
            return error_ = rc;
        }
        bufferLengthInBytes_ = static_cast<ByteCount_t>(iBufferLength * CHAR_BIT);

#ifdef CMNSTRM_USE_CHECK_SUM
        hash_ = nullptr;
        CRC_ = nullptr;
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

        return error_ = OK;
    }

    template<>
    ReturnCode ByteStreamReaderT<RawIStream>::Init(const unsigned char *iBuffer, ByteCount_t iBufferLength)
    {
        return Init(reinterpret_cast<const char *>(iBuffer), static_cast<uint32_t>(iBufferLength));
    }

    template<>
    ReturnCode ByteStreamReaderT<RawIStream>::Init(const ByteStreamReaderT<RawIStream> &bs2)
    {
        if ( stream_ )
        {
            delete stream_;
        }
        stream_ = new(std::nothrow) RawIStream;
        if ( !stream_ )
        {
            return error_ = IO_FAIL;
        }
        ReturnCode rc = stream_->Init(*bs2.stream_);
        if ( rc )
        {
            delete stream_;
            stream_ = 0;
            return error_ = IO_FAIL;
        }
        ByteStreamStateT<RawIStream> st = bs2.getState();
        setState(st);

#ifdef CMNSTRM_USE_CHECK_SUM
        hash_ = nullptr;
        CRC_ = nullptr;
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

        return OK;
    }

    template<>
    ByteStreamReaderT<RawIStream> &ByteStreamReaderT<RawIStream>::operator=(const ByteStreamReaderT<RawIStream> &bs2)
    {
        if ( &bs2 != this )
        {
#ifndef __clang_analyzer__
            if ( !bs2.stream_ )
            {
                // this should probably throw an exception
                delete stream_;
            }
            else
            {
                if ( stream_ )
                {
                    *stream_ = *bs2.stream_;
                }
                else
                {
                    stream_ = new RawIStream(*bs2.stream_);
                }
            }
            ByteStreamStateT<RawIStream> state = bs2.getState();
            setState(state);
#endif // #ifndef __clang_analyzer__
        }
        return *this;
    }

    template<>
    ByteStreamReaderT<RawIStream>::~ByteStreamReaderT()
    {
        delete stream_;
    }

    template<>
    char *ByteStreamReaderT<RawIStream>::buffer() const
    {
        if ( stream_ )
        {
            return stream_->buffer();
        }
        return 0;
    }

}  // namespace CommonStream
