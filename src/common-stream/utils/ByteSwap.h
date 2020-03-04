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

#ifndef COMMON_STREAM_SRC_UTILS_BYTESWAP_H_
#define COMMON_STREAM_SRC_UTILS_BYTESWAP_H_

#include "Namespace.h"

namespace CommonStream
{

    namespace BYTESWAP
    {
        inline void swapByteOrder(uint16_t& us)
        {
            us = (us >> 8) |
                 (us << 8);
        }

        inline void swapByteOrder(uint32_t& ui)
        {
            ui = (ui >> 24) |
                 ((ui<<8) & 0x00FF0000) |
                 ((ui>>8) & 0x0000FF00) |
                 (ui << 24);
        }

        inline void swapByteOrder(uint64_t& ull)
        {
            ull = (ull >> 56) |
                  ((ull<<40) & 0x00FF000000000000) |
                  ((ull<<24) & 0x0000FF0000000000) |
                  ((ull<<8) & 0x000000FF00000000) |
                  ((ull>>8) & 0x00000000FF000000) |
                  ((ull>>24) & 0x0000000000FF0000) |
                  ((ull>>40) & 0x000000000000FF00) |
                  (ull << 56);
        }

    }  // namespace BYTESWAP

}  // namespace CommonStream

#endif // COMMON_STREAM_SRC_UTILS_BYTESWAP_H_
