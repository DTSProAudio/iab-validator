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

#ifndef COMMON_STREAM_SRC_UTILS_RAWBUF_H_
#define COMMON_STREAM_SRC_UTILS_RAWBUF_H_

#include <streambuf>
#include <string>
#include "Namespace.h"

namespace CommonStream
{

template <typename char_type>
struct OStreamBufWrapper : public std::basic_streambuf<char_type, std::char_traits<char_type> >
{
    OStreamBufWrapper(char_type* buffer, std::streamsize bufferLength)
    {
        // set the "put" pointer the start of the buffer and record its length.
        this->setp(buffer, buffer + bufferLength);
    }
};

template <typename char_type>
struct IStreamBufWrapper : public std::basic_streambuf<char_type, std::char_traits<char_type> >
{
    IStreamBufWrapper(char_type* buffer, std::streamsize bufferLength)
    {
        // set the "put" pointer the start of the buffer and record its length.
        this->setg(buffer, buffer, buffer + bufferLength);
    }
};

}  // namespace CommonStream

#endif // COMMON_STREAM_SRC_UTILS_RAWBUF_H_
