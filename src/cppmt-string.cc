/*
    Copyright © 2013-2014 Nicolas Normand <nnormand@justcodeit.org>. All Rights Reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

    3. The name of the author may not be used to endorse or promote products derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Nicolas Normand "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "cppmt-string.h"
#include <algorithm>

namespace cppmt
{

bool endsWith(const string& haystack, const string& needle)
{
	if (haystack.length() >= needle.length()) {
		return haystack.compare(haystack.size() - needle.size(), needle.size(), needle) == 0;
	}
	return false;
}

// Copy free trim functions
void trim(string* s)
{
	rtrim(s);
	ltrim(s);
}

// http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// &Limitation: might not work on international strings
void ltrim(string* s)
{
	s->erase(s->begin(), std::find_if(s->begin(), s->end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
}

void rtrim(string* s)
{
	s->erase(std::find_if(s->rbegin(), s->rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s->end());
}


// Copy trim functions
string trim(const string& str)
{
	string s = str;
	trim(&s);
	return s;
}

string ltrim(const string& str)
{
	string s = str;
	ltrim(&s);
	return s;
}

string rtrim(const string& str)
{
	string s = str;
	rtrim(&s);
	return s;
}

std::vector<string> split(const string& str, char delim)
{
	std::vector<string> tokens;
	split(tokens, str, delim);
	return tokens;
}

void split(std::vector<string>& tokens, const string& str, char delim)
{
	std::stringstream ss(str);
	string item;
	while(std::getline(ss, item, delim)) {
		tokens.push_back(item);
	}
	// Previous code doesn't handle the last column if it ends with the delimiter.
	if (endsWith(str, string(&delim, 1))) {
		tokens.push_back("");
	}
}

} // End of: namespace cppmt

