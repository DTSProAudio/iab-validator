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

#ifndef COMMON_STREAM_SRC_HASH_MD5GENERATOR_H_
#define COMMON_STREAM_SRC_HASH_MD5GENERATOR_H_

#include "Namespace.h"
#include "HashGenerator.h"

namespace CommonStream
{

    class MD5Generator : public HashGenerator
    {
    public:

        MD5Generator();
        virtual ~MD5Generator();

        virtual EHashType getHashType(void);

        virtual EHashState getState(void);

        virtual bool useHash(void);

        virtual bool start(void);

        virtual bool stop(void);

        virtual bool pause(void);

        virtual bool resume(void);

        virtual bool accumulate(uint8_t *iValue, uint32_t iByteCount);

        virtual uint32_t getHashSize(void);

        virtual bool getHash(uint8_t *oHash);

    private:
        
        struct MD5Context {
            uint32_t buf[4];
            uint32_t bits[2];
            unsigned char in[64];
        };

        static void MD5Init(struct MD5Context *context);
        static void MD5Update(struct MD5Context *context, unsigned char const *buf, unsigned len);
        static void MD5Final(unsigned char digest[16], struct MD5Context *context);
        static void MD5Transform(uint32_t buf[4], uint32_t const in[16]);

        MD5Context ctx_;
        unsigned char ss_[16];
        EHashState      state_;
    };

}  // namespace CommonStream

#endif // COMMON_STREAM_SRC_HASH_MD5GENERATOR_H_
