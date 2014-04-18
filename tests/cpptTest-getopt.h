/*
    Copyright © 2013-2014 Nicolas Normand <nnormand@justcodeit.org>. All Rights Reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

    3. The name of the author may not be used to endorse or promote products derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Nicolas Normand "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef CPPT_TESTS_STRING_H_
#define CPPT_TESTS_STRING_H_

#include "cppmt-getopt.h"
#include <cppunit/extensions/HelperMacros.h>

class cpptTestGetOpt: public CppUnit::TestFixture
{
	public:
	CPPUNIT_TEST_SUITE( cpptTestGetOpt );

	CPPUNIT_TEST( testOptFlags   );
	CPPUNIT_TEST( testOptArgs    );
	CPPUNIT_TEST( testSameOpt    );
	CPPUNIT_TEST( testOnlyLong   );
	CPPUNIT_TEST( testOnlyShort  );
	CPPUNIT_TEST( testConfigFile );

	CPPUNIT_TEST_SUITE_END();

	void testOptFlags();
	void testOptArgs();
	void testSameOpt();
	void testOnlyLong();
	void testOnlyShort();
	void testConfigFile();
};


#endif

