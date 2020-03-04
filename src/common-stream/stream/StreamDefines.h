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

#ifndef COMMON_STREAM_SRC_UTILS_STREAMDEFINES_H_
#define COMMON_STREAM_SRC_UTILS_STREAMDEFINES_H_

#include "Namespace.h"
#include "StreamBasicTypes.h"

namespace CommonStream
{
    typedef int ReturnCode;

    // MDA Needs to support 64-bit values here
    //
    typedef uint64_t BitCount_t;
    typedef int64_t  SBitCount_t;

    typedef uint64_t ByteCount_t;
    typedef int64_t  SByteCount_t;

#define OK              (0)

#define IO_EOF          (-1)
#define IO_FAIL         (1)

#define PARAMS_BAD      (100)

#define CMNSTRM_USE_CHECK_SUM
}   // namespace CommonStream

#endif  // COMMON_STREAM_SRC_UTILS_STREAMDEFINES_H_
