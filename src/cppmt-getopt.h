/*
    Copyright © 2013-2014 Nicolas Normand <nnormand@justcodeit.org>. All Rights Reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

    3. The name of the author may not be used to endorse or promote products derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Nicolas Normand "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef CPP_TOOLS_GETOPT_H_
#define CPP_TOOLS_GETOPT_H_

#include <getopt.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "cppmt-string.h"

namespace cppmt
{

class Opts
{
	public:
	typedef string (*convert_t)(void*, char*);
	typedef enum
	{
		EXIT_ON_OPEN_FAILURE,
		CONTINUE_ON_OPEN_FAILURE,
		SILENT_CONTINUE_ON_OPEN_FAILURE
	} cfg_fail_t;

	protected:
	struct opt_t
	{
		string long_opt;
		string helpstring;
		void* recv_opt;
		convert_t conv;
		bool was_found;
		char short_opt;
		int has_arg;
		bool was_in_command_line;

		opt_t(const string& long_opt, char short_opt, const string& helpstring, int has_arg, void* recv_opt, convert_t conv);
	};

	bool fail_on_open_error__;
	bool silent_on_open_error__;

	bool got_errors__;
	bool autoshow_help__;
	bool autoshow_version__;
	string   version__;
	std::ostream& out_stream__;
	std::ostream& err_stream__;
	void (*exit_func__)(int);
	string config_file__;

	std::vector<opt_t> all_opts__;

	bool find_same_opt(const string& long_opt, char short_opt) const;
	void build_struct_option_array(std::vector<struct option>& opts, string& short_opts, int* flag) const;
	static string convert_flag(void* recv_opt, char* opt_arg);
	int read_config_file();
	void handle_line(const std::string& line);

	public:
	Opts(std::ostream& o_stream = std::cout, std::ostream& e_stream = std::cerr);
	void set_exit_func(void (*exit_func)(int));

	template <typename T>
	static string convert(void* recv_opt, char* opt_arg);

	template<typename T>
	int add(const string& long_opt, char short_opt, const string& helpstring, int has_arg, T* recv_opt, convert_t conv = Opts::convert<T>);
	int add(const string& long_opt, char short_opt, const string& helpstring, bool* flag);

	int auto_config_file(const string& long_opt, char short_opt, const string& helpstring, const string& default_location="");
	int auto_config_file(const string& long_opt, char short_opt, const string& helpstring, cfg_fail_t fail, const string& default_location="");
	int auto_help(); // Add option "--help", "-h". Uses "-H" if option "-h" exists
	int auto_help(const string& long_opt, char short_opt, const string& helpstring="Show this help and exit");
	int auto_version(const string& version); // If version.empty() == true, disable this feature. Add option "--version", "-v". Uses "-V" if option "-v" exists
	int auto_version(const string& version, const string& long_opt, char short_opt, const string& helpstring="show version and exit");

	int get(int argc, char** argv, int force_optind=1); // Returns optind
	bool got_errors() const;

	void show_help();
};

template <>
string Opts::convert<string>(void* recv_opt, char* opt_arg);

} // End of namespace cppmt

#include "cppmt-getopt.hxx"

#endif

