/*
    Copyright © 2013-2014 Nicolas Normand <nnormand@justcodeit.org>. All Rights Reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

    3. The name of the author may not be used to endorse or promote products derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Nicolas Normand "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef CPP_TOOLS_H_
#define CPP_TOOLS_H_

#include <string>
#include <sstream>
#include <vector>

namespace cppmt
{

using std::string;


template <typename T, typename STRINGTYPE>
T fromString(const STRINGTYPE& v, bool* success = NULL)
{
	T cv;
	std::stringstream ss;
	ss << v;
	ss >> cv;
	if (success) {
		*success = (ss && ss.eof());
	}
	return cv;
}

template <typename T, typename STRINGTYPE>
bool fromString(T& val, const STRINGTYPE& v)
{
	bool r;
	val = fromString<T>(v, &r);
	return r;
}

template <typename T>
string toString(T v)
{
	std::stringstream ss;
	ss << v;
	return ss.str();
}

bool endsWith(const string& haystack, const string& needle);

// alter *str
// str must not be NULL
void  trim(string* str);
void ltrim(string* str);
void rtrim(string* str);

string trim(const string& str);
string ltrim(const string& str);
string rtrim(const string& str);

std::vector<string> split(                             const string& str, char delim);
void                split(std::vector<string>& tokens, const string& str, char delim);

}

#endif

