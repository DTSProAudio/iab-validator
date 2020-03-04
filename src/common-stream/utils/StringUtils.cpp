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

#include "StringUtils.h"
#include <string>

#if defined(_WIN32) && _MSC_VER >= 1600
#include <cvt/wstring>
#include <codecvt>
#endif

namespace CommonStream
{

    namespace StringUtils
    {

        std::string UTF8::uc2utf8(unsigned int uc)
        {
            std::string str("");

            char c;

            if (uc < 128)
            {
                c = (char) uc;
                str.append(1,c);
                return str;
            }

            if (uc < (0x0800))
            {
                c = (((uc >> 6) & 0x1F) | 0xc0);

                str.append(1,c);

                str += uctail(1,uc);

                return str;
            }

            if (uc < (0x010000))
            {
                c = (((uc >> 12) & 0x0F) | 0xe0);

                str.append(1,c);
                str += uctail(2,uc);

                return str;
            }

            if (uc < (0x200000))
            {
                c = (((uc >> 18) & 0x07) | 0xf0);

                str.append(1,c);
                str += uctail(3,uc);

                return str;
            }

            if (uc < (0x04000000))
            {
                c = (((uc >> 24) & 0x03) | 0xf8);

                str.append(1,c);
                str += uctail(4,uc);

                return str;
            }

            if (uc < (0x80000000))
            {
                c = (((uc >> 30) & 0x01) | 0xfc);

                str.append(1,c);
                str += uctail(5,uc);

                return str;
            }

            return str;
        }


        std::string UTF8::uctail(unsigned int cnt,unsigned int uc)
        {
            char c;

            std::string str("");

            for (int m = cnt-1; m>=0; m--)
            {
                c = static_cast<char>((((uc >> (m*6)) & 0x3F) | 0x80));

                str.append(1,c);
            }

            return str;
        }

        std::string UTF8::ws2utf8 (const std::wstring &wstr)
        {
            std::string str("");

            for (unsigned int i=0; i < wstr.length(); i++)
            {
                str += uc2utf8(wstr[i]);
            }

            return str;
        }

        unsigned int UTF8::nbts(unsigned char s)
        {
            int m = 0;

            for (int m=0; m<8; m++)
            {
                if (((s << m) & 0x80) == 0)
                {
                    return m == 0 ? 1 : m;
                }
            }

            return m;
        }

        inline unsigned int UTF8::utf82uc(const std::string &str, unsigned int stridx, unsigned int &charcnt)
        {
            unsigned char c = (unsigned char) str[stridx];

            charcnt = nbts(c);

            if (charcnt == 1)
            {
                return (unsigned int) c;
            }

            unsigned int uc = (((1 << (7-charcnt))-1) & str[stridx]) << (charcnt-1)*6;

            stridx++;

            for (unsigned int i=1; i<charcnt; stridx++,i++)
            {
                c = (unsigned char) str[stridx];

                uc += ((c & 0x3F) << (charcnt-1-i)*6);
            }

            return uc;
        }

        std::wstring UTF8::utf82ws(const std::string &str)
        {
            wchar_t wcharbuf[128];

            unsigned int strlength = (unsigned int) str.length();
            unsigned int stridx = 0;
            unsigned int charcnt;

            unsigned int wcharcnt = 0;

            while (strlength > 0)
            {
                wcharbuf[wcharcnt] = utf82uc(str,stridx,charcnt);

                stridx += charcnt;
                strlength -= charcnt;
                wcharcnt++;
            }

            return std::wstring(wcharbuf,wcharcnt);
        }

        /**
         *
         * UTF8 Conversion Utitilities
         *
         * When conversion fails, an empty string is returned.
         *
         */

        std::string wtoutf8 (const std::wstring &wstr)
        {
#if defined(_WIN32) && _MSC_VER >= 1600
            std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;

            return conv.to_bytes(wstr);
#else
            return StringUtils::UTF8::ws2utf8(wstr);
#endif
        }

        std::wstring utf8tow(const std::string &str)
        {
#if defined(_WIN32) && _MSC_VER >= 1600

            std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;

            return conv.from_bytes(str);
#else
            return StringUtils::UTF8::utf82ws(str);
#endif
        }

    } // namespace StringUtils

}  // namespace CMNSTRM


