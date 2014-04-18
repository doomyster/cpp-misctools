/*
    Copyright © 2013-2014 Nicolas Normand <nnormand@justcodeit.org>. All Rights Reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

    3. The name of the author may not be used to endorse or promote products derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Nicolas Normand "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "cpptTest-getopt.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <stdarg.h>

CPPUNIT_TEST_SUITE_REGISTRATION( cpptTestGetOpt );



class arg_t
{
	public:
	char** argv;
	int argc;

	public:
	arg_t(): argv(NULL), argc(0) {}
	arg_t(const char* arg0, ...)     { va_list va; va_start(va, arg0); init__(arg0, va); va_end(va); }
	void init(const char* arg0, ...) { va_list va; va_start(va, arg0); init__(arg0, va); va_end(va); }
	protected:
	void init__(const char* arg0, va_list va)
	{
		std::vector<cppmt::string> args;
		args.push_back(arg0);

		const char* a = va_arg(va, const char*);
		while (a != NULL) {
			args.push_back(a);
			a = va_arg(va, const char*);
		}

		argv = new char*[args.size() +1];
		for (unsigned int i=0; i<args.size(); ++i) {
			argv[i] = strdup(args[i].c_str());
		}

		argv[args.size()] = NULL;
		argc = args.size();
	}

	public:
	~arg_t()
	{
		/* Doesn't seem to work (make next test to segfault)
		if (argv) {
			char** i = argv;
			while (*i) {
				free(*i);
				++i;
			}
			delete [] argv;
		}
		*/
	}
};

std::ostream& operator<< (std::ostream& os, const arg_t& a)
{
	if (a.argv) {
		cppmt::string comma = "";

		char** i = a.argv;
		while (*i) {
			os << comma << '`' << *i << '`';
			comma = ", ";
			++i;
		}
	}
	return os;
}

static bool strnullcmp(const char* s1, const char* s2)
{
	if (s1 == NULL && s2 != NULL) {
		return false;
	}
	if (s1 != NULL && s2 == NULL) {
		return false;
	}
	if (s1 == NULL && s2 == NULL) {
		return true;
	}
	int ret = strcmp(s1, s2);

	return ret == 0;
}

template <typename T, typename S>
class opt_test_t
{
	public:
	T flag_opt1;
	S flag_opt2;
	int exp_ret;
	const char* exp_args[2];
	bool has_errors;

	opt_test_t(): flag_opt1(T()), flag_opt2(S()), exp_ret(0)
	{
		exp_args[0] = exp_args[1] = NULL;
	}

	opt_test_t(T f1, S f2, int er, bool e, const char* ea1, const char* ea2): flag_opt1(f1), flag_opt2(f2), exp_ret(er), has_errors(e)
	{
		exp_args[0] = ea1;
		exp_args[1] = ea2;
	}

	void init (T f1, S f2, int er, bool e, const char* ea1, const char* ea2)
	{
		flag_opt1 = f1;
		flag_opt2 = f2;
		exp_ret = er;
		exp_args[0] = ea1;
		exp_args[1] = ea2;
		has_errors = e;
	}
};

template <typename T, typename S>
class test_instance_t
{
	public:
	typedef std::pair<arg_t, opt_test_t<T, S> > pair_t;
};

// This function is just to save horizontal space (initialization is a looong line)
template <typename T, typename S>
std::pair<T, S> MP(T t, S s)
{
	return std::make_pair(t, s);
}

template <typename T, typename S>
void make_test(const std::string& testName, unsigned int i, cppmt::Opts& o, T& flag_opt1, S& flag_opt2, arg_t& a, opt_test_t<T, S>& t)
{
	int ret = o.get(a.argc, a.argv);
	bool has_errors = o.got_errors();

	std::stringstream ss_flag1;
	std::stringstream ss_flag2;
	std::stringstream ss_ret;
	std::stringstream ss_err;
	std::stringstream ss_arg1;
	std::stringstream ss_arg2;

	ss_flag1 << testName <<  " flag1 on args " << a << ", with i == " << i << ": expected " << t.flag_opt1   << ", got " << flag_opt1;
	ss_flag2 << testName <<  " flag2 on args " << a << ", with i == " << i << ": expected " << t.flag_opt2   << ", got " << flag_opt2;
	ss_ret   << testName << " return on args " << a << ", with i == " << i << ": expected " << t.exp_ret     << ", got " << ret;
	ss_err   << testName << " errors on args " << a << ", with i == " << i << ": expected " << t.has_errors  << ", got " << has_errors;
	ss_arg1  << testName <<   " arg1 on args " << a << ", with i == " << i << ": expected " << t.exp_args[0] << ", got " << a.argv[ret];
	if (ret+1 <= a.argc) {
		ss_arg2  <<   "arg2 on args " << a << ", with i == " << i << ": expected " << t.exp_args[1] << ", got " << a.argv[ret+1];
	}

	CPPUNIT_ASSERT_MESSAGE(ss_flag1.str(), t.flag_opt1  == flag_opt1);
	CPPUNIT_ASSERT_MESSAGE(ss_flag2.str(), t.flag_opt2  == flag_opt2);
	CPPUNIT_ASSERT_MESSAGE(  ss_ret.str(), t.exp_ret    == ret      );
	CPPUNIT_ASSERT_MESSAGE(  ss_err.str(), t.has_errors == has_errors);
	CPPUNIT_ASSERT_MESSAGE( ss_arg1.str(), strnullcmp(t.exp_args[0], a.argv[ret]));
	if (ret+1 <= a.argc) {
		CPPUNIT_ASSERT_MESSAGE( ss_arg2.str(), strnullcmp(t.exp_args[1], a.argv[ret+1]));
	}
}

void cpptTestGetOpt::testOptFlags()
{
	std::ofstream devnull("/dev/null");
	std::vector< test_instance_t<bool, bool>::pair_t > test;

	test.push_back( MP(arg_t("a.out", "pwet", "-s", NULL),              opt_test_t<bool, bool>(true,  false, 2, false, "pwet",   NULL  )) );
	test.push_back( MP(arg_t("a.out", "pwet", "--long", "gnark", NULL), opt_test_t<bool, bool>(true,  false, 2, false, "pwet",  "gnark")) );
	test.push_back( MP(arg_t("a.out", "-S", NULL),                      opt_test_t<bool, bool>(false, true,  2, false,  NULL,    NULL  )) );
	test.push_back( MP(arg_t("a.out", "--long2", "pwet", NULL),         opt_test_t<bool, bool>(false, true,  2, false, "pwet",   NULL  )) );
	test.push_back( MP(arg_t("a.out", "-sS", NULL),                     opt_test_t<bool, bool>(true,  true,  2, false,  NULL,    NULL  )) );
	test.push_back( MP(arg_t("a.out", NULL),                            opt_test_t<bool, bool>(false, false, 1, false,  NULL,    NULL  )) );
	test.push_back( MP(arg_t("a.out", "pwet", NULL),                    opt_test_t<bool, bool>(false, false, 1, false, "pwet",   NULL  )) );

	for (unsigned int i=0; i<test.size(); ++i) {
		bool flag_opt1 = false;
		bool flag_opt2 = false;

		cppmt::Opts o(devnull, devnull);
		o.add("long",  's', "Help string",   &flag_opt1);
		o.add("long2", 'S', "Help string 2", &flag_opt2);

		make_test(__FUNCTION__, i, o, flag_opt1, flag_opt2, test[i].first, test[i].second);
	}
}

void cpptTestGetOpt::testOptArgs()
{
	// Save horizontal space
	using cppmt::string;
	typedef opt_test_t<string, int> opt_test_si_t;

	std::ofstream devnull("/dev/null");
	std::vector< test_instance_t<string, int>::pair_t > test;

	test.push_back( MP(arg_t("a.out", NULL),                                        opt_test_si_t("<default>", 33, 1, false, NULL, NULL)) );
	test.push_back( MP(arg_t("a.out", "pwet", NULL),                                opt_test_si_t("<default>",  33, 1, false, "pwet", NULL )) );
	test.push_back( MP(arg_t("a.out", "-s", "SKY",     "pwet", NULL),               opt_test_si_t("SKY",        33, 3, false, "pwet", NULL )) );
	test.push_back( MP(arg_t("a.out", "-sSKY",         "pwet", NULL),               opt_test_si_t("SKY",        33, 2, false, "pwet", NULL )) );
	test.push_back( MP(arg_t("a.out", "--long=SKY",    "pwet", NULL),               opt_test_si_t("SKY",        33, 2, false, "pwet", NULL )) );
	test.push_back( MP(arg_t("a.out", "--long", "SKY", "pwet", NULL),               opt_test_si_t("SKY",        33, 3, false, "pwet", NULL )) );
	test.push_back( MP(arg_t("a.out", "pwet", "--long", "SKY", NULL),               opt_test_si_t("SKY",        33, 3, false, "pwet", NULL )) );

	test.push_back( MP(arg_t("a.out", "pwet", "-S", "74", NULL),                    opt_test_si_t("<default>",  74, 3, false, "pwet", NULL )) );
	test.push_back( MP(arg_t("a.out", "-S87", NULL),                                opt_test_si_t("<default>",  87, 2, false,  NULL,  NULL )) );
	test.push_back( MP(arg_t("a.out", "--long2=107", NULL),                         opt_test_si_t("<default>", 107, 2, false,  NULL,  NULL )) );
	test.push_back( MP(arg_t("a.out", "--long2=blob", "pwet", NULL),                opt_test_si_t("<default>",   0, 2, true,  "pwet", NULL )) );
	test.push_back( MP(arg_t("a.out", "", "--long", "SKY", "--long2=-5", "", NULL), opt_test_si_t("SKY",        -5, 4, false,     "", ""   )) );

	for (unsigned int i=0; i<test.size(); ++i) {
		cppmt::string flag_opt1 = "<default>";
		int          flag_opt2 = 33;

		cppmt::Opts o(devnull, devnull);
		o.add("long",  's', "Help string",   required_argument, &flag_opt1);
		o.add("long2", 'S', "Help string 2", required_argument, &flag_opt2);

		make_test(__FUNCTION__, i, o, flag_opt1, flag_opt2, test[i].first, test[i].second);
	}
}

void cpptTestGetOpt::testSameOpt()
{
	cppmt::Opts o;

	CPPUNIT_ASSERT(o.add("blob", 'b', "", NULL) == 0);
	CPPUNIT_ASSERT(o.add("blob", 'n', "", NULL) != 0);
	CPPUNIT_ASSERT(o.add("pwet", 'b', "", NULL) != 0);
	CPPUNIT_ASSERT(o.add("pwet", 'p', "", NULL) == 0);
	CPPUNIT_ASSERT(o.auto_help() == 0);
	CPPUNIT_ASSERT(o.auto_help() != 0);
}

void cpptTestGetOpt::testOnlyLong()
{
	std::ofstream devnull("/dev/null");
	std::vector< test_instance_t<bool, int>::pair_t > test;

	test.push_back( MP(arg_t("a.out", "pwet", "--long", NULL),        opt_test_t<bool, int>(true,  34, 2, false, "pwet", NULL)) );
	test.push_back( MP(arg_t("a.out", "--long2", "42", NULL),         opt_test_t<bool, int>(false, 42, 3, false,  NULL,  NULL)) );
	test.push_back( MP(arg_t("a.out", "--long2=11", "--long", NULL),  opt_test_t<bool, int>(true,  11, 3, false,  NULL,  NULL)) );

	for (unsigned int i=0; i<test.size(); ++i) {
		bool flag_opt1 = false;
		int  flag_opt2 = 34;

		cppmt::Opts o(devnull, devnull);
		o.add("long",   0 , "Help string",   &flag_opt1);
		o.add("long2",  0 , "Help string 2", required_argument, &flag_opt2);

		make_test(__FUNCTION__, i, o, flag_opt1, flag_opt2, test[i].first, test[i].second);
	}
}

void cpptTestGetOpt::testOnlyShort()
{
	std::ofstream devnull("/dev/null");
	std::vector< test_instance_t<bool, int>::pair_t > test;

	test.push_back( MP(arg_t("a.out", "pwet", "-l", NULL),    opt_test_t<bool, int>(true,  34, 2, false, "pwet",  NULL)) );
	test.push_back( MP(arg_t("a.out", "-L", "42", NULL),      opt_test_t<bool, int>(false, 42, 3, false,  NULL,   NULL)) );
	test.push_back( MP(arg_t("a.out", "-L11", "-l", NULL),    opt_test_t<bool, int>(true,  11, 3, false,  NULL,   NULL)) );

	for (unsigned int i=0; i<test.size(); ++i) {
		bool flag_opt1 = false;
		int  flag_opt2 = 34;

		cppmt::Opts o(devnull, devnull);
		o.add("", 'l', "Help string",   &flag_opt1);
		o.add("", 'L', "Help string 2", required_argument, &flag_opt2);

		make_test(__FUNCTION__, i, o, flag_opt1, flag_opt2, test[i].first, test[i].second);
	}
}

void cpptTestGetOpt::testConfigFile()
{
	std::string a[5];
	std::string b[7];
	std::string c[7];
	std::string d[4];

	cppmt::Opts o;
	o.add("optA1",  0 , "", required_argument, &a[0]);
	o.add("optA2",  0 , "", required_argument, &a[1]);
	o.add("optA3",  0 , "", required_argument, &a[2]);
	o.add("optA4",  0 , "", required_argument, &a[3]);
	o.add("optA5",  0 , "", required_argument, &a[4]);

	o.add("optB1",  0 , "", required_argument, &b[0]);
	o.add("optB2",  0 , "", required_argument, &b[1]);
	o.add("optB3",  0 , "", required_argument, &b[2]);
	o.add("optB4",  0 , "", required_argument, &b[3]);
	o.add("optB5",  0 , "", required_argument, &b[4]);
	o.add("optB6",  0 , "", required_argument, &b[5]);
	o.add("optB7",  0 , "", required_argument, &b[6]);

	o.add("optC1",  0 , "", required_argument, &c[0]);
	o.add("optC2",  0 , "", required_argument, &c[1]);
	o.add("optC3",  0 , "", required_argument, &c[2]);
	o.add("optC4",  0 , "", required_argument, &c[3]);
	o.add("optC5",  0 , "", required_argument, &c[4]);
	o.add("optC6",  0 , "", required_argument, &c[5]);
	o.add("optC7",  0 , "", required_argument, &c[6]);

	o.add("optD1",  0 , "", required_argument, &d[0]);
	o.add("optD2",  0 , "", required_argument, &d[1]);
	o.add("optD3",  0 , "", required_argument, &d[2]);
	o.add("optD4",  0 , "", required_argument, &d[3]);

	o.auto_config_file("config",  'c', "Set a configuration file.");

	arg_t ar("./a.out", "--optD1=not_default", "--optD2=gna", "--optD3", "", "-c", "./tests_01.getopt.cfg", NULL);
	o.get(ar.argc, ar.argv);

	CPPUNIT_ASSERT(a[0] == "valueA1");
	CPPUNIT_ASSERT(a[1] == "valueA2");
	CPPUNIT_ASSERT(a[2] == "valueA3");
	CPPUNIT_ASSERT(a[3] == "valueA4");
	CPPUNIT_ASSERT(a[4] == "valueA5");

	CPPUNIT_ASSERT(b[0] == "valueB1");
	CPPUNIT_ASSERT(b[1] == " valueB2");
	CPPUNIT_ASSERT(b[2] == "value'B3 ");
	CPPUNIT_ASSERT(b[3] == "value#B4");
	CPPUNIT_ASSERT(b[4] == "value\"B5");
	CPPUNIT_ASSERT(b[5] == " ");
	CPPUNIT_ASSERT(b[6] == "");

	CPPUNIT_ASSERT(c[0] == "valueC1");
	CPPUNIT_ASSERT(c[1] == " valueC2");
	CPPUNIT_ASSERT(c[2] == "value\"C3 ");
	CPPUNIT_ASSERT(c[3] == "value#C4");
	CPPUNIT_ASSERT(c[4] == "value'C5");
	CPPUNIT_ASSERT(c[5] == " ");
	CPPUNIT_ASSERT(c[6] == "");

	CPPUNIT_ASSERT(d[0] == "not_default");
	CPPUNIT_ASSERT(d[1] == "gna");
	CPPUNIT_ASSERT(d[2] == "");
	CPPUNIT_ASSERT(d[3] == "default4");

	CPPUNIT_ASSERT(o.got_errors() == false);
}

