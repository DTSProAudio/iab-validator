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

#ifndef COMMON_STREAM_SRC_HASH_HASHGENERATOR_H_
#define COMMON_STREAM_SRC_HASH_HASHGENERATOR_H_

// Inclusion of <stdint.h> (instead of <cstdin>) for C++98 backward compatibility.
#include <stdint.h>
#include "Namespace.h"

namespace CommonStream
{

    /**
     * @enum EHashType
     *
     * @brief Available byte based hash algorithms.
     *
     * @note Append only to the end and set eHashType_Maximum as the last valid hash.
     *
     */
    enum EHashType
    {
        eHashType_Minimum = 0,
        eHashType_None = eHashType_Minimum,
        eHashType_MD5 = 1,
        eHashType_Maximum = eHashType_MD5
    };


    /**
     * @brief HashGenerator is the interface for all hash algorithms used by the BitStreamReader and BitStreamWriter objects.
     *
     *
     */
    class HashGenerator
    {
    public:

        /**
         * @enum EHashState
         *
         * @brief Represents the states of the hash class
         *
         * @note Valid state transitions:
         *          1) Stopped -> Stopped
         *          2) Stopped -> Started
         *          3) Started -> Started
         *          4) Started -> Paused
         *          5) Started -> Stopped
         *          6) Paused -> Paused
         *          7) Paused -> Started
         *          8) Paused -> Stopped
         */
        enum EHashState
        {
            eHashState_Stopped = 0,
            eHashState_Started,
            eHashState_Paused
        };

        /**
         * @brief Destructor.
         *
         */
        virtual ~HashGenerator() {}

        /**
         * @brief Returns the type of hash.
         *
         */
        virtual EHashType getHashType(void) = 0;

        /**
         * @brief Returns the state of the hash class.
         *
         */
        virtual EHashState getState(void) = 0;

        /**
         * @brief Returns if a hash algorithm is currently being used (accumulated).
         *
         */
        virtual bool useHash(void) = 0;

        /**
         * @brief Starts the acculuation of a hash.
         *
         */
        virtual bool start(void) = 0;

        /**
         * @brief Ends the acculuation of a hash.
         *
         * @note Potentially computes the final hash with additional processing.
         *
         * @return true if hash is stopped and successfully computed.
         */
        virtual bool stop(void) = 0;

        /**
         * @brief Pauses the acculuation of a hash.
         *
         * @return true if hash is stopped and successfully computed.
         */
        virtual bool pause(void) = 0;

        /**
         * @brief Resume the acculuation of a hash.
         *
         * @return true if hash is stopped and successfully computed.
         */
        virtual bool resume(void) = 0;

        /**
         * @brief Adds a value to the hash algorithm.
         *
         * @param iValue pointer to a buffer of data to be hashed.
         * @param iByteCount number of bytes stored in iValue
         * @return true if hash is properly added.
         *
         */
        virtual bool accumulate(uint8_t *iValue, uint32_t iByteCount) = 0;

        /**
         * @brief Size in bytes of final hash.
         *
         */
        virtual uint32_t getHashSize(void) = 0;

        /**
         * @brief Provides an interface for getting the contents of the computed hash.
         *
         * @note oHash must be large enough to store the hash in.
         *       It must be the same or larger than getHashSize.
         *
         * @param oHash pointer to a buffer of data to be hashed.
         * @return true if hash is properly set to oHash.
         *
         */
        virtual bool getHash(uint8_t *oHash) = 0;
    };

}  // namespace CommonStream

#endif // COMMON_STREAM_SRC_HASH_HASHGENERATOR_H_
