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

#include "ByteStreamWriterT.h"

#include <assert.h>
#include <iostream>
#include <locale>
#include <algorithm>
#include <limits>

#if defined(_WIN32) && _MSC_VER >= 1600
#include <codecvt>
#endif

#include "RawOStream.h"

namespace CommonStream
{

    template<>
    ByteStreamWriterT<std::ofstream>::ByteStreamWriterT(std::ofstream &iStream)
    {
        stream_ = &iStream;

        byteCount_ = 0;
        bufferLengthInBytes_ = std::numeric_limits<ByteCount_t>::max();

#ifdef CMNSTRM_USE_CHECK_SUM
        hash_ = nullptr;
        CRC_ = nullptr;
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

        error_ = OK;
    }

    template<>
    ByteStreamWriterT<std::ofstream>::~ByteStreamWriterT()
    {
    }

    template<>
    ByteStreamWriterT<std::ostream>::ByteStreamWriterT(std::ostream &iStream)
    {
        stream_ = &iStream;

        byteCount_ = 0;
        bufferLengthInBytes_ = std::numeric_limits<ByteCount_t>::max();

#ifdef CMNSTRM_USE_CHECK_SUM
        hash_ = nullptr;
        CRC_ = nullptr;
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

        error_ = OK;
    }

    template<>
    ByteStreamWriterT<std::ostream>::~ByteStreamWriterT()
    {
    }

    template<>
    ReturnCode  ByteStreamWriterT<RawOStream>::Init(char *iBuffer, ByteCount_t iBufferLength)
    {
        if ( !iBuffer || !iBufferLength )
        {
            return error_ = PARAMS_BAD;
        }
        Init();

        if ( stream_ )
        {
            delete stream_;
        }

        stream_ = new(std::nothrow) RawOStream;

        if ( !stream_ )
        {
            return error_ = IO_FAIL;
        }

        ReturnCode rc = stream_->Init(iBuffer, iBufferLength);

        if ( rc )
        {
            return error_ = rc;
        }

        bufferLengthInBytes_ = static_cast<ByteCount_t>(iBufferLength*CHAR_BIT);

#ifdef CMNSTRM_USE_CHECK_SUM
        hash_ = nullptr;
        CRC_ = nullptr;
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

        error_ = OK;

        return OK;
    }

    template<>
    ReturnCode ByteStreamWriterT<RawOStream>::Init(const ByteStreamWriterT<RawOStream> &bs2)
    {
        if ( stream_ )
        {
            delete stream_;
        }
        stream_ = new(std::nothrow) RawOStream;
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
        ByteStreamStateT<RawOStream> st = bs2.getState();
        setState(st);

#ifdef CMNSTRM_USE_CHECK_SUM
        hash_ = nullptr;
        CRC_ = nullptr;
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

        return OK;
    }

    template<>
    ByteStreamWriterT<RawOStream> &ByteStreamWriterT<RawOStream>::operator=(const ByteStreamWriterT<RawOStream> &bs2)
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
                    // reuse if possible
                {
                    *stream_ = *bs2.stream_;
                }
                else
                {
                    stream_ = new RawOStream(*bs2.stream_);
                }
            }
            ByteStreamStateT<RawOStream> state = bs2.getState();
            setState(state);
#endif // #ifndef __clang_analyzer__
        }
        return *this;
    }

    template<>
    ByteStreamWriterT<RawOStream>::~ByteStreamWriterT()
    {
        delete stream_;
    }

    template<>
    char *ByteStreamWriterT<RawOStream>::buffer() const
    {
        if ( stream_ )
        {
            return stream_->buffer();
        }
        return 0;
    }

    template<>
    void ByteStreamWriterT<RawOStream>::dump_buffer_ascii(FILE *fp)
    {
        if ( !stream_ )
        {
            return;
        }
        char *buf = stream_->buffer();
        ByteCount_t len = (byteCount()+7)/8;
        for ( ByteCount_t i=0; i < len; i++ )
        {
            if ( i%16 == 0 )
            {
                fprintf(fp,"\n");
            }
            unsigned x = static_cast<unsigned>(buf[i]);
            fprintf(fp,"%02x ",x & 0xFF);
        }
        fprintf(fp, "\n");
        fflush(fp);
    }

}  // namespace CommonStream
