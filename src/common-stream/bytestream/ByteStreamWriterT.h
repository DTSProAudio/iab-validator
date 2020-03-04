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

#ifndef COMMON_STREAM_SRC_BYTESTREAM_BYTESTREAMWRITERT_H_
#define COMMON_STREAM_SRC_BYTESTREAM_BYTESTREAMWRITERT_H_

#include <exception>
#include <sstream>
#include <istream>
#include <iostream>
#include <climits>
#include <cstdio>

#include "Namespace.h"
#include "ByteStreamStateT.h"
#include "ByteSwap.h"

#ifdef CMNSTRM_USE_CHECK_SUM
#include "HashGenerator.h"
#include "CRCGenerator.h"
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

namespace CommonStream
{

    template<typename StreamType>
    class ByteStreamWriterT : public ByteStreamStateT<StreamType>
    {

        using ByteStreamStateT<StreamType>::error_;
        using ByteStreamStateT<StreamType>::byteCount_;
        using ByteStreamStateT<StreamType>::bufferLengthInBytes_;
    
    public:

        ByteStreamWriterT();

        ByteStreamWriterT(const ByteStreamWriterT<StreamType> &bs2); ///< copy constructor
        ByteStreamWriterT(char *iBuffer, ByteCount_t iBufferLength);
        ByteStreamWriterT(unsigned char *iBuffer, ByteCount_t iBufferLength);
        ByteStreamWriterT(std::ofstream &iStream);
        ByteStreamWriterT(std::ostream &iStream);

        ~ByteStreamWriterT();

        ByteStreamWriterT &operator=(const ByteStreamWriterT<StreamType> &bs2);

        ReturnCode Init();
        ReturnCode Init(char *iBuffer, ByteCount_t iBufferLength);
        ReturnCode Init(unsigned char *iBuffer, ByteCount_t iBufferLength);
        ReturnCode Init(const ByteStreamWriterT<StreamType> &bs2);

        std::ios_base::streampos streamPosition() const
        {
            return stream_->tellp();
        }
        
        std::ios_base::streampos streamPosition(std::ios_base::streampos sp)
        {
            return stream_->seekp(sp);
        }

        ReturnCode write(bool iVal);

        ReturnCode write(uint8_t iVal);
        ReturnCode write(uint16_t iVal);
        ReturnCode write(uint32_t iVal);
        ReturnCode write(uint64_t iVal);

        ReturnCode write(char iVal);

        ReturnCode write(int8_t iVal);
        ReturnCode write(int16_t iVal);
        ReturnCode write(int32_t iVal);
        ReturnCode write(int64_t iVal);

        ReturnCode write(const uint8_t *iVal, uint32_t iNumBytes);

        ByteCount_t getByteCount(void);

        ReturnCode seek(uint64_t iPosition);

        ReturnCode ShrinkBuffer(ByteCount_t new_buffer_size);
        ReturnCode IncrementByteCount(ByteCount_t incr);
        ReturnCode skip(ByteCount_t bytes_to_skip);

        char *buffer() const;

        void clear(void);
        bool good(void);
        bool eof(void);
        bool fail(void);

        operator bool ()
        {
            return good();
        }

        ReturnCode status(void);

        ByteStreamStateT<StreamType> reset(void);

        // utilities, debugging
        void dump_buffer_ascii(FILE *fp);

#ifdef CMNSTRM_USE_CHECK_SUM
        bool                setHashGenerator(HashGenerator *iGenerator);
        HashGenerator*      getHashGenerator(void);

        bool                setCRCGenerator(CRCGenerator *iGenerator);
        CRCGenerator*       getCRCGenerator(void);
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

    private:

        ByteStreamStateT<StreamType> getState(void) const;
        ByteStreamStateT<StreamType> setState(ByteStreamStateT<StreamType> state);
        
        StreamType          *stream_;

#ifdef CMNSTRM_USE_CHECK_SUM
        CRCGenerator		*CRC_;
        HashGenerator		*hash_;
#endif // #ifdef CMNSTRM_USE_CHECK_SUM
    };

    template<typename StreamType>
    ByteStreamWriterT<StreamType>::ByteStreamWriterT()
        :
        stream_(nullptr)
#ifdef CMNSTRM_USE_CHECK_SUM
        , hash_(nullptr)
        , CRC_(nullptr)
#endif // #ifdef CMNSTRM_USE_CHECK_SUM
    {
        Init();
    }

    template<typename StreamType>
    ByteStreamWriterT<StreamType>::ByteStreamWriterT(char *iBuffer, ByteCount_t iBufferLength)
        :
        stream_(nullptr)
#ifdef CMNSTRM_USE_CHECK_SUM
        , hash_(nullptr)
        , CRC_(nullptr)
#endif // #ifdef CMNSTRM_USE_CHECK_SUM
    {
        ReturnCode rc = Init(iBuffer, iBufferLength);
        if ( rc != OK )
        {
            // throw something appropriate
        }
    }

    template<typename StreamType>
    ByteStreamWriterT<StreamType>::ByteStreamWriterT(const ByteStreamWriterT<StreamType> &bs2)
        :
        stream_(nullptr)
#ifdef CMNSTRM_USE_CHECK_SUM
        , hash_(nullptr)
        , CRC_(nullptr)
#endif // #ifdef CMNSTRM_USE_CHECK_SUM
    {
        ReturnCode rc = Init(bs2);
        if ( rc != OK )
        {
            // throw something appropriate
        }
    }

    template<typename StreamType>
    ByteStreamWriterT<StreamType>::ByteStreamWriterT(unsigned char *iBuffer, ByteCount_t iBufferLength)
        :
        stream_(nullptr)
#ifdef CMNSTRM_USE_CHECK_SUM
        , hash_(nullptr)
        , CRC_(nullptr)
#endif // #ifdef CMNSTRM_USE_CHECK_SUM
    {
        ReturnCode rc = Init(iBuffer, iBufferLength);
        if ( rc != OK )
        {
            // throw something appropriate
        }
    }

    template<typename StreamType>
    ReturnCode  ByteStreamWriterT<StreamType>::Init()
    {
        byteCount_ = 0;
        bufferLengthInBytes_ = 0;
        if ( stream_ )
        {
            delete stream_;
        }
        stream_ = 0;

#ifdef CMNSTRM_USE_CHECK_SUM
        hash_ = nullptr;
        CRC_ = nullptr;
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

        error_ = OK;
        return OK;
    }

    template<typename StreamType>
    ReturnCode  ByteStreamWriterT<StreamType>::Init(unsigned char *iBuffer, ByteCount_t iBufferLength)
    {
        return Init(reinterpret_cast<char *>(iBuffer), iBufferLength);
    }

    /*
     * Retreiving the number of used bytes
     */
    template<typename StreamType>
    ByteCount_t ByteStreamWriterT<StreamType>::getByteCount(void)
    {
        return byteCount_;
    }

    template<typename StreamType>
    ReturnCode ByteStreamWriterT<StreamType>::seek(uint64_t iPosition)
    {
        if (!good())
        {
            return status();
        }

        stream_->seekp(iPosition);

        return status();
    }

    template<typename StreamType>
    void ByteStreamWriterT<StreamType>::clear()
    {
        error_ = OK;
        stream_->clear();
    }

    /// \brief Shrink the buffer to a given number of bytes
    /// \param new_buffer_size  new size in bytes
    /// \return OK/error code
    template<typename StreamType>
    ReturnCode  ByteStreamWriterT<StreamType>::ShrinkBuffer(ByteCount_t new_buffer_size)
    {

        ByteCount_t tentative_byte_size = CHAR_BIT*new_buffer_size;

        // check that we are not enlarging the buffer, and that we are not cutting
        // already processed bytes
        if ( tentative_byte_size > bufferLengthInBytes_ || tentative_byte_size < byteCount_ )
        {
            return error_ = IO_FAIL;
        }
        bufferLengthInBytes_ = tentative_byte_size;
        return OK;
    }

    template<typename StreamType>
    ReturnCode  ByteStreamWriterT<StreamType>::IncrementByteCount(ByteCount_t incr)
    {
        ByteCount_t tentative = byteCount_ + incr;
        if ( tentative > bufferLengthInBytes_ )
        {
            byteCount_ = bufferLengthInBytes_;
        }
        else
        {
            byteCount_ = tentative;
        }
        return OK;
    }

    /**
     *
     */

    template<typename StreamType>
    bool  ByteStreamWriterT<StreamType>::good()
    {
        if (error_ != OK)
        {
            return  false;
        }

        return stream_->good();
    }

    /**
     *
     */

    template<typename StreamType>
    bool  ByteStreamWriterT<StreamType>::eof()
    {
        if (stream_->eof())
            error_ = IO_EOF;
        return error_ == IO_EOF;
    }

    /**
     *
     */

    template<typename StreamType>
    bool  ByteStreamWriterT<StreamType>::fail()
    {
        if (stream_->fail())
            error_ = IO_FAIL;
        return error_ == IO_FAIL;
    }

    /**
     *  Retrieving the error code from the byte stream state
     */

    template<typename StreamType>
    ReturnCode  ByteStreamWriterT<StreamType>::status(void)
    {
        if (error_ != OK)
        {
            return  error_;
        }

        if (eof())
        {
            return IO_EOF;
        }

        if (fail())
        {
            return IO_FAIL;
        }

        return OK;
    }

    template<typename StreamType>
    ReturnCode ByteStreamWriterT<StreamType>::skip(ByteCount_t iNumbytes)
    {
        if (!good())
        {
            return status();
        }

        uint8_t tmp = 0;

        ByteCount_t bytesLeftToWrite = iNumbytes;

        while (bytesLeftToWrite)
        {
            write(tmp);
            --bytesLeftToWrite;
        }

        return status();
    }

    // Note that this functions may cause stream failures

    template<typename StreamType>
    ByteStreamStateT<StreamType> ByteStreamWriterT<StreamType>::getState(void) const
    {

        ByteStreamStateT<StreamType> state;

        state.streamState(stream_->rdstate());
        state.streamPosition(stream_->tellp());
        state.byteCount(byteCount_);
        state.error(error_);
        state.bufferLengthInBytes(bufferLengthInBytes_);
        return state;
    }

    // Note that this functions may cause stream failures

    template<typename StreamType>
    ByteStreamStateT<StreamType> ByteStreamWriterT<StreamType>::setState(ByteStreamStateT<StreamType> state)
    {
        stream_->clear();
        stream_->setstate(state.streamState());
        stream_->seekp(state.streamPosition());

        byteCount_ = state.byteCount();
        error_ = state.error();
        bufferLengthInBytes_ = state.bufferLengthInBytes();
        return state;
    }

    // Note that this functions may cause stream failures

    template<typename StreamType>
    ByteStreamStateT<StreamType> ByteStreamWriterT<StreamType>::reset(void)
    {
        stream_->clear();
        stream_->seekp(0);

        byteCount_ = 0;

        error_ = OK;

        return getState();
    }

    template<typename StreamType>
    ReturnCode  ByteStreamWriterT<StreamType>::write(bool iVal)
    {
        if (!good())
        {
            return status();
        }

        this->write((uint8_t)iVal);

        return status();
    }

    template<typename StreamType>
    ReturnCode  ByteStreamWriterT<StreamType>::write(uint8_t iVal)
    {
        if (!good())
        {
            return status();
        }

        stream_->write((char*) &iVal, 1);

        if (!good())
        {
            return status();
        }

        byteCount_ += 1;

#ifdef CMNSTRM_USE_CHECK_SUM
        if (hash_ && hash_->useHash())
        {
            hash_->accumulate((uint8_t*)&iVal, 1);
        }
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

        return status();
    }

    template<typename StreamType>
    ReturnCode  ByteStreamWriterT<StreamType>::write(uint16_t iVal)
    {
        if (!good())
        {
            return status();
        }

        BYTESWAP::swapByteOrder(iVal);

        stream_->write((char*) &iVal, 2);

        if (!good())
        {
            return status();
        }

        byteCount_ += 2;

#ifdef CMNSTRM_USE_CHECK_SUM
        if (hash_ && hash_->useHash())
        {
            hash_->accumulate((uint8_t*)&iVal, 2);
        }
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

        return status();
    }

    template<typename StreamType>
    ReturnCode  ByteStreamWriterT<StreamType>::write(uint32_t iVal)
    {
        if (!good())
        {
            return status();
        }

        BYTESWAP::swapByteOrder(iVal);

        stream_->write((char*) &iVal, 4);

        if (!good())
        {
            return status();
        }

        byteCount_ += 4;

#ifdef CMNSTRM_USE_CHECK_SUM
        if (hash_ && hash_->useHash())
        {
            hash_->accumulate((uint8_t*)&iVal, 4);
        }
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

        return status();
    }

    template<typename StreamType>
    ReturnCode  ByteStreamWriterT<StreamType>::write(uint64_t iVal)
    {
        if (!good())
        {
            return status();
        }

        BYTESWAP::swapByteOrder(iVal);

        stream_->write((char*) &iVal, 8);

        if (!good())
        {
            return status();
        }

        byteCount_ += 8;

#ifdef CMNSTRM_USE_CHECK_SUM
        if (hash_ && hash_->useHash())
        {
            hash_->accumulate((uint8_t*)&iVal, 8);
        }
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

        return status();
    }

    template<typename StreamType>
    ReturnCode ByteStreamWriterT<StreamType>::write(char iVal)
    {
        if (!good())
        {
            return status();
        }

        stream_->write(&iVal, 1);

        if (!good())
        {
            return status();
        }

        byteCount_ += 1;

#ifdef CMNSTRM_USE_CHECK_SUM
        if (hash_ && hash_->useHash())
        {
            hash_->accumulate((uint8_t*)&iVal, 1);
        }
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

        return status();
    }

    template<typename StreamType>
    ReturnCode  ByteStreamWriterT<StreamType>::write(int8_t iVal)
    {
        if (!good())
        {
            return status();
        }

        stream_->write((char*) &iVal, 1);

        if (!good())
        {
            return status();
        }

        byteCount_ += 1;

#ifdef CMNSTRM_USE_CHECK_SUM
        if (hash_ && hash_->useHash())
        {
            hash_->accumulate((uint8_t*)&iVal, 1);
        }
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

        return status();
    }

    template<typename StreamType>
    ReturnCode  ByteStreamWriterT<StreamType>::write(int16_t iVal)
    {
        if (!good())
        {
            return status();
        }

        uint16_t tmp = iVal;
        BYTESWAP::swapByteOrder(tmp);

        stream_->write((char*) &tmp, 2);

        if (!good())
        {
            return status();
        }

        byteCount_ += 2;

#ifdef CMNSTRM_USE_CHECK_SUM
        if (hash_ && hash_->useHash())
        {
            hash_->accumulate((uint8_t*)&iVal, 2);
        }
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

        return status();
    }

    template<typename StreamType>
    ReturnCode  ByteStreamWriterT<StreamType>::write(int32_t iVal)
    {
        if (!good())
        {
            return status();
        }

        uint32_t tmp = iVal;
        BYTESWAP::swapByteOrder(tmp);

        stream_->write((char*) &tmp, 4);

        if (!good())
        {
            return status();
        }

        byteCount_ += 4;

#ifdef CMNSTRM_USE_CHECK_SUM
        if (hash_ && hash_->useHash())
        {
            hash_->accumulate((uint8_t*)&iVal, 4);
        }
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

        return status();
    }

    template<typename StreamType>
    ReturnCode  ByteStreamWriterT<StreamType>::write(int64_t iVal)
    {
        if (!good())
        {
            return status();
        }

        uint64_t tmp = iVal;
        BYTESWAP::swapByteOrder(tmp);

        stream_->write((char*) &tmp, 8);

        if (!good())
        {
            return status();
        }

        byteCount_ += 8;

#ifdef CMNSTRM_USE_CHECK_SUM
        if (hash_ && hash_->useHash())
        {
            hash_->accumulate((uint8_t*)&iVal, 8);
        }
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

        return status();
    }

    template<typename StreamType>
    ReturnCode  ByteStreamWriterT<StreamType>::write(const uint8_t *iVal, uint32_t iNumBytes)
    {
        if (!good())
        {
            return status();
        }

        stream_->write((char*) iVal, iNumBytes);

        if (!good())
        {
            return status();
        }

        byteCount_ += iNumBytes;

#ifdef CMNSTRM_USE_CHECK_SUM
        if (hash_ && hash_->useHash())
        {
            hash_->accumulate((uint8_t*)iVal, iNumBytes);
        }
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

        return status();
    }

#ifdef CMNSTRM_USE_CHECK_SUM
    template<typename StreamType>
    bool ByteStreamWriterT<StreamType>::setHashGenerator(HashGenerator *iGenerator)
    {
        if ((hash_ != nullptr) && (iGenerator != nullptr))
        {
            return false;
        }

        hash_ = iGenerator;
        return true;
    }

    template<typename StreamType>
    HashGenerator* ByteStreamWriterT<StreamType>::getHashGenerator(void)
    {
        return hash_;
    }

    template<typename StreamType>
    bool ByteStreamWriterT<StreamType>::setCRCGenerator(CRCGenerator *iGenerator)
    {
        if ((CRC_ != nullptr) && (iGenerator != nullptr))
        {
            return false;
        }

        CRC_ = iGenerator;
        return true;
    }

    template<typename StreamType>
    CRCGenerator* ByteStreamWriterT<StreamType>::getCRCGenerator(void)
    {
        return CRC_;
    }
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

}  // namespace CommonStream

#endif // COMMON_STREAM_SRC_BYTESTREAM_BYTESTREAMWRITERT_H_
