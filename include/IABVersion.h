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
* Constants that contain the current IAB SDK version
*
* @file
*/

#ifndef __IABVERSION_H__
#define	__IABVERSION_H__

#define IAB_VERSION_MAJOR       0
#define IAB_VERSION_MINOR       1
#define IAB_VERSION_REVISION    0
#define IAB_VERSION_SUBREVISION 0

//
// https://gcc.gnu.org/onlinedocs/cpp/Stringification.html
#define STRINGIFY2(s) #s
#define STRINGIFY(s) STRINGIFY2(s)
#define WIDE_STRINGIFY2(s) L ## #s
#define WIDE_STRINGIFY(s) WIDE_STRINGIFY2(s)

//
// The following have been verified with toolchain preprocessors 'gcc -E' on
// OSX/Linux and 'cl /P /C' on Windows
//
// produces the string form: "M.m.r.s"
#define IAB_VERSION_STRING \
    STRINGIFY(IAB_VERSION_MAJOR.IAB_VERSION_MINOR.IAB_VERSION_REVISION.IAB_VERSION_SUBREVISION)

// produces the string form: L"M.m.r.s"
#define IAB_VERSION_STRING_WIDE \
    WIDE_STRINGIFY(IAB_VERSION_MAJOR.IAB_VERSION_MINOR.IAB_VERSION_REVISION.IAB_VERSION_SUBREVISION)

/******************************************** for Windows .rc2 version resources ***********************/
#ifdef _WIN32

#ifdef _UNICODE
#define WIN_STRFILEVER IAB_VERSION_STRING_WIDE
#else
#define WIN_STRFILEVER IAB_VERSION_STRING
#endif // _UNICODE

#define WIN_STRPRODUCTVER  WIN_STRFILEVER
#define WIN_FILEVER        IAB_VERSION_MAJOR,IAB_VERSION_MINOR,IAB_VERSION_REVISION,IAB_VERSION_SUBREVISION
#define WIN_PRODUCTVER     WIN_FILEVER

#endif // _WIN32


#endif // __IABVERSION_H__
