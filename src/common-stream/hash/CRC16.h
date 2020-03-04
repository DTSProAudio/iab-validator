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

/**
 * Class to compute CRC from a bitstream block
 *
 * @file
 */
 
#ifndef COMMON_STREAM_SRC_HASH_CRC16_H_
#define COMMON_STREAM_SRC_HASH_CRC16_H_

#include "Namespace.h"
#include "CRCGenerator.h"

namespace CommonStream
{

    class CRC16Generator : public CRCGenerator
    {
    public:

        CRC16Generator();
        ~CRC16Generator();

        virtual ECRCType getCRCType(void);

        virtual ECRCState getState(void);

        virtual bool useCRC(void);

        virtual bool start(void);
        virtual bool stop(void);

        virtual bool pause(void);
        virtual bool resume(void);

        virtual bool accumulate(uint32_t iBits, uint8_t iBitCount);
        virtual bool accumulate(uint64_t iBits, uint8_t iBitCount);

        virtual uint32_t getCRCRegisterSize(void);
        virtual bool getCRCRegister(uint8_t *oCRC);

    private:

        uint16_t    register_;
        ECRCState   state_;
    };

}  // namespace CommonStream

#endif // COMMON_STREAM_SRC_HASH_CRC16_H_
