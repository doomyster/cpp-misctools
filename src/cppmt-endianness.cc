/*
    Copyright © 2016 Nicolas Normand <nnormand@justcodeit.org>. All Rights Reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

    3. The name of the author may not be used to endorse or promote products derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Nicolas Normand "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "cppmt-endianness.h"
#include <cstddef>

namespace cppmt
{

endian_t getHostEndianness_st()
{
	union {
		uint32_t i;
		char c[4];
	} v = {0x01020304};

	if (v.c[0] == 0x01 && v.c[1] == 0x02 && v.c[2] == 0x03 && v.c[3] == 0x04) {
		return E_BIG_ENDIAN;
	}
	else if (v.c[0] == 0x04 && v.c[1] == 0x03 && v.c[2] == 0x02 && v.c[3] == 0x01) {
		return E_LITTLE_ENDIAN;
	}
	else {
		return E_UNKNOWN;
	}
}

endian_t getHostEndianness()
{
	static endian_t x = getHostEndianness_st();
	return x;
}


template <typename T>
static T bytesSwap_st(T x)
{
	T r = 0;
	size_t len = sizeof(T);
	for (unsigned int i=0; i<len; ++i) {
		r |= ((x >> (i*8)) & 0xff) << ((len-i-1)*8);
	}
	return r;
}

uint8_t  bytesSwap(uint8_t  x) { return x; }
uint16_t bytesSwap(uint16_t x) { return bytesSwap_st(x); }
uint32_t bytesSwap(uint32_t x) { return bytesSwap_st(x); }
uint64_t bytesSwap(uint64_t x) { return bytesSwap_st(x); }

// if host is not in big endian, swaps the endianness; Does nothing if endianness is unknown
uint8_t  toBigEndian(uint8_t  x) { return x; }
uint16_t toBigEndian(uint16_t x) { if (getHostEndianness() == E_BIG_ENDIAN) { return x; } else { return bytesSwap(x); } }
uint32_t toBigEndian(uint32_t x) { if (getHostEndianness() == E_BIG_ENDIAN) { return x; } else { return bytesSwap(x); } }
uint64_t toBigEndian(uint64_t x) { if (getHostEndianness() == E_BIG_ENDIAN) { return x; } else { return bytesSwap(x); } }

// Consider the input as big endian, and if the host in in little endian, swaps the endianness; Does nothing if endianness is unknown
uint8_t  fromBigEndian(uint8_t  x) { return x; }
uint16_t fromBigEndian(uint16_t x) { if (getHostEndianness() == E_BIG_ENDIAN) { return x; } else { return bytesSwap(x); } }
uint32_t fromBigEndian(uint32_t x) { if (getHostEndianness() == E_BIG_ENDIAN) { return x; } else { return bytesSwap(x); } }
uint64_t fromBigEndian(uint64_t x) { if (getHostEndianness() == E_BIG_ENDIAN) { return x; } else { return bytesSwap(x); } }

} // End of: namespace cppmt

