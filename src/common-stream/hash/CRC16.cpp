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

#include "CRC16.h"
#include <string.h>
#include <cassert>

/*
	Implements CCIT CRC 16
*/

namespace CommonStream
{

    CRC16Generator::CRC16Generator()
    {
        state_ = eCRCState_Stopped;
    }

    CRC16Generator::~CRC16Generator()
    {
    }

    ECRCType CRC16Generator::getCRCType(void)
    {
        return eCRCType_CRC16;
    }

    CRCGenerator::ECRCState CRC16Generator::getState(void)
    {
        return state_;
    }

    bool CRC16Generator::useCRC(void)
    {
        bool useCRC = (state_ == eCRCState_Started) || (state_ == eCRCState_Paused);

        return useCRC;
    }

    bool CRC16Generator::start()
    {
        if ((state_ == eCRCState_Stopped)
                || (state_ == eCRCState_Started)
                || (state_ == eCRCState_Paused))
        {
            state_ = eCRCState_Started;
            register_ = 0xFFFF;

            return true;
        }
        else
        {
            return false;
        }
    }

    bool CRC16Generator::accumulate(uint32_t iBits, uint8_t iBitCount)
    {
        if (state_ == eCRCState_Stopped)
        {
            return false;
        }
        else if (state_ == eCRCState_Paused)
        {
            return true;
        }

        bool high_bit = false;

        iBits = iBits << (32 - iBitCount);

        for(int i = iBitCount - 1; i >= 0; i--)
        {

            high_bit = ((register_ >> 15) != 0);

            register_ = (register_ << 1) | ((iBits >> 31) & 1);

            iBits = iBits << 1;

            if (high_bit)
            {
                register_ = register_ ^ 0x1021;
            }
        }

        return true;
    }

    bool CRC16Generator::accumulate(uint64_t iBits, uint8_t iBitCount)
    {
        if (state_ == eCRCState_Stopped)
        {
            return false;
        }
        else if (state_ == eCRCState_Paused)
        {
            return true;
        }

        bool high_bit = false;

        iBits = iBits << (64 - iBitCount);

        for(int i = iBitCount - 1; i >= 0; i--)
        {

            high_bit = ((register_ >> 15) != 0);

            register_ = (register_ << 1) | ((iBits >> 63) & 1);

            iBits = iBits << 1;

            if (high_bit)
            {
                register_ = register_ ^ 0x1021;
            }
        }

        return true;
    }

    bool CRC16Generator::stop()
    {
        uint32_t bits = 0;
        accumulate(bits, 16);

        if ((state_ == eCRCState_Stopped)
                || (state_ == eCRCState_Started)
                || (state_ == eCRCState_Paused))
        {
            state_ = eCRCState_Stopped;
        }
        else
        {
            return false;
        }

        return true;
    }

    bool CRC16Generator::pause(void)
    {
        if ((state_ == eCRCState_Paused) || (state_ == eCRCState_Started))
        {
            state_ = eCRCState_Paused;
            return true;
        }
        else
        {
            return false;
        }
    }

    bool CRC16Generator::resume(void)
    {
        if (state_ == eCRCState_Paused)
        {
            state_ = eCRCState_Started;
        }
        else
        {
            return false;
        }

        return true;
    }

    uint32_t CRC16Generator::getCRCRegisterSize(void)
    {
        return sizeof(register_);
    }

    bool CRC16Generator::getCRCRegister(uint8_t *oCRC)
    {
        memcpy(oCRC, &register_, this->getCRCRegisterSize());

        return true;
    }

}  // namespace CommonStream
