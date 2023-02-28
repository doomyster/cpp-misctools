/*
    Copyright © 2016 Nicolas Normand <nnormand@justcodeit.org>. All Rights Reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

    3. The name of the author may not be used to endorse or promote products derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Nicolas Normand "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef CPPMT_ENDIANNESS_H_
#define CPPMT_ENDIANNESS_H_

#include <stdint.h>

namespace cppmt
{

typedef enum
{
	E_UNKNOWN = -1,
	E_BIG_ENDIAN,
	E_LITTLE_ENDIAN
} endian_t;

endian_t getHostEndianness();

// Swaps the endianness
uint8_t  bytesSwap(uint8_t  x);
uint16_t bytesSwap(uint16_t x);
uint32_t bytesSwap(uint32_t x);
uint64_t bytesSwap(uint64_t x);

// if host is not in big endian, swaps the endianness; Does nothing if endianness is unknown
uint8_t  toBigEndian(uint8_t  x);
uint16_t toBigEndian(uint16_t x);
uint32_t toBigEndian(uint32_t x);
uint64_t toBigEndian(uint64_t x);

// Consider the input as big endian, and if the host in in little endian, swaps the endianness; Does nothing if endianness is unknown
uint8_t  fromBigEndian(uint8_t  x);
uint16_t fromBigEndian(uint16_t x);
uint32_t fromBigEndian(uint32_t x);
uint64_t fromBigEndian(uint64_t x);

} // End of: namespace cppmt

#endif

