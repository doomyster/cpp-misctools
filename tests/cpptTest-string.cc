/*
    Copyright © 2013-2014 Nicolas Normand <nnormand@justcodeit.org>. All Rights Reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

    3. The name of the author may not be used to endorse or promote products derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Nicolas Normand "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "cpptTest-string.h"
#include <iostream>

CPPUNIT_TEST_SUITE_REGISTRATION( cpptTestString );

class obj
{
	public:
	int o;
	float b;
	bool j;

	obj(): o(0), b(5.75), j(false) {}
};

std::ostream& operator<< (std::ostream& os, const obj& o)
{
	os << "{ " << o.o << ", " << o.b << ", ";
	if (o.j) {
		os << 'T';
	}
	else {
		os << 'F';
	}
	return os << "}";
}

void cpptTestString::testToString()
{
	cppmt::string s1;
	cppmt::string s2;
	cppmt::string s3;
	cppmt::string s4;

	obj o;

	s1 = cppmt::toString(35l);
	s2 = cppmt::toString("alreadyString");
	s3 = cppmt::toString( cppmt::string("some\0binary\0string", 18) );
	s4 = cppmt::toString( o );

	CPPUNIT_ASSERT(s1 == "35");
	CPPUNIT_ASSERT(s2 == "alreadyString");
	CPPUNIT_ASSERT(s3 == cppmt::string("some\0binary\0string", 18) );
	CPPUNIT_ASSERT(s4 == "{ 0, 5.75, F}");
}

template <typename T>
class conv_table_t
{
	public:
	cppmt::string str_value;
	T    expected_value;
	bool expected_result;
	bool unknown_value;

	conv_table_t(const cppmt::string& sv, T ev, bool er, bool u): str_value(sv), expected_value(ev), expected_result(er), unknown_value(u) {}
};

typedef conv_table_t<         int> table_int_t;
typedef conv_table_t<unsigned int> table_uint_t;

static std::vector<table_int_t>& getTestTable_int()
{
	static std::vector<table_int_t> t;
	static bool first = true;

	if (first) {
		t.push_back(table_int_t("15",  15, true,  false));
		t.push_back(table_int_t("15a", 15, false, false));
		t.push_back(table_int_t("a15",  0, false, true ));
		t.push_back(table_int_t("-5",  -5, true,  false));
		t.push_back(table_int_t("0xff", 0, false, false));
		first = false;
	}

	return t;
}

static std::vector<table_uint_t>& getTestTable_uint()
{
	static std::vector<table_uint_t> t;
	static bool first = true;

	if (first) {
		t.push_back(table_uint_t("15",                 15,  true,  false));
		t.push_back(table_uint_t("15a",                15,  false, false));
		t.push_back(table_uint_t("a15",                 0,  false, true ));
		t.push_back(table_uint_t("-1",  (unsigned int)(-1), true,  false));
		first = false;
	}

	return t;
}

void cpptTestString::testFromString()
{
	std::vector<table_int_t > t = getTestTable_int();
	std::vector<table_uint_t> T = getTestTable_uint();


	std::vector<table_int_t>::iterator it;
	for (it = t.begin(); it != t.end(); ++it) {
		int res1;
		int res2;
		int res3;
		bool suc1 = false;
		bool suc2 = false;

		res1 = cppmt::fromString<int>(it->str_value, &suc1);
		suc2 = cppmt::fromString(res2, it->str_value);
		res3 = cppmt::fromString<int>(it->str_value); // Makes sure no segfault when NULL as second argument

		if (it->unknown_value == false) {
			CPPUNIT_ASSERT(res1 == it->expected_value);
			CPPUNIT_ASSERT(res2 == it->expected_value);
			CPPUNIT_ASSERT(res3 == it->expected_value);
		}
		CPPUNIT_ASSERT(suc1 == it->expected_result);
		CPPUNIT_ASSERT(suc2 == it->expected_result);
	}

	std::vector<table_uint_t>::iterator uit;
	for (uit = T.begin(); uit != T.end(); ++uit) {
		unsigned int res1;
		unsigned int res2;
		unsigned int res3;
		bool suc1 = false;
		bool suc2 = false;

		res1 = cppmt::fromString<unsigned int>(uit->str_value, &suc1);
		suc2 = cppmt::fromString(res2, uit->str_value);
		res3 = cppmt::fromString<unsigned int>(uit->str_value); // Makes sure no segfault when NULL as second argument

		if (uit->unknown_value == false) {
			CPPUNIT_ASSERT(res1 == uit->expected_value);
			CPPUNIT_ASSERT(res2 == uit->expected_value);
			CPPUNIT_ASSERT(res3 == uit->expected_value);
		}
		CPPUNIT_ASSERT(suc1 == uit->expected_result);
		CPPUNIT_ASSERT(suc2 == uit->expected_result);
	}

}

void cpptTestString::testEndsWith()
{
	CPPUNIT_ASSERT( cppmt::endsWith("the SKY",  "SKY") == true  );
	CPPUNIT_ASSERT( cppmt::endsWith("the SKY.", "SKY") == false );
	CPPUNIT_ASSERT( cppmt::endsWith(     "SKY", "SKY") == true  );
	CPPUNIT_ASSERT( cppmt::endsWith(     "YSK", "SKY") == false );
	CPPUNIT_ASSERT( cppmt::endsWith(     "S",   "SKY") == false );
	CPPUNIT_ASSERT( cppmt::endsWith(     "",    "SKY") == false );
}

void cpptTestString::testTrim()
{
	CPPUNIT_ASSERT( cppmt:: trim ("  haha  ")                  == "haha" );
	CPPUNIT_ASSERT( cppmt:: trim ("\t\n \vha\t\n \vha\t\n \v") == "ha\t\n \vha");
	CPPUNIT_ASSERT( cppmt:: trim ("    \t  \n  \v  ")          == "");

	CPPUNIT_ASSERT( cppmt::ltrim ("  haha  ")                  == "haha  " );
	CPPUNIT_ASSERT( cppmt::ltrim ("\t\n \vha\t\n \vha\t\n \v") == "ha\t\n \vha\t\n \v");
	CPPUNIT_ASSERT( cppmt::ltrim ("    \t  \n  \v  ")          == "");

	CPPUNIT_ASSERT( cppmt::rtrim ("  haha  ")                  == "  haha" );
	CPPUNIT_ASSERT( cppmt::rtrim ("\t\n \vha\t\n \vha\t\n \v") == "\t\n \vha\t\n \vha");
	CPPUNIT_ASSERT( cppmt::rtrim ("    \t  \n  \v  ")          == "");
}

