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

#ifndef COMMON_STREAM_SRC_RAWSTREAM_RAWSTREAM_H_
#define COMMON_STREAM_SRC_RAWSTREAM_RAWSTREAM_H_

#include <fstream>

#include "Namespace.h"
#include "StreamDefines.h"

namespace CommonStream
{

    class RawStream
    {
    public:
        
        RawStream();
        RawStream(char *buf, BitCount_t buf_size); ///< construct with outside storage
        RawStream(const char *buf, BitCount_t buf_size); ///< construct with outside storage
        RawStream(BitCount_t buf_size); ///< construct with own storage
        RawStream(const RawStream &stream2); ///< copy constructor
        RawStream &operator=(const RawStream &stream2);
        ~RawStream();
        ReturnCode Init();
        ReturnCode Init(char *buf, BitCount_t buf_size); ///< init with outside storage
        ReturnCode Init(const char *buf, BitCount_t buf_size); ///< init with outside storage
        ReturnCode Init(BitCount_t buf_size); ///< init with own storage
        ReturnCode Init(const RawStream &stream2); ///< Init with copy and own new storage
        ReturnCode InitReuse(const RawStream &stream2); ///< Init with copy, reusing storage if possible

        void Dealloc();
        // getters
        char *buffer() const
        {
            return buffer_;
        }
        BitCount_t buffer_size() const
        {
            return buffer_size_;
        }

        std::ios_base::iostate rdstate() const;
        std::ios_base::iostate setstate(std::ios_base::iostate st);
        void clear();
        bool good() const;
        bool fail() const;
        bool eof() const;
        bool bad() const;
        void reset();
        BitCount_t gcount() const;
        BitCount_t tell() const;
        ReturnCode seek(BitCount_t pos);
        ReturnCode seek(SBitCount_t offs, std::ios_base::seekdir way);

    protected:
    
        static const std::ios_base::iostate eofbit_ = static_cast<std::ios_base::iostate>(4);
        static const std::ios_base::iostate failbit_ = static_cast<std::ios_base::iostate>(2);
        static const std::ios_base::iostate badbit_ = static_cast<std::ios_base::iostate>(1);
        static const std::ios_base::iostate goodstate_ = static_cast<std::ios_base::iostate>(0);

        BitCount_t index_;
        char *buffer_;
        BitCount_t buffer_size_;
        BitCount_t gcount_;
        std::ios_base::iostate state_; // eof | fail | bad
        bool own_storage_;
    };
} // namespace CommonStream

#endif // COMMON_STREAM_SRC_RAWSTREAM_RAWSTREAM_H_
