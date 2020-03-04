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
 * Header file for providing support for nullptr on older g++ compilers which
 * do not support C++11, such as g++ v4.4.7
 *
 * @file
 */

#ifndef NULLPTR_OLD_COMPILERS_H_
#define NULLPTR_OLD_COMPILERS_H_

/* Support for nullptr was added in GCC 4.6.0: older versions such as g++ v4.4.7 cannot handle nullptr */
#ifdef USE_MDA_NULLPTR
const class NullPtrTemplate
{
public:
    template<class T>
    inline operator T*() const
    {
        return 0;
    }
    
    template<class C, class T>
    inline operator T C::*() const
    {
        return 0;
    }

    void operator&() const;
    
} nullptr = {};
#endif /* USE_MDA_NULLPTR */

#endif /* NULLPTR_OLD_COMPILERS_H_ */

