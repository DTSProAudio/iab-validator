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

#ifndef COMMON_STREAM_SRC_UTILS_STREAMBASICTYPES_H_
#define COMMON_STREAM_SRC_UTILS_STREAMBASICTYPES_H_

#if defined(_MSC_VER) && _MSC_VER <= 1500

// Visual Studio 2008 and below doesn't support C99 headers
// 8 bit unsigned integer
typedef unsigned char uint8_t;
// 8 bit signed integer
typedef signed char int8_t;
// 16 bit unsigned integer
typedef unsigned short uint16_t; // NOLINT
// 16 bit signed integer
typedef short int16_t; // NOLINT
// 32 bit unsigned integer
typedef unsigned int uint32_t;
// 32 bit signed integer
typedef int int32_t;
// 64 bit unsigned integer
typedef unsigned long long uint64_t; // NOLINT
// 64 bit signed integer
typedef long long int64_t; // NOLINT

#define UINT8_MAX   0xff
#define UINT16_MAX  0xffff
#define UINT32_MAX  0xffffffff
#define UINT64_MAX  0xffffffffffffffff

#define INT8_MAX    0x7f
#define INT16_MAX   0x7fff
#define INT32_MAX   0x7fffffff
#define INT64_MAX   0x7fffffffffffffff

#define INT8_MIN    (-0x7f - 1)
#define INT16_MIN   (-0x7fff - 1)
#define INT32_MIN   (-0x7fffffff - 1)
#define INT64_MIN   (-0x7fffffffffffffff - 1)

#else
#include <stdint.h>
#endif

#include <stddef.h> // contains additional types like size_t etc. & macro NULL

#ifdef USE_MDA_NULLPTR
/* support lack of nullptr on older C++ compilers such as g++-4.4.7 */
#include "nullptrOldCompilers.h"
#endif

#endif  // COMMON_STREAM_SRC_UTILS_STREAMBASICTYPES_H_
