/*
    Copyright © 2013-2014 Nicolas Normand <nnormand@justcodeit.org>. All Rights Reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

    3. The name of the author may not be used to endorse or promote products derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Nicolas Normand "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


namespace cppmt
{

template <typename T>
string Opts::convert(void* recv_opt, char* opt_arg)
{
	if (opt_arg)
	{
		T* o = reinterpret_cast<T*>(recv_opt);
		std::stringstream ss(opt_arg);
		ss >> *o;

		if (!ss)
			return "Invalid type convertion";
	}
	return "";
}

template <>
string Opts::convert<string>(void* recv_opt, char* opt_arg)
{
	if (opt_arg)
	{
		string* s = reinterpret_cast<string*>(recv_opt);
		*s = opt_arg;
	}
	return "";
}


template <typename T>
int Opts::add(const string& long_opt, char short_opt, const string& helpstring, int has_arg, T* recv_opt, convert_t conv)
{
	if (find_same_opt(long_opt, short_opt))
		return -1;
	
	if (has_arg != required_argument && has_arg != optional_argument)
		return -2;

	all_opts__.push_back( opt_t(long_opt, short_opt, helpstring, has_arg, reinterpret_cast<void*>(recv_opt), conv) );
	return 0;
}

}

