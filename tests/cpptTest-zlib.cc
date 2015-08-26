/*
    Copyright © 2013-2014 Nicolas Normand <nnormand@justcodeit.org>. All Rights Reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

    3. The name of the author may not be used to endorse or promote products derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Nicolas Normand "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "cpptTest-zlib.h"
#include "cppmt-exceptions.h"
#include <iostream>

CPPUNIT_TEST_SUITE_REGISTRATION( cpptTestZlib );

void cpptTestZlib::testDeflate()
{
	try {
		cppmt::deflate_handler hdl(-5);
		CPPUNIT_ASSERT_MESSAGE("Compression level watch guard failure.", false);
	} catch(const cppmt::exception& e) {
	}

	try {
		cppmt::deflate_handler hdl(-1, 64);
	} catch(const cppmt::exception& e) {
		CPPUNIT_ASSERT_MESSAGE("Compression level watch guard failure.", false);
	}

	try {
		cppmt::deflate_handler hdl(6, 0);
		CPPUNIT_ASSERT_MESSAGE("Internal buffer watch guard failure.", false);
	} catch(const cppmt::exception& e) {
	}

	cppmt::deflate_handler hdl1(9,  512);
	cppmt::deflate_handler hdl2(5, 8182);
	cppmt::deflate_handler hdl3(6,    8);

	std::ifstream is("data/Bare_mountain,_nevada.jpg");
	CPPUNIT_ASSERT(is.is_open());

	std::string orig_input;
	std::stringstream ss1;
	std::stringstream ss2;
	std::stringstream ss3;

	int status1;
	int status2;
	int status3;
	do {
		char buf[4096];
		is.read(buf, 4096);
		size_t rd_sz = is.gcount();

		std::string in(buf, rd_sz);
		std::string out;
		std::string tmp;
		orig_input += in;

		if (rd_sz > 0) {
			status1 = hdl1.compress(out,  in          );
				  hdl2.compress(ss2,  in, &status2);
			tmp     = hdl3.compress(      in, &status3);

			CPPUNIT_ASSERT(status1 == Z_OK);
			CPPUNIT_ASSERT(status2 == Z_OK);
			CPPUNIT_ASSERT(status3 == Z_OK);

			ss1 << out;
			ss3 << tmp;
		}

		if (is.eof() == true) {
			out = "";

			status1 = hdl1.compress(out,  "",           true);
				  hdl2.compress(ss2,  "", &status2, true);
			tmp     = hdl3.compress(      "", &status3, true);

			CPPUNIT_ASSERT(status1 == Z_STREAM_END);
			CPPUNIT_ASSERT(status2 == Z_STREAM_END);
			CPPUNIT_ASSERT(status3 == Z_STREAM_END);

			ss1 << out;
			ss3 << tmp;
			break;
		}
	} while(true);

	std::string tmp[3];
	cppmt::inflate_handler ih1(8192*16);
	cppmt::inflate_handler ih2(8192*16);
	cppmt::inflate_handler ih3(8192*16);

	tmp[0] = ih1.decompress(ss1.str(), &status1, false);
	tmp[1] = ih2.decompress(ss2.str(), &status2, false);
	tmp[2] = ih3.decompress(ss3.str(), &status3, false);

	tmp[0] += ih1.decompress("", &status1, true);
	tmp[1] += ih2.decompress("", &status2, true);
	tmp[2] += ih3.decompress("", &status3, true);
			
	CPPUNIT_ASSERT(status1 == Z_STREAM_END);
	CPPUNIT_ASSERT(status2 == Z_STREAM_END);
	CPPUNIT_ASSERT(status3 == Z_STREAM_END);

	CPPUNIT_ASSERT(tmp[0] == orig_input);
	CPPUNIT_ASSERT(tmp[1] == orig_input);
	CPPUNIT_ASSERT(tmp[2] == orig_input);

}

