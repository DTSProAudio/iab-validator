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
  * Instantiate a streambuf from an iterator
  *
  * @file
  */

#ifndef COMMON_STREAM_SRC_UTILS_IITERSBUF_H_
#define COMMON_STREAM_SRC_UTILS_IITERSBUF_H_

#include <streambuf>
#include <string>
#include "Namespace.h"

namespace CommonStream
{

template<class I>
class iitersbuf : public std::streambuf
{
public:

    iitersbuf(I pStart, I pEnd);

    I& getIterator();

protected:

    virtual int_type underflow();

    virtual int_type uflow();

    I			fHead;
    I			fTail;
};

template<class I>
iitersbuf<I>::iitersbuf(I pStart,
                        I pEnd)
:
std::streambuf()
{
    fHead = pStart;
    fTail = pEnd;
}

template<class I>
I& iitersbuf<I>::getIterator()
{
    return fHead;
}

template<class I>
std::streambuf::int_type iitersbuf<I>::underflow()
{
    if (fHead != fTail)
    {
        return std::char_traits<char>::to_int_type(*fHead);
    }
    else
    {
        return (std::char_traits<char>::eof());
    }
}

template<class I>
std::streambuf::int_type iitersbuf<I>::uflow()
{
    std::streambuf::int_type r = underflow();

    if (r != std::char_traits<char>::eof())
    {
        fHead++;
    }

    return r;
}

}  // namespace CommonStream

#endif // COMMON_STREAM_SRC_UTILS_IITERSBUF_H_
