/*
    Copyright © 2013-2015 Nicolas Normand <nnormand@justcodeit.org>. All Rights Reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

    3. The name of the author may not be used to endorse or promote products derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Nicolas Normand "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "cpptTest-getenv.h"
#include "cppmt-env.h"
#include <iostream>

CPPUNIT_TEST_SUITE_REGISTRATION( cpptTestGetEnv );

void cpptTestGetEnv::testEnv()
{
	int res = -10;
	cppmt::getEnv::status_t st;
	unsetenv("LIBCPPMT_TESTING_1"); // Should not happen, but just to be sure.

	// Tests with environment unset:
	{
		// Environment does not exist: using default value to int()
		st = cppmt::getEnv::check<int>("LIBCPPMT_TESTING_1", &res);
		CPPUNIT_ASSERT(res == 0);
		CPPUNIT_ASSERT(st.wasNull == true);

		// Environment does not exist; using explicit default value
		st = cppmt::getEnv::check<int>("LIBCPPMT_TESTING_1", &res, 33);
		CPPUNIT_ASSERT(res == 33);
		CPPUNIT_ASSERT(st.wasNull == true);

		// Environment does not exist; using the 'safe' method
		CPPUNIT_ASSERT(cppmt::getEnv::safe<int>("LIBCPPMT_TESTING_1") == 0);
		CPPUNIT_ASSERT(cppmt::getEnv::safe<int>("LIBCPPMT_TESTING_1", -15) == -15);

		
		// Now the same tests, using a bounded range
		st = cppmt::getEnv::checkBounded<int>("LIBCPPMT_TESTING_1", &res, 10, 15);
		CPPUNIT_ASSERT(st.wasNull == true);
		CPPUNIT_ASSERT(res == 0); // Seems unnatural; maybe the defaultValue can be outbounds ?

		st = cppmt::getEnv::checkBounded<int>("LIBCPPMT_TESTING_1", &res, 10, 15, -10);
		CPPUNIT_ASSERT(st.wasNull == true);
		CPPUNIT_ASSERT(res == -10); // Seems unnatural; maybe the defaultValue can be outbounds ?


		CPPUNIT_ASSERT(cppmt::getEnv::safeBounded("LIBCPPMT_TESTING_1", 10, 15) == 0);
		CPPUNIT_ASSERT(cppmt::getEnv::safeBounded("LIBCPPMT_TESTING_1", 10, 15, -10) == -10);
	}

	// Tests with environment set with invalid value
	{
		setenv("LIBCPPMT_TESTING_1", "Not_a_Number", 1);

		// Environment does not exist: using default value to int()
		st = cppmt::getEnv::check<int>("LIBCPPMT_TESTING_1", &res);
		CPPUNIT_ASSERT(res == 0);
		CPPUNIT_ASSERT(st.wasNull == false);
		CPPUNIT_ASSERT(st.hasError == true);

		// Environment does not exist; using explicit default value
		st = cppmt::getEnv::check<int>("LIBCPPMT_TESTING_1", &res, 33);
		CPPUNIT_ASSERT(res == 33);
		CPPUNIT_ASSERT(st.wasNull == false);
		CPPUNIT_ASSERT(st.hasError == true);

		// Environment does not exist; using the 'safe' method
		CPPUNIT_ASSERT(cppmt::getEnv::safe<int>("LIBCPPMT_TESTING_1") == 0);
		CPPUNIT_ASSERT(cppmt::getEnv::safe<int>("LIBCPPMT_TESTING_1", -15) == -15);

		
		// Now the same tests, using a bounded range
		st = cppmt::getEnv::checkBounded<int>("LIBCPPMT_TESTING_1", &res, 10, 15);
		CPPUNIT_ASSERT(st.wasNull == false);
		CPPUNIT_ASSERT(st.hasError == true);
		CPPUNIT_ASSERT(res == 0); // Seems unnatural; maybe the defaultValue can be outbounds ?

		st = cppmt::getEnv::checkBounded<int>("LIBCPPMT_TESTING_1", &res, 10, 15, -10);
		CPPUNIT_ASSERT(st.wasNull == false);
		CPPUNIT_ASSERT(st.hasError == true);
		CPPUNIT_ASSERT(res == -10); // Seems unnatural; maybe the defaultValue can be outbounds ?


		CPPUNIT_ASSERT(cppmt::getEnv::safeBounded("LIBCPPMT_TESTING_1", 10, 15) == 0);
		CPPUNIT_ASSERT(cppmt::getEnv::safeBounded("LIBCPPMT_TESTING_1", 10, 15, -10) == -10);
	}

	// Tests with environment set with good value
	{
		setenv("LIBCPPMT_TESTING_1", "13", 1);

		// Environment does not exist: using default value to int()
		st = cppmt::getEnv::check<int>("LIBCPPMT_TESTING_1", &res);
		CPPUNIT_ASSERT(res == 13);
		CPPUNIT_ASSERT(st.wasNull == false);
		CPPUNIT_ASSERT(st.hasError == false);

		// Environment does not exist; using explicit default value
		st = cppmt::getEnv::check<int>("LIBCPPMT_TESTING_1", &res, 33);
		CPPUNIT_ASSERT(res == 13);
		CPPUNIT_ASSERT(st.wasNull == false);
		CPPUNIT_ASSERT(st.hasError == false);

		// Environment does not exist; using the 'safe' method
		CPPUNIT_ASSERT(cppmt::getEnv::safe<int>("LIBCPPMT_TESTING_1") == 13);
		CPPUNIT_ASSERT(cppmt::getEnv::safe<int>("LIBCPPMT_TESTING_1", -15) == 13);

		
		// Now the same tests, using a bounded range
		st = cppmt::getEnv::checkBounded<int>("LIBCPPMT_TESTING_1", &res, 10, 15);
		CPPUNIT_ASSERT(st.wasNull == false);
		CPPUNIT_ASSERT(st.hasError == false);
		CPPUNIT_ASSERT(st.wasOutBound == false);
		CPPUNIT_ASSERT(res == 13);

		st = cppmt::getEnv::checkBounded<int>("LIBCPPMT_TESTING_1", &res, 10, 15, -10);
		CPPUNIT_ASSERT(st.wasNull == false);
		CPPUNIT_ASSERT(st.hasError == false);
		CPPUNIT_ASSERT(st.wasOutBound == false);
		CPPUNIT_ASSERT(res == 13);


		CPPUNIT_ASSERT(cppmt::getEnv::safeBounded("LIBCPPMT_TESTING_1", 10, 15) == 13);
		CPPUNIT_ASSERT(cppmt::getEnv::safeBounded("LIBCPPMT_TESTING_1", 10, 15, -10) == 13);

		// Now check for outer bounds

		st = cppmt::getEnv::checkBounded<int>("LIBCPPMT_TESTING_1", &res, -10, 10);
		CPPUNIT_ASSERT(st.wasNull == false);
		CPPUNIT_ASSERT(st.hasError == false);
		CPPUNIT_ASSERT(st.wasOutBound == true);
		CPPUNIT_ASSERT(res == 10);

		st = cppmt::getEnv::checkBounded<int>("LIBCPPMT_TESTING_1", &res, 30, 99);
		CPPUNIT_ASSERT(st.wasNull == false);
		CPPUNIT_ASSERT(st.hasError == false);
		CPPUNIT_ASSERT(st.wasOutBound == true);
		CPPUNIT_ASSERT(res == 30);

		CPPUNIT_ASSERT(cppmt::getEnv::safeBounded<int>("LIBCPPMT_TESTING_1", 30, 99) == 30);
		CPPUNIT_ASSERT(cppmt::getEnv::safeBounded<int>("LIBCPPMT_TESTING_1", -30, 10) == 10);
	}


}

