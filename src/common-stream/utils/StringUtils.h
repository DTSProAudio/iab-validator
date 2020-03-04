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

#ifndef COMMON_STREAM_SRC_UTILS_STRINGUTILS_H_
#define COMMON_STREAM_SRC_UTILS_STRINGUTILS_H_

#include <iostream>
#include <string>

#include "Namespace.h"

namespace CommonStream
{

    namespace StringUtils
    {
        class UTF8
        {
        public:

            static std::string ws2utf8 (const std::wstring &wstr);
            static std::wstring utf82ws (const std::string &str);
            inline static unsigned int utf82uc(const std::string &str, unsigned int stridx, unsigned int &charcnt);

        private:

            static std::string uctail(unsigned int cnt, unsigned int uc);
            static std::string uc2utf8(unsigned int uc);
            static unsigned int nbts(unsigned char s);
            static unsigned int utf82uc(std::string str);
        };

        /**
         *
         * UTF8 Conversion Utitilities
         *
         * When conversion fails, an empty string is returned.
         *
         */

        std::string wtoutf8 (const std::wstring &wstr);
        std::wstring utf8tow(const std::string &str);

    }  // namespace StringUtils

}  // namespace CommonStream

#endif // COMMON_STREAM_SRC_UTILS_STRINGUTILS_H_
