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

#ifndef COMMON_STREAM_SRC_HASH_CRCGENERATOR_H_
#define COMMON_STREAM_SRC_HASH_CRCGENERATOR_H_

// Inclusion of <stdint.h> (instead of <cstdin>) for C++98 backward compatibility.
#include <stdint.h>
#include "Namespace.h"

namespace CommonStream
{

    /**
     * @enum ECRCType
     *
     * @brief Available byte based CRC algorithms.
     *
     * @note Append only to the end and set eCRCType_Maximum as the last valid CRC.
     *
     */
    enum ECRCType
    {
        eCRCType_Minimum = 0,
        eCRCType_None = eCRCType_Minimum,
        eCRCType_CRC16 = 1,
        eCRCType_Maximum = eCRCType_CRC16
    };


    /**
     * @brief CRCGenerator is the interface for all CRC algorithms used by the BitStreamReader and BitStreamWriter objects.
     *
     *
     */
    class CRCGenerator
    {
    public:

        /**
         * @enum ECRCState
         *
         * @brief Represents the states of the crc class
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
         *
         */
        enum ECRCState
        {
            eCRCState_Stopped = 0,
            eCRCState_Started,
            eCRCState_Paused
        };

        /**
         * @brief Destructor.
         *
         */
        virtual ~CRCGenerator() {}

        /**
         * @brief Returns the type of CRC.
         *
         */
        virtual ECRCType getCRCType(void) = 0;

        /**
         * @brief Returns the state of the hash class.
         *
         */
        virtual ECRCState getState(void) = 0;

        /**
         * @brief Returns if a CRC algorithm is currently being used (accumulated).
         *
         */
        virtual bool useCRC(void) = 0;

        /**
         * @brief Starts the acculuation of a CRC.
         *
         */
        virtual bool start(void) = 0;

        /**
         * @brief Ends the acculuation of a CRC.
         *
         * @note Potentially computes the final CRC with additional processing.
         *
         * @return true if CRC is stopped and successfully computed.
         */
        virtual bool stop(void) = 0;

        /**
         * @brief Pauses the acculuation of a hash.
         *
         * @return true if hash is stopped and successfully computed.
         */
        virtual bool pause(void) = 0;

        /**
         * @brief Resumes the acculuation of a hash.
         *
         * @return true if hash is stopped and successfully computed.
         */
        virtual bool resume(void) = 0;

        /**
         * @brief Adds a value to the CRC algorithm.
         *
         * @param iBits data to be CRCed.
         * @param iBitCount number of bytes stored in iValue
         * @return true if CRC is properly added.
         *
         */
        virtual bool accumulate(uint32_t iBits, uint8_t iBitCount) = 0;
        virtual bool accumulate(uint64_t iBits, uint8_t iBitCount) = 0;

        /**
         * @brief Size in bytes of final CRC.
         *
         */
        virtual uint32_t getCRCRegisterSize(void) = 0;

        /**
         * @brief Provides an interface for getting the contents of the computed CRC.
         *
         * @note oCRC must be large enough to store the CRC in.
         *       It must be the same or larger than getCRCSize.
         *
         * @param oCRC pointer to a buffer of data to be CRCed.
         * @return true if CRC is properly set to oCRC.
         *
         */
        virtual bool getCRCRegister(uint8_t *oCRC) = 0;

    };

}  // namespace CommonStream

#endif // COMMON_STREAM_SRC_HASH_CRCGENERATOR_H_
