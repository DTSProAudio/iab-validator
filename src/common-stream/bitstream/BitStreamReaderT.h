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

#ifndef COMMON_STREAM_SRC_BITSTREAM_BITSTREAMREADERT_H_
#define COMMON_STREAM_SRC_BITSTREAM_BITSTREAMREADERT_H_

#include <string.h>
#include <istream>
#include <climits>

#include "Namespace.h"
#include "BitStreamStateT.h"

#ifdef CMNSTRM_USE_CHECK_SUM
#include "HashGenerator.h"
#include "CRCGenerator.h"
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

namespace CommonStream
{
    template<typename StreamType> class BitStreamReaderT;

    template<typename StreamType>
    class BitStreamStateRestorerT
    {
    public:
        BitStreamStateRestorerT(BitStreamReaderT<StreamType> *iBSR)
        {
            if (iBSR == nullptr)
            {
                return;
            }

            bsr_ = iBSR;

#ifdef CMNSTRM_USE_CHECK_SUM
            if (bsr_->getHashGenerator())
            {
                bsr_->getHashGenerator()->pause();
            }
            if (bsr_->getCRCGenerator())
            {
                bsr_->getCRCGenerator()->pause();
            }
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

            savedState_ = bsr_->getState();
        }

        ~BitStreamStateRestorerT()
        {
            if (bsr_ == nullptr)
            {
                return;
            }

#ifdef CMNSTRM_USE_CHECK_SUM
            if (bsr_->getHashGenerator())
            {
                bsr_->getHashGenerator()->resume();
            }
            if (bsr_->getCRCGenerator())
            {
                bsr_->getCRCGenerator()->resume();
            }
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

            bsr_->setState(savedState_);
        }

    private:
        BitStreamStateT<StreamType>      savedState_;
        BitStreamReaderT<StreamType>     *bsr_;
    };

    template<typename StreamType>
    class BitStreamReaderT : public BitStreamStateT<StreamType>
    {
        using BitStreamStateT<StreamType>::bitBufferFill_;
        using BitStreamStateT<StreamType>::bitBuffer_;
        using BitStreamStateT<StreamType>::bitCount_;
        using BitStreamStateT<StreamType>::error_;
        using BitStreamStateT<StreamType>::bufferLengthInBits_;
        
        // Friend class for access to getState and setState
        //
        friend class BitStreamStateRestorerT<StreamType>;
        
    public:

        BitStreamReaderT();
        BitStreamReaderT(const BitStreamReaderT<StreamType> &bs2);
        BitStreamReaderT(const char *iBuffer, BitCount_t iBufferLength);
        BitStreamReaderT(const unsigned char *iBuffer, BitCount_t iBufferLength);
        BitStreamReaderT(std::ifstream &iStream);
        BitStreamReaderT(std::istream &iStream);

        ~BitStreamReaderT();

        ReturnCode Init();
        ReturnCode Init(const char *iBuffer, BitCount_t iBufferLength);
        ReturnCode Init(const unsigned char *iBuffer, BitCount_t iBufferLength);
        ReturnCode Init(const BitStreamReaderT<StreamType> &bs2);
        BitStreamReaderT<StreamType> &operator=(const BitStreamReaderT<StreamType> &bs2);

        std::ios_base::streampos streamPosition() const
        {
            return stream_->tellg();
        }
        
		StreamType& streamPosition(std::ios_base::streampos sp)
        {
            return stream_->seekg(sp);
        }
        
        ReturnCode read(uint8_t &oVal, uint8_t iNumBits);
        ReturnCode read(uint16_t &oVal, uint8_t iNumBits);
        ReturnCode read(uint32_t &oVal, uint8_t iNumBits);
        ReturnCode read(uint64_t &oVal, uint8_t iNumBits);

        ReturnCode read(int8_t &oVal, uint8_t iNumBits);
        ReturnCode read(int16_t &oVal, uint8_t iNumBits);
        ReturnCode read(int32_t &oVal, uint8_t iNumBits);
        ReturnCode read(int64_t &oVal, uint8_t iNumBits);

        ReturnCode read(bool &oval);
        ReturnCode read(char &oval);

        ReturnCode read(uint8_t &oVal);
        ReturnCode read(uint16_t &oVal);
        ReturnCode read(uint32_t &oVal);
        ReturnCode read(uint64_t &oVal);

        ReturnCode read(int8_t &oVal);
        ReturnCode read(int16_t &oVal);
        ReturnCode read(int32_t &oVal);
        ReturnCode read(int64_t &oVal);

        ReturnCode read(uint8_t *oVal, uint32_t iNumBytes);
        ReturnCode readAligned(uint8_t *oVal, uint32_t iNumBytes);

        ReturnCode peek(uint8_t &oVal, uint8_t iNumBits);
        ReturnCode peek(uint16_t &oVal, uint8_t iNumBits);
        ReturnCode peek(uint32_t &oVal, uint8_t iNumBits);
        ReturnCode peek(uint64_t &oVal, uint8_t iNumBits);

        ReturnCode peek(int8_t &oVal, uint8_t iNumBits);
        ReturnCode peek(int16_t &oVal, uint8_t iNumBits);
        ReturnCode peek(int32_t &oVal, uint8_t iNumBits);
        ReturnCode peek(int64_t &oVal, uint8_t iNumBits);

        ReturnCode peek(bool &oVal);

        ReturnCode peek(uint8_t &oVal);
        ReturnCode peek(uint16_t &oVal);
        ReturnCode peek(uint32_t &oVal);
        ReturnCode peek(uint64_t &oVal);

        ReturnCode peek(int8_t &oVal);
        ReturnCode peek(int16_t &oVal);
        ReturnCode peek(int32_t &oVal);
        ReturnCode peek(int64_t &oVal);

        ReturnCode skip(uint8_t iNumbits);
        ReturnCode skip(BitCount_t iNumbits);
        ReturnCode skipAligned(uint8_t iNumbytes);

        ReturnCode sync(const uint8_t *iVal, uint8_t iNumBytes, const uint8_t *iMask);
        ReturnCode sync(const uint8_t *iVal, uint8_t iNumBytes);

        ReturnCode align(void);

        BitCount_t getBitCount(void) const;

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

        BitStreamStateT<StreamType> reset(void);

#ifdef CMNSTRM_USE_CHECK_SUM
        bool                setHashGenerator(HashGenerator *iGenerator);
        HashGenerator*      getHashGenerator(void);

        bool                setCRCGenerator(CRCGenerator *iGenerator);
        CRCGenerator*       getCRCGenerator(void);
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

    private:
        BitStreamStateT<StreamType> getState(void) const;
        BitStreamStateT<StreamType> setState(BitStreamStateT<StreamType> state);

        ReturnCode read_unchecked(uint32_t &oVal, uint8_t iNumBits);

        StreamType		*stream_;

#ifdef CMNSTRM_USE_CHECK_SUM
        CRCGenerator		*CRC_;
        HashGenerator		*hash_;
#endif // #ifdef CMNSTRM_USE_CHECK_SUM
    };

    template<typename StreamType>
    BitStreamReaderT<StreamType>::BitStreamReaderT()
    {
        Init();
    }

    template<typename StreamType>
    BitStreamReaderT<StreamType>::BitStreamReaderT(const BitStreamReaderT<StreamType> &bs2)
    {
        ReturnCode rc = Init(bs2);
        if ( rc != OK )
        {
            // throw something appropriate
        }
    }

    template<typename StreamType>
    BitStreamReaderT<StreamType>::BitStreamReaderT(const char *iBuffer, BitCount_t iBufferLength)
    {
        ReturnCode rc = Init(iBuffer, iBufferLength);
        if ( rc != OK )
        {
            // throw something appropriate
        }
    }

    template<typename StreamType>
    BitStreamReaderT<StreamType>::BitStreamReaderT(const unsigned char *iBuffer, BitCount_t iBufferLength)
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
    ReturnCode BitStreamReaderT<StreamType>::Init()
    {
        bitBufferFill_ = 0;
        bitBuffer_ = 0;
        bitCount_ = 0;
        bufferLengthInBits_ = 0;
        stream_ = nullptr;

#ifdef CMNSTRM_USE_CHECK_SUM
        hash_ = nullptr;
        CRC_ = nullptr;
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

        error_ = OK;

        return OK;
    }

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::sync(const uint8_t *iVal, uint8_t iNumBytes, const uint8_t *iMask)
    {
        if (iNumBytes == 0 || iVal == nullptr)
        {
            return PARAMS_BAD;
        }

        BitStreamStateT<StreamType> state = getState();

        align();

        if (!good())
        {
            return status();
        }

        // Allocate enough memory to store our read data
        //
        uint8_t *currSyncBuf = new uint8_t[iNumBytes];

        // Allocate memory for temporary masked buffers
        //
        uint8_t *maskedSyncBuf = new uint8_t[iNumBytes];
        uint8_t *maskedValBuf = new uint8_t[iNumBytes];

        // Create temporary pointers for various copying
        //
        const uint8_t *maskPtr = iMask;
        uint8_t *insertionPoint = currSyncBuf;
        uint8_t *maskedSyncBufPtr = maskedSyncBuf;
        uint8_t *maskedValBufPtr = maskedValBuf;

        // If any of our allocations failed,
        // clean up and return early.
        if (currSyncBuf == nullptr || maskedSyncBuf == nullptr || maskedValBuf == nullptr)
        {
            if (currSyncBuf)
            {
                delete [] currSyncBuf;
            }

            if (maskedSyncBuf)
            {
                delete [] maskedSyncBuf;
            }

            if (maskedValBuf)
            {
                delete [] maskedValBuf;
            }

            return false;
        }

        // Don't start comparison until we have accumulated enough bytes
        //
        uint64_t accumulatedBytes = 0;
        bool foundVal = false;
        char charToRead;

        do
        {
            if (accumulatedBytes >= iNumBytes)
            {
                // Copy to tmp bufs
                //
                memcpy(maskedSyncBuf, currSyncBuf, iNumBytes);
                memcpy(maskedValBuf, iVal, iNumBytes);

                // Update tmp pointers
                //
                maskPtr = iMask;
                maskedSyncBufPtr = maskedSyncBuf;
                maskedValBufPtr = maskedValBuf;

                // Apply the mask
                //
                for (int32_t i = 0; i < iNumBytes; i++)
                {
                    *maskedSyncBufPtr = *maskedSyncBufPtr & *maskPtr;
                    *maskedValBufPtr = *maskedValBufPtr & *maskPtr;

                    ++maskedSyncBufPtr;
                    ++maskedValBufPtr;
                    ++maskPtr;
                }

                // Check to see if we have a match
                //
                int32_t success = memcmp(maskedSyncBuf, maskedValBuf, iNumBytes);

                if (success == 0)
                {
                    foundVal = true;
                    break;
                }

                stream_->read(&charToRead, 1);
                if (stream_->good())
                {

                    memmove(currSyncBuf, currSyncBuf + 1, iNumBytes - 1);
                    memcpy(currSyncBuf + iNumBytes - 1, &charToRead, 1);

                    bitCount_ += 8;
                }
            }
            else
            {
                stream_->read(&charToRead, 1);
                if (stream_->good())
                {
                    memcpy(insertionPoint, &charToRead, 1);

                    ++insertionPoint;
                    ++accumulatedBytes;

                    bitCount_ += 8;
                }
            }
        }
        while (stream_->good());

        delete [] maskedValBuf;
        delete [] maskedSyncBuf;
        delete [] currSyncBuf;

        if (foundVal)
        {
            // Seek back to the beginning of the byte sequence
            // we were looking for
            //
            stream_->seekg(-iNumBytes, std::ios::cur);

            bitCount_ -= (iNumBytes << 3);

            return status();
        }
        else
        {
            ReturnCode rc = status();
            setState(state);

            return rc;
        }
    }

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::sync(const uint8_t *iVal, uint8_t iNumBytes)
    {
        if (iNumBytes == 0 || iVal == nullptr)
        {
            return PARAMS_BAD;
        }

        BitStreamStateT<StreamType> state = getState();

        align();

        if (!good())
        {
            return status();
        }

        // Allocate enough memory to store our read data
        //
        uint8_t *currSyncBuf = new uint8_t[iNumBytes];
        uint8_t *insertionPoint = currSyncBuf;

        if (currSyncBuf == nullptr)
        {
            return false;
        }

        // Don't start comparison until we have accumulated enough bytes
        //
        uint64_t accumulatedBytes = 0;
        bool foundVal = false;
        char charToRead;

        do
        {
            if (accumulatedBytes >= iNumBytes)
            {
                // Check to see if we have a match
                int32_t success = memcmp(currSyncBuf, iVal, iNumBytes);

                if (success == 0)
                {
                    foundVal = true;
                    break;
                }

                stream_->read(&charToRead, 1);
                if (stream_->good())
                {
                    memmove(currSyncBuf, currSyncBuf + 1, iNumBytes - 1);
                    memcpy(currSyncBuf + iNumBytes - 1, &charToRead, 1);

                    bitCount_ += 8;
                }
            }
            else
            {
                stream_->read(&charToRead, 1);
                if (stream_->good())
                {
                    memcpy(insertionPoint, &charToRead, 1);

                    ++insertionPoint;
                    ++accumulatedBytes;

                    bitCount_ += 8;
                }
            }
        }
        while (stream_->good());

        delete [] currSyncBuf;

        if (foundVal)
        {
            // Seek back to the beginning of the byte sequence
            // we were looking for
            //
            stream_->seekg(-iNumBytes, std::ios::cur);

            bitCount_ -= (iNumBytes << 3);

            return status();
        }
        else
        {
            ReturnCode rc = status();
            setState(state);

            return rc;
        }
    }

    /*
     * Retreiving the number of used bits
     */

    template<typename StreamType>
    BitCount_t BitStreamReaderT<StreamType>::getBitCount(void) const
    {
        return bitCount_;
    }

    template<typename StreamType>
    void BitStreamReaderT<StreamType>::clear()
    {
        error_ = OK;
        stream_->clear();
    }

    /**
     * Is the byte stream good?
     */

    template<typename StreamType>
    bool BitStreamReaderT<StreamType>::good()
    {
        if (error_ != OK)
        {
            return  false;
        }

        return stream_->good();
    }

    /**
     * Is the byte stream in EOF state?
     */

    template<typename StreamType>
    bool BitStreamReaderT<StreamType>::eof()
    {
        if (stream_->eof())
            error_ = IO_EOF;
        return error_ == IO_EOF;
    }

    /**
     * Is the byte stream in FAIL state?
     */

    template<typename StreamType>
    bool BitStreamReaderT<StreamType>::fail()
    {
        if (stream_->fail())
            error_ = IO_FAIL;
        return error_ == IO_FAIL;
    }

    /**
     *  Retrieving the error code from the byte stream state
     */

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::status(void)
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

    /**
     *
     */

    // Note that this functions may cause stream failures

    template<typename StreamType>
    BitStreamStateT<StreamType> BitStreamReaderT<StreamType>::getState(void) const
    {
        BitStreamStateT<StreamType> state;

        state.streamState(stream_->rdstate());
        state.streamPosition(stream_->tellg());

        state.bitBuffer(bitBuffer_);
        state.bitBufferFill(bitBufferFill_);
        state.bitCount(bitCount_);
        state.bufferLengthInBits(bufferLengthInBits_);
        state.error(error_);

        return state;
    }

    // Note that this functions may cause stream failures

    template<typename StreamType>
    BitStreamStateT<StreamType> BitStreamReaderT<StreamType>::setState(BitStreamStateT<StreamType> state)
    {
        stream_->clear();
        stream_->setstate(state.streamState());
        stream_->seekg(state.streamPosition());
        
        bitBuffer_ = state.bitBuffer();
        bitBufferFill_ = state.bitBufferFill();
        bitCount_ = state.bitCount();
        bufferLengthInBits_ = state.bufferLengthInBits();
        error_ = state.error();

        return state;
    }

    // Note that this functions may cause stream failures

    template<typename StreamType>
    BitStreamStateT<StreamType> BitStreamReaderT<StreamType>::reset(void)
    {
        stream_->clear();
        stream_->seekg(0);

        bitBuffer_ = 0;
        bitBufferFill_ = 0;
        bitCount_ = 0;

        error_ = OK;

        return getState();
    }

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::read(uint8_t *oVal, uint32_t iNumBytes)
    {
        if (iNumBytes == 0 || oVal == nullptr)
        {
            return PARAMS_BAD;
        }

        if (!good())
        {
            return status();
        }

        uint64_t totalBitCount = bitCount_ + (iNumBytes * 8);

        // We will hit a buffer overflow
        // Make sure we flush the last bits followed by 0
        // then return IO_EOF
        //
        if (totalBitCount > bufferLengthInBits_)
        {
            error_ = IO_EOF;
            return IO_EOF;
        }

        stream_->read((char *) oVal, iNumBytes);
        if (stream_->good())
        {
#ifdef CMNSTRM_USE_CHECK_SUM
            if (hash_ && hash_->useHash())
            {
                hash_->accumulate(oVal, iNumBytes);
            }
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

            uint8_t *buf = oVal;
            uint8_t *end = oVal + iNumBytes;

            while (buf != end)
            {
                uint8_t tmp = *buf;

                *buf = (tmp >> bitBufferFill_) | bitBuffer_;

                bitBuffer_ = tmp << (8 - bitBufferFill_);

#ifdef CMNSTRM_USE_CHECK_SUM
                if (CRC_ && CRC_->useCRC())
                {
                    CRC_->accumulate((uint32_t)*buf, 8);
                }
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

                buf++;
            }

            return status();

        }
        else
        {
            return status();
        }

    }

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::readAligned(uint8_t *oVal, uint32_t iNumBytes)
    {
        if (iNumBytes == 0 || oVal == nullptr)
        {
            return PARAMS_BAD;
        }

        if (!good())
        {
            return status();
        }

        align();

        return this->read(oVal, iNumBytes);
    }

    /*
     * Reading and peeking booleans
     */
    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::read(bool &oVal)
    {
        if (!good())
        {
            return status();
        }

        // Make sure there is at least one bit available to read

        BitCount_t totalBitCount = bitCount_ + 1;

        // We will hit a buffer overflow
        // Make sure we flush the last bits followed by 0
        // then return IO_EOF
        //
        if (totalBitCount > bufferLengthInBits_)
        {
            error_ = IO_EOF;
            return IO_EOF;
        }

        if (bitBufferFill_ == 0)
        {
            char c;

            //stream_->get(c);
            stream_->read(&c, 1);
            if (stream_->good())
            {
                bitBuffer_ = (unsigned char) c;
                bitBufferFill_ = 8;

#ifdef CMNSTRM_USE_CHECK_SUM
                if (hash_ && hash_->useHash())
                {
                    hash_->accumulate(&bitBuffer_, 1);
                }
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

            }
            else
            {
                return status();
            }
        }

        // Asserted: bitBufferFill_ > 0 => read bit

        oVal = (bitBuffer_ >> 7) != 0;
        bitBuffer_ <<= 1;
        bitBufferFill_--;
        bitCount_++;

#ifdef CMNSTRM_USE_CHECK_SUM
        if (CRC_ && CRC_->useCRC())
        {
            CRC_->accumulate((uint32_t)oVal, 1);
        }
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

        return status();
    }

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::peek(bool &oVal)
    {
        if (!good())
        {
            return status();
        }

        BitStreamStateRestorerT<StreamType> s(this);

        return read(oVal);
    }

    /*
     * Reading and peeking 8-bit unsigned integers
     */

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::read(uint8_t &oVal, uint8_t iNumBits)
    {
        if (!good())
        {
            return status();
        }

        if (iNumBits > 8)
        {
            return PARAMS_BAD;
        }

        uint32_t tmp = 0;
        this->read(tmp, iNumBits);
        oVal = static_cast<uint8_t>(tmp);

        return status();
    }

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::peek(uint8_t &oVal, uint8_t iNumBits)
    {
        if (!good())
        {
            return status();
        }

        BitStreamStateRestorerT<StreamType> s(this);

        return read(oVal, iNumBits);
    }

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::read(uint8_t &oVal)
    {
        return read(oVal, 8);
    }

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::peek(uint8_t &oVal)
    {
        if (!good())
        {
            return status();
        }

        BitStreamStateRestorerT<StreamType> s(this);

        return read(oVal);
    }

    /*
     * Reading and peeking 16-bit unsigned integers
     */

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::read(uint16_t &oVal, uint8_t iNumBits)
    {
        if (!good())
        {
            return status();
        }

        if (iNumBits > 16)
        {
            return PARAMS_BAD;
        }

        uint32_t tmp = 0;
        this->read_unchecked(tmp, iNumBits);
        oVal = static_cast<uint16_t>(tmp);

        return status();
    }

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::peek(uint16_t &oVal, uint8_t iNumBits)
    {
        if (!good())
        {
            return status();
        }

        BitStreamStateRestorerT<StreamType> s(this);

        return read(oVal, iNumBits);
    }

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::read(uint16_t &oVal)
    {
        return read(oVal, 16);
    }

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::peek(uint16_t &oVal)
    {
        if (!good())
        {
            return status();
        }

        BitStreamStateRestorerT<StreamType> s(this);

        return read(oVal);
    }

    /*
     * Reading and peeking 32-bit unsigned integers
     */
    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::read(uint32_t &oVal, uint8_t iNumBits)
    {
        if (!good())
        {
            return status();
        }

        if (iNumBits > 32)
        {
            return PARAMS_BAD;
        }

        return read_unchecked(oVal, iNumBits);
    }

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::read_unchecked(uint32_t &oVal, uint8_t iNumBits)
    {
        oVal = 0;

        if ( iNumBits == 0 )
        {
            return status();
        }

        BitCount_t totalBitCount = bitCount_ + iNumBits;

        // We will hit a buffer overflow
        // Make sure we flush the last bits followed by 0
        // then return IO_EOF
        //
        if (totalBitCount > bufferLengthInBits_)
        {
            error_ = IO_EOF;
            return IO_EOF;
        }

        uint8_t currentBit = iNumBits;

        while (currentBit > 0)
        {
            uint32_t rbits = (currentBit > bitBufferFill_) ? bitBufferFill_ : currentBit;
            currentBit -= rbits;

            if (rbits > 0)
            {
                oVal = (oVal << rbits) | (bitBuffer_ >> (8 - rbits));
                bitBuffer_ = bitBuffer_ << rbits;
                bitBufferFill_ -= rbits;
                bitCount_ += rbits;
            }

            if (currentBit > 0)
            {
                char c;

                //stream_->read(&c, 1);
                stream_->get(c);
                if (stream_->good())
                {
                    bitBuffer_ = (unsigned char) c;
                    bitBufferFill_ = 8;

#ifdef CMNSTRM_USE_CHECK_SUM
                    if (hash_ && hash_->useHash())
                    {
                        hash_->accumulate(&bitBuffer_, 1);
                    }
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

                }
                else
                {
                    return status();
                }
            }
        }

#ifdef CMNSTRM_USE_CHECK_SUM
        if (CRC_ && CRC_->useCRC())
        {
            CRC_->accumulate(oVal, iNumBits);
        }
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

        return status();
    }

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::peek(uint32_t &oVal, uint8_t iNumBits)
    {
        if (!good())
        {
            return status();
        }

        BitStreamStateRestorerT<StreamType> s(this);

        return read(oVal, iNumBits);
    }

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::read(uint32_t &oVal)
    {
        return read(oVal, 32);
    }

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::peek(uint32_t &oVal)
    {
        if (!good())
        {
            return false;
        }

        BitStreamStateRestorerT<StreamType> s(this);

        return read(oVal);
    }

    /*
     * Reading and peeking 64-bit unsigned integers
     */

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::read(uint64_t &oVal, uint8_t iNumBits)
    {
        if (!good())
        {
            return status();
        }

        if (iNumBits > 64)
        {
            return PARAMS_BAD;
        }

        BitCount_t totalBitCount = bitCount_ + iNumBits;

        // We will hit a buffer overflow
        // Make sure we flush the last bits followed by 0
        // then return IO_EOF
        //
        if (totalBitCount > bufferLengthInBits_)
        {
            error_ = IO_EOF;
            return IO_EOF;
        }

        uint32_t oVal32;
        oVal = 0;
        ReturnCode rc;
        uint8_t bitsRead = 0;
        if ( iNumBits > 32 )
        {
            rc = read_unchecked(oVal32, iNumBits-32);
            if ( rc != OK )
            {
                return rc;
            }
            oVal = static_cast<uint64_t>(oVal32) << 32;
            bitsRead = 32;
        }
        rc = read_unchecked(oVal32, iNumBits - bitsRead);
        if ( rc )
        {
            return rc;
        }
        oVal |= static_cast<uint64_t>(oVal32);

        bitCount_ = totalBitCount;
        return status();
    }

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::peek(uint64_t &oVal, uint8_t iNumBits)
    {
        if (!good())
        {
            return status();
        }

        BitStreamStateRestorerT<StreamType> s(this);

        return read(oVal, iNumBits);
    }


    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::read(uint64_t &oVal)
    {
        return read(oVal, 64);
    }

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::peek(uint64_t &oVal)
    {
        if (!good())
        {
            return false;
        }

        BitStreamStateRestorerT<StreamType> s(this);

        return read(oVal);
    }

    /*
     * Reading and peeking 8-bit integers
     */

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::read(int8_t &oVal, uint8_t iNumBits)
    {
        if (!good())
        {
            return status();
        }

        if (iNumBits > 8)
        {
            return PARAMS_BAD;
        }

        uint32_t tmp = 0;
        this->read_unchecked(tmp, iNumBits);
        oVal = static_cast<int8_t>(tmp);

        return status();
    }

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::peek(int8_t &oVal, uint8_t iNumBits)
    {
        if (!good())
        {
            return status();
        }

        BitStreamStateRestorerT<StreamType> s(this);

        return read(oVal, iNumBits);
    }

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::read(char &oVal)
    {
        int8_t tmp;
        read(tmp, 8);
        oVal = tmp;

        return status();
    }

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::read(int8_t &oVal)
    {
        return read(oVal, 8);
    }
    
    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::peek(int8_t &oVal)
    {
        if (!good())
        {
            return status();
        }

        BitStreamStateRestorerT<StreamType> s(this);

        return read(oVal);
    }

    /*
     * Reading and peeking 16-bit integers
     */

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::read(int16_t &oVal, uint8_t iNumBits)
    {
        if (!good())
        {
            return status();
        }

        if (iNumBits > 16)
        {
            return PARAMS_BAD;
        }

        int32_t tmp = 0;
        this->read(tmp, iNumBits);
        oVal = static_cast<int16_t>(tmp);

        return status();
    }

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::peek(int16_t &oVal, uint8_t iNumBits)
    {
        if (!good())
        {
            return status();
        }

        BitStreamStateRestorerT<StreamType> s(this);

        return read(oVal, iNumBits);
    }

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::read(int16_t &oVal)
    {
        return read(oVal, 16);
    }

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::peek(int16_t &oVal)
    {
        if (!good())
        {
            return status();
        }

        BitStreamStateRestorerT<StreamType> s(this);

        return read(oVal);
    }

    /*
     * Reading and peeking 32-bit integers
     */

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::read(int32_t &oVal, uint8_t iNumBits)
    {
        if (!good())
        {
            return status();
        }

        if (iNumBits > 32)
        {
            return PARAMS_BAD;
        }

        uint32_t tmp = 0;
        this->read_unchecked(tmp, iNumBits);
        oVal = static_cast<int32_t>(tmp);

        return status();
    }

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::peek(int32_t &oVal, uint8_t iNumBits)
    {
        if (!good())
        {
            return status();
        }

        BitStreamStateRestorerT<StreamType> s(this);

        return read(oVal, iNumBits);
    }

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::read(int32_t &oVal)
    {
        return read(oVal, 32);
    }

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::peek(int32_t &oVal)
    {
        if (!good())
        {
            return false;
        }

        BitStreamStateRestorerT<StreamType> s(this);

        return read(oVal);
    }

    /*
     * Reading and peeking 64-bit integers
     */

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::read(int64_t &oVal, uint8_t iNumBits)
    {
        if (!good())
        {
            return status();
        }

        if (iNumBits > 64)
        {
            return PARAMS_BAD;
        }

        oVal = 0;

        uint64_t tmp;
        this->read(tmp, iNumBits);
        oVal = static_cast<uint64_t>(tmp);

        return status();
    }

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::peek(int64_t &oVal, uint8_t iNumBits)
    {
        if (!good())
        {
            return status();
        }

        BitStreamStateRestorerT<StreamType> s(this);

        return read(oVal, iNumBits);
    }

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::read(int64_t &oVal)
    {
        return read(oVal, 64);
    }

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::peek(int64_t &oVal)
    {
        if (!good())
        {
            return false;
        }

        BitStreamStateRestorerT<StreamType> s(this);

        return read(oVal);
    }

    /*
     * Aligning
     */

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::align(void)
    {
        if (!good())
        {
            return status();
        }

        uint8_t tmp;

        if (bitBufferFill_)
        {
            return read(tmp, bitBufferFill_);
        }

        return status();
    }

    /*
     * Skipping
     */

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::skip(BitCount_t iNumbits)
    {
        if (!good())
        {
            return status();
        }

        BitCount_t bytesLeftToRead = iNumbits / 8;

        while (bytesLeftToRead)
        {
            uint8_t tmp;
            read(tmp, 8);
            --bytesLeftToRead;
        }

        uint8_t bitsLeftToRead = iNumbits % 8;
        if (bitsLeftToRead)
        {
            uint64_t tmp;
            read(tmp, bitsLeftToRead);
        }

        return status();
    }


    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::skip(uint8_t iNumbits)
    {
        return skip(static_cast<BitCount_t>(iNumbits));
    }

    template<typename StreamType>
    ReturnCode BitStreamReaderT<StreamType>::skipAligned(uint8_t iNumbytes)
    {
        if (!good())
        {
            return status();
        }

        this->align();

        return 0;
    }

#ifdef CMNSTRM_USE_CHECK_SUM
    template<typename StreamType>
    bool BitStreamReaderT<StreamType>::setHashGenerator(HashGenerator *iGenerator)
    {
        if ((hash_ != nullptr) && (iGenerator != nullptr))
        {
            return false;
        }

        hash_ = iGenerator;
        return true;
    }

    template<typename StreamType>
    HashGenerator* BitStreamReaderT<StreamType>::getHashGenerator(void)
    {
        return hash_;
    }

    template<typename StreamType>
    bool BitStreamReaderT<StreamType>::setCRCGenerator(CRCGenerator *iGenerator)
    {
        if ((CRC_ != nullptr) && (iGenerator != nullptr))
        {
            return false;
        }

        CRC_ = iGenerator;
        return true;
    }

    template<typename StreamType>
    CRCGenerator* BitStreamReaderT<StreamType>::getCRCGenerator(void)
    {
        return CRC_;
    }
#endif // #ifdef CMNSTRM_USE_CHECK_SUM

} //namespace CommonStream

#endif // COMMON_STREAM_SRC_BITSTREAM_BITSTREAMREADERT_H_

