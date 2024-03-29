/*
    Copyright © 2016 Nicolas Normand <nnormand@justcodeit.org>. All Rights Reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

    3. The name of the author may not be used to endorse or promote products derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Nicolas Normand "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "cpptTest-endianness.h"
#include <iostream>

CPPUNIT_TEST_SUITE_REGISTRATION( cpptTestEndianness );

void cpptTestEndianness::testBytesSwap()
{
	CPPUNIT_ASSERT( cppmt::bytesSwap( static_cast<uint8_t >(0xf1              ) ) ==               0xf1 );
	CPPUNIT_ASSERT( cppmt::bytesSwap( static_cast<uint16_t>(0xf1e2            ) ) ==             0xe2f1 );
	CPPUNIT_ASSERT( cppmt::bytesSwap( static_cast<uint32_t>(0xf1e2d3c4        ) ) ==         0xc4d3e2f1 );
	CPPUNIT_ASSERT( cppmt::bytesSwap( static_cast<uint64_t>(0xf1e2d3c4b5a69788) ) == 0x8897a6b5c4d3e2f1 );
}

