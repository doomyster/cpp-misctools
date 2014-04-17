/*
    Copyright © 2013-2014 Nicolas Normand <nnormand@justcodeit.org>. All Rights Reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

    3. The name of the author may not be used to endorse or promote products derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Nicolas Normand "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef CPP_TOOLS_EXCEPTIONS_H_
#define CPP_TOOLS_EXCEPTIONS_H_

#include <exception>
#include <string>
#include <cstring>
#include <typeinfo>

#include <cerrno>

extern "C"
{
#include <netdb.h>
}

namespace cppt
{

class exception: public std::exception
{
	private:
	mutable std::string full_msg__;

	protected:
	std::string msg__;

	exception() {}
	public:
	exception(const std::string& msg_): msg__(msg_) {}
	virtual ~exception() throw() {}

	virtual const char* type() const throw() { return "exception"; }
	virtual const char* what() const throw() { full_msg__ = type(); full_msg__ += ": " + msg__; return full_msg__.c_str(); }

	virtual const std::string& user_message() const { return msg__; }
};

class stdc_exception: public exception
{
	protected:
	int c_errno__;

	public:
	stdc_exception(int c_errno_, const std::string& msg_): exception((msg_ + ": ") + strerror(c_errno_)), c_errno__(c_errno_) {}
	virtual ~stdc_exception() throw() {}

	virtual const char* type() const throw() { return "stdc_exception"; }
	inline int c_errno() const throw() { return c_errno__; }
};

class h_exception: public exception
{
	protected:
	int h_errno__;

	public:
	h_exception(int h_errno_, const std::string& msg_): exception((msg_ + ": ") + hstrerror(h_errno_)), h_errno__(h_errno_) {}
	virtual ~h_exception() throw() {}

	virtual const char* type() const throw() { return "h_exception"; }
	inline int herrno() const throw() { return h_errno__; } // Apparently, I can't call the method 'h_errno'... Hell ! 'h_errno' is a #define !!
};

} // namespace cppt

#endif

