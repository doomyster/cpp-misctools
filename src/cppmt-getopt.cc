/*
    Copyright © 2013-2014 Nicolas Normand <nnormand@justcodeit.org>. All Rights Reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

    3. The name of the author may not be used to endorse or promote products derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Nicolas Normand "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <cstring>

#include <pcrecpp.h>

#include "cppmt-getopt.h"

namespace cppmt
{

string Opts::convert_flag(void* recv_opt, char* opt_arg)
{
        bool* o = reinterpret_cast<bool*>(recv_opt);
	*o = true;

	return "";
}

Opts::opt_t::opt_t(const string& long_opt_, char short_opt_, const string& helpstring_, int has_arg_, void* recv_opt_, Opts::convert_t conv_):
		long_opt(long_opt_),
		helpstring(helpstring_),
		recv_opt(recv_opt_),
		conv(conv_),
		was_found(false),
		short_opt(short_opt_),
		has_arg(has_arg_),
		was_in_command_line(false)
{
}

void Opts::set_exit_func(void (*exit_func)(int))
{
	exit_func__ = (exit_func == NULL) ? exit: exit_func;
}

bool Opts::find_same_opt(const string& long_opt, char short_opt) const
{
	std::vector<opt_t>::const_iterator it = all_opts__.begin();
	bool found_same_opt = false;

	while(it != all_opts__.end())
	{
		if ((it->long_opt == long_opt && long_opt.empty() == false) || (it->short_opt == short_opt && short_opt != '\0'))
		{
			found_same_opt = true;
			break;
		}

		++it;
	}

	return found_same_opt;
}

Opts::Opts(std::ostream& o_stream, std::ostream& e_stream): autoshow_help__(false), 
							    autoshow_version__(false), 
							    out_stream__(o_stream), 
							    err_stream__(e_stream),
							    exit_func__(exit),
							    config_file__()
{
}

int Opts::add(const string& long_opt, char short_opt, const string& helpstring, bool* flag)
{
	if (find_same_opt(long_opt, short_opt))
		return -1;
	
	all_opts__.push_back( opt_t(long_opt, short_opt, helpstring, no_argument, reinterpret_cast<void*>(flag), convert_flag) );
	return 0;
}

int Opts::auto_config_file(const string& long_opt, char short_opt, const string& helpstring, const string& default_location)
{
	config_file__ = default_location;
	return add(long_opt, short_opt, helpstring, required_argument, &config_file__);
}

int Opts::auto_help()
{
	int res = add("help", 'h', "Show this help and exit", &autoshow_help__);
	if (res < 0)
		return add("help", 'H', "Show this help and exit", &autoshow_help__);
	return res;
}

int Opts::auto_version(const string& version)
{
	int res = add("version", 'v', "Show version and exit", &autoshow_version__);
	if (res < 0)
	{
		res = add("version", 'V', "show version and exit", &autoshow_version__);
		if (res >= 0)
			version__ = version;
		return res;
	}
	version__ = version;
	return res;
}

void Opts::build_struct_option_array(std::vector<struct option>& opts, string& short_opts, int* flag) const
{
	int counter = 1;
	std::vector<opt_t>::const_iterator it = all_opts__.begin();
	while(it != all_opts__.end())
	{
		if (it->long_opt.empty() == false)
		{
			if (it->short_opt)
			{
				struct option build_opt = { it->long_opt.c_str(), it->has_arg, NULL, it->short_opt };
				opts.push_back(build_opt);
			}
			else
			{
				struct option build_opt = { it->long_opt.c_str(), it->has_arg, flag, counter };
				opts.push_back(build_opt);
			}
		}
		if (it->short_opt)
		{
			short_opts += it->short_opt;
			short_opts.append(it->has_arg, ':');
		}

		++counter;
		++it;
	}

	struct option last_opt = { NULL, 0, NULL, 0 };
	opts.push_back(last_opt);
}

int Opts::get(int argc, char** argv, int force_optind)
{
	std::vector<struct option> opts;
	string short_opts;
	int flag;

	build_struct_option_array(opts, short_opts, &flag);

	int c;
	int option_index = 1;
	
	got_errors__ = false;
	optind = force_optind;
	while(1)
	{
		flag = 0;
		c = getopt_long(argc, argv, short_opts.c_str(), &(opts[0]), &option_index);
		if (c == -1)
			break;

		if (c == 0)
		{
			if (flag == 0) // Nothing was touched. Strange, should not happen
				return -1; // Let's do that for now
			opt_t& curr_opt = all_opts__[flag -1];

			if (curr_opt.conv && curr_opt.recv_opt)
			{
				const string& tmpstr = curr_opt.conv(curr_opt.recv_opt, optarg);
				if (tmpstr.empty() == false)
				{
					got_errors__ = true;
					err_stream__ << argv[optind-1] << ": " << tmpstr << std::endl;
				}
				curr_opt.was_in_command_line = true;
			}
		}
		else if (c == '?')
		{
			got_errors__ = true;
		}
		else
		{
			std::vector<opt_t>::iterator it = all_opts__.begin();
			while (it != all_opts__.end())
			{
				if (c == it->short_opt && it->conv && it->recv_opt)
				{
					const string& tmpstr = it->conv(it->recv_opt, optarg);

					if (tmpstr.empty() == false)
					{
						got_errors__ = true;
						err_stream__ << argv[optind-1] << ": " << tmpstr << std::endl; 
					}
					it->was_in_command_line = true;
				}

				++it;
			}
		}
	}

	if (autoshow_help__ == true)
	{
		show_help();
		exit_func__(0);
	}
	if (autoshow_version__ == true)
	{
		out_stream__ << version__ << std::endl;
		exit_func__(0);
	}
	if (!config_file__.empty())
	{
		read_config_file();
	}

	return optind;
}

void Opts::handle_line(const std::string& line)
{
	static pcrecpp::RE dquo ("^\\s*([^=\\s]+)\\s*=\\s*\"((?:[^\"\\\\]|\\\\.)*+)\"\\s*(?:#.*)?$");
	static pcrecpp::RE squo ("^\\s*([^=\\s]+)\\s*=\\s*'((?:[^'\\\\]|\\\\.)*+)'\\s*(?:#.*)?$");
	static pcrecpp::RE nquo ("^\\s*([^=\\s]+)\\s*=\\s*([^#]*)\\s*(?:#.*)?$");
	static pcrecpp::RE empty("^\\s*(?:#.*)*$");
	static pcrecpp::RE rep_dquote("\\\\\"");
	static pcrecpp::RE rep_squote("\\\\'");

	std::string opt_name;
	std::string opt_value;
	bool quoted = false;

	if (line.empty() || empty.FullMatch(line)) {
		return;
	}
	else if (dquo.FullMatch(line, &opt_name, &opt_value)) {
		rep_dquote.GlobalReplace("\"", &opt_value);
		quoted = true;
	}
	else if (squo.FullMatch(line, &opt_name, &opt_value)) {
		rep_squote.GlobalReplace("'", &opt_value);
		quoted = true;
	}
	else if (nquo.FullMatch(line, &opt_name, &opt_value)) {
	}
	else {
		err_stream__ << "read(" << config_file__ << "): invalid line: `" << line << "`" << std::endl;
		return;
	}

	opt_name  = trim(opt_name);
	if (!quoted) {
		opt_value = trim(opt_value);
	}

	std::vector<opt_t>::iterator it;
	for (it = all_opts__.begin(); it != all_opts__.end(); ++it) {
		if (it->long_opt == opt_name && it->was_in_command_line == false && it->conv && it->recv_opt) {
			char* o = strdup(opt_value.c_str());
			const string& res = it->conv(it->recv_opt, o);
			free(o);

			if (res.empty() == false) {
				got_errors__ = true;
				err_stream__ << "In config file `" << config_file__ << "`, option " << opt_name << ": " << res << std::endl;
			}
		}
	}
}

void Opts::read_config_file()
{
	FILE* f = fopen(config_file__.c_str(), "r");
	if (f == NULL)
	{
		int e = errno;
		err_stream__ << "open(" << config_file__ << "): " << strerror(e) << std::endl;
		got_errors__ = true;
		return;
	}

	size_t t;
	ssize_t n;
	char* line = NULL;

	while ((n = getline(&line, &t, f) > 0))
	{
		t = strlen(line);
		if (line[t-1] == '\n') {
			line[t-1] = '\0';
		}
		std::string s = line;
		free(line);
		line = NULL;

		handle_line(s);
	}
	if (n < 0) {
		int e = errno;
		err_stream__ << "getline(" << config_file__ << "): " << strerror(e) << std::endl;
		got_errors__ = true;
	}

	fclose(f);
}

void Opts::show_help()
{
	if (version__.empty() == false)
		out_stream__ << version__ << std::endl;

	std::vector<std::vector<string> > help_strings;
	unsigned int  longoptlen_max = 0;
	unsigned int shortoptlen_max = 0;
	{
		std::vector<opt_t>::const_iterator it = all_opts__.begin();
		while (it != all_opts__.end())
		{
			std::vector<string> strvec;

			string shortopt_helpstring;
			string  longopt_helpstring;

			if (it->short_opt != 0)
			{
				shortopt_helpstring = string("-") + it->short_opt;
				if ((it->has_arg == 1) && it->long_opt.empty())
					shortopt_helpstring += "<ARG>";
				else if ((it->has_arg == 2) && it->long_opt.empty())
					shortopt_helpstring += "[<ARG>]";
				if (it->long_opt.empty() == false)
					shortopt_helpstring += ", ";
			}

			if (it->long_opt.empty() == false)
			{
				longopt_helpstring = "--" + it->long_opt;
				if (it->has_arg == 1)
					longopt_helpstring += "=<ARG>";
				else if (it->has_arg == 2)
					longopt_helpstring += "=[<ARG>]";
			}


			if ( longopt_helpstring.size() >  longoptlen_max)
				longoptlen_max =  longopt_helpstring.size();
			if (shortopt_helpstring.size() > shortoptlen_max)
				shortoptlen_max = shortopt_helpstring.size();

			strvec.push_back(shortopt_helpstring);
			strvec.push_back( longopt_helpstring);
			strvec.push_back(it->helpstring);

			help_strings.push_back(strvec);

			++it;
		}
		longoptlen_max += 2;
		shortoptlen_max += 2;
	}

	std::vector<std::vector<string> >::iterator it = help_strings.begin();
	while(it != help_strings.end())
	{
		out_stream__ << (*it)[0] << string( shortoptlen_max - ((*it)[0]).size(), ' ' ) <<
			(*it)[1] << string(  longoptlen_max - ((*it)[1]).size(), ' ' ) <<
			(*it)[2] << std::endl;

		++it;
	}
}

bool Opts::got_errors() const
{
	return got_errors__;
}

} // End of: namespace cppmt

