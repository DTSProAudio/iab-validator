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

#ifndef COMMON_STREAM_SRC_UTILS_PCMUTILITIES_H_
#define COMMON_STREAM_SRC_UTILS_PCMUTILITIES_H_

#if defined(_MSC_VER)
#define CMNSTRM_INLINE __inline
#else
// could also be static __inline__ __attribute__((always_inline)) or just static __inline__
#define CMNSTRM_INLINE static inline
#endif

#define KNoError 0

#ifdef __cplusplus
extern "C" {
#endif

#include "Namespace.h"

namespace CommonStream
{
    /**************************************************************************
     *
     * PCM conversion utilities
     *
     * Utility functions to convert between integer and floating point PCM
     * representations.
     * <em> Big_Endian byte order is assumed, unless otherwise stated.</em>
     *
     * @defgroup pcmutilities MDA PCM Utilities/Helper functions
     *
     *****************************************************************************/

    /**
     * Converts 24-bit PCM samples to 32-bit native integer samples.
     *
     * @param pDest Pointer to a buffer of pCount int samples.
     * @param pSrc Pointer to a buffer of pCount PCM24 samples.
     * @param pCount Number of samples to convert.
     * @return KNoError if no errors occurred.
     *
     *
     * @ingroup pcmutilities
     */

    CMNSTRM_INLINE int PCM24ToInt32(int *pDest, char *pSrc, unsigned int pCount)
    {
        unsigned int i;

        unsigned char *lSrc = (unsigned char *) pSrc;

        for(i = 0; i < pCount; i++)
        {
            *pDest  = *(lSrc++) << 24;
            *pDest |= *(lSrc++) << 16;
            *pDest |= *(lSrc++) << 8;

            pDest++;
        }

        return KNoError;
    }

    /**
     * Stores the value a single 24-bit PCM big-endian sample in a native int
     *
     * @param pSrc Pointer to a PCM24 big-endian sample.
     * @return pDest Native int sample.
     *
     * @ingroup pcmutilities
     */

    CMNSTRM_INLINE int OnePCM24ToInt32(unsigned char *pSrc)
    {
        int tmp = 0;

        tmp  = *(pSrc++) << 24;
        tmp |= *(pSrc++) << 16;
        tmp |= *(pSrc++) << 8;

        return tmp;
    }

    /**
     * Converts 24-bit PCM samples to 32-bit Int samples, stored specifically in <b>
     * Little-Endian </b> byte order.
     *
     * @param pDest Pointer to a buffer of pCount int samples, in <b>Little-Endian </b> byte order.
     * @param pSrc Pointer to a buffer of pCount PCM24 samples.
     * @param pCount Number of samples to convert.
     * @return KNoError if no errors occurred.
     *
     *
     * @ingroup pcmutilities
     */

    CMNSTRM_INLINE int PCM24ToInt32_LE(int *pDest, char *pSrc, unsigned int pCount)
    {
        unsigned int i = 0;

        unsigned char *lSrc = (unsigned char *) pSrc;

        for(i = 0; i < pCount; i++)
        {
            *pDest  = *(lSrc++) << 8;
            *pDest |= *(lSrc++) << 16;
            *pDest |= *(lSrc++) << 24;

            pDest++;
        }

        return KNoError;
    }


    /**
     * Converts 24-bit PCM samples to 32-bit Int samples, stored specifically in <b>
     * Little-Endian </b> byte order. Then right justify the samples, using
     * sign extension to pad. So a 24 bit sample
     * AA BB FF
     * is converted to
     * FF FF BB AA
     *
     * @param pDest Pointer to a buffer of pCount int samples, in <b>Little-Endian </b> byte order.
     * @param pSrc Pointer to a buffer of pCount PCM24 samples.
     * @param pCount Number of samples to convert.
     * @return KNoError if no errors occurred.
     *
     *
     * @ingroup pcmutilities
     */

    CMNSTRM_INLINE int PCM24ToInt32_LE_RJ_SE(int *pDest, char *pSrc, unsigned int pCount)
    {
        unsigned int i = 0;

        unsigned char *lSrc = (unsigned char *) pSrc;

        for(i = 0; i < pCount; i++)
        {
            *pDest  = *(lSrc++) << 8;
            *pDest |= *(lSrc++) << 16;
            *pDest |= *(lSrc++) << 24;

            *pDest >>= 8;

            pDest++;
        }

        return KNoError;
    }

    /**
     * Converts 32-bit PCM samples to 32-bit native integer samples.
     *
     * @param pDest Pointer to a buffer of pCount int samples.
     * @param pSrc Pointer to a buffer of pCount PCM32 samples.
     * @param pCount Number of samples to convert.
     * @return KNoError if no errors occurred.
     *
     *
     * @ingroup pcmutilities
     */

    CMNSTRM_INLINE int PCM32ToInt32(int *pDest, char *pSrc, unsigned int pCount)
    {
        unsigned int i = 0;

        unsigned char *lSrc = (unsigned char *) pSrc;

        for(i = 0; i < pCount; i++)
        {
            *pDest  = *(lSrc++) << 24;
            *pDest |= *(lSrc++) << 16;
            *pDest |= *(lSrc++) << 8;
            *pDest |= *(lSrc++) << 0;

            pDest++;
        }

        return KNoError;

    }

    /**
     * Converts 32-bit native interger samples to 32-bit PCM samples.
     *
     * @param pDest Pointer to a buffer of pCount PCM32 samples.
     * @param pSrc Pointer to a buffer of pCount int samples.
     * @param pCount Number of samples to convert.
     * @return KNoError if no errors occurred.
     *
     * @ingroup pcmutilities
     */

    CMNSTRM_INLINE int Int32ToPCM32(char *pDest, int *pSrc, unsigned int pCount)
    {
        unsigned int i = 0;

        unsigned char *lDest = (unsigned char *) pDest;

        for(i = 0; i < pCount; i++)
        {
            *(lDest++) = (*pSrc >> 24) & 0x000000FF;
            *(lDest++) = (*pSrc >> 16) & 0x000000FF;
            *(lDest++) = (*pSrc >> 8)  & 0x000000FF;
            *(lDest++) = (*pSrc >> 0)  & 0x000000FF;

            pSrc++;
        }

        return KNoError;
    }


    /**
     * Converts 32-bit native interger samples to 24-bit PCM samples.
     *
     * @param pDest Pointer to a buffer of pCount PCM24 samples.
     * @param pSrc Pointer to a buffer of pCount int samples.
     * @param pCount Number of samples to convert.
     * @return KNoError if no errors occurred.
     *
     * @ingroup pcmutilities
     */

    CMNSTRM_INLINE int Int32ToPCM24(char *pDest, int *pSrc, unsigned int pCount)
    {
        unsigned int i = 0;

        unsigned char *lDest = (unsigned char *) pDest;

        for(i = 0; i < pCount; i++)
        {
            *(lDest++) = (*pSrc >> 24) & 0x000000FF;
            *(lDest++) = (*pSrc >> 16) & 0x000000FF;
            *(lDest++) = (*pSrc >> 8)  & 0x000000FF;

            pSrc++;
        }

        return KNoError;
    }

    /**
     * Converts 32-bit PCM samples to 24-bit PCM samples.
     *
     * @param pDest Pointer to a buffer of pCount PCM24 samples.
     * @param pSrc Pointer to a buffer of pCount PCM32 samples.
     * @param pCount Number of samples to convert.
     * @return KNoError if no errors occurred.
     *
     * @ingroup pcmutilities
     */

    CMNSTRM_INLINE int PCM32ToPCM24(char *pDest, char *pSrc, unsigned int pCount)
    {
        unsigned int i = 0;

        unsigned char *lSrc = (unsigned char *) pSrc;
        unsigned char *lDest = (unsigned char *) pDest;

        for(i = 0; i < pCount; i++)
        {
            *lDest++  = *lSrc++;								// MSB
            *lDest++  = *lSrc++;								// Mid-SB
            *lDest++  = *lSrc++;								// LSB

            lSrc++;												// Lowest byte of 32-bit discarded, Upper 3 of 4 bytes used
        }

        return KNoError;

    }

    /**
     * Converts 24-bit PCM samples to float samples.
     *
     * @param pDest Pointer to a buffer of pCount float samples.
     * @param pSrc Pointer to a buffer of pCount PCM24 samples.
     * @param pCount Number of samples to convert.
     * @return KNoError if no errors occurred.
     *
     *
     * @ingroup pcmutilities
     */

    CMNSTRM_INLINE int PCM24ToFloat(float *pDest, char *pSrc, unsigned int pCount)
    {
        unsigned int    i = 0;

        int             val = 0;

        unsigned char *lSrc = (unsigned char *) pSrc;

        for(i = 0; i < pCount; i++)
        {
            val  = *(lSrc++) << 24;
            val |= *(lSrc++) << 16;
            val |= *(lSrc++) << 8;

            *(pDest++) = (float) val / 0x7fffffff;
        }

        return KNoError;

    }

    /**
     * Converts 32-bit PCM samples to float samples.
     *
     * @param pDest Pointer to a buffer of pCount float samples.
     * @param pSrc Pointer to a buffer of pCount PCM32 samples.
     * @param pCount Number of samples to convert.
     * @return KNoError if no errors occurred.
     *
     *
     * @ingroup pcmutilities
     */

    CMNSTRM_INLINE int PCM32ToFloat(float *pDest, char *pSrc, unsigned int pCount)
    {
        unsigned int    i = 0;

        int             val = 0;

        unsigned char *lSrc = (unsigned char *) pSrc;

        for(i = 0; i < pCount; i++)
        {
            val  = *(lSrc++) << 24;
            val |= *(lSrc++) << 16;
            val |= *(lSrc++) << 8;
            val |= *(lSrc++) << 0;

            *(pDest++) = (float) val / 0x7fffffff;
        }

        return KNoError;

    }

} // namespace CommonStream

#ifdef __cplusplus
}
#endif

#endif // COMMON_STREAM_SRC_UTILS_PCMUTILITIES_H_
