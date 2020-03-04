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

#ifndef COMMON_STREAM_SRC_STREAM_STREAMTYPES_H_
#define COMMON_STREAM_SRC_STREAM_STREAMTYPES_H_

#include <map>
#include <exception>

#include "Namespace.h"
#include "StreamBasicTypes.h"

#ifdef USE_MDA_NULLPTR
/* support lack of nullptr on older C++ compilers such as g++-4.4.7 */
#include "nullptrOldCompilers.h"
#endif

namespace CommonStream
{

    /******************************************************************************
     *
     * VarUInt32
     *
     *****************************************************************************/

    template<int I>
    struct VarUInt32
    {

        uint32_t	value_;

        operator uint32_t()
        {
            return value_;
        }

        uint32_t operator=(uint32_t iVal)
        {
            value_ = iVal;
            return iVal;
        }

    };

    /******************************************************************************
     *
     * PackedInteger
     *
     *****************************************************************************/

    struct PackedUInt32
    {

        uint32_t	value_;

        uint32_t operator=(uint32_t iVal)
        {
            value_ = iVal;
            return iVal;
        }

        operator uint32_t()
        {
            return value_;
        }

    };

    /******************************************************************************
     *
     * PackedUInt64
     *
     *****************************************************************************/

    struct PackedUInt64
    {

        uint64_t	value_;

        uint64_t operator=(uint64_t iVal)
        {
            value_ = iVal;
            return iVal;
        }

        operator uint64_t()
        {
            return value_;
        }

    };

    /******************************************************************************
     *
     * PackedLength io
     *
     *****************************************************************************/

    struct PackedLength
    {
        uint32_t	length_;
    };

    template<class T>
    class OptionalParameter
    {

    public:

        OptionalParameter() : present_(false)
        {
        }

        void setValue(const T &iVal)
        {
            value_ = iVal;
            present_ = true;
        }

        T& getValue()
        {
            return value_;
        }

        const T& getValue() const
        {
            return value_;
        }

        bool getPresent() const
        {
            return present_;
        }

        void setPresent(bool iPresent = true)
        {
            present_ = iPresent;
        }

        operator uint32_t()
        {
            return (uint32_t) value_;
        }

        uint32_t operator=(uint32_t iVal)
        {

	// The assignment of value_ = iVal causes
	// a warning for conversion due to VarUInt32.
	//
	// There is no simple way to properly implement casting to
	// remove the warning.
            value_ = iVal;
            
            setPresent();
            return iVal;
        }

        T operator=(T iVal)
        {
            value_ = iVal;
            setPresent();
            return iVal;
        }
    private:
        bool	present_;
        T		value_;
    };

    template<uint32_t N>
    class Plex
    {

    public:

        Plex(uint32_t n)
        {
            value_ = n;
        }

        Plex()
        {
            value_ = 0;
        }

        operator uint32_t() const
        {
            return value_;
        }

        uint32_t operator=(uint32_t iVal)
        {
            value_ = iVal;
            return iVal;
        }

    private:
        uint32_t value_;
    };

}  // namespace CommonStream

#endif // COMMON_STREAM_SRC_STREAM_STREAMTYPES_H_
