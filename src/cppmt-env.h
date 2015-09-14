/*
    Copyright © 2013-2015 Nicolas Normand <nnormand@justcodeit.org>. All Rights Reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

    3. The name of the author may not be used to endorse or promote products derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Nicolas Normand "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef CPPMT_ENV_H_
#define CPPMT_ENV_H_

#include "cppmt-string.h"
#include <cstdlib>

namespace cppmt
{

namespace getEnv
{
	class status_t
	{
		public:
		bool wasNull;
		bool hasError;
		bool wasOutBound;

		status_t(bool wNull = false, bool hErr = false, bool wOut = false);
	};

	template <typename T>
	static T safe(const char* envName, T defaultValue = T());

	template <typename T>
	static status_t check(const char* envName, T* res, T defaultValue = T());

	template <typename T>
	static T safeBounded(const char* envName, T vmin, T vmax, T defaultValue = T());

	template <typename T>
	static status_t checkBounded(const char* envName, T* res, T vmin, T vmax, T defaultValue = T());

} // End of: namespace getEnv

} // End of: namespace cppmt

#include "cppmt-env.hxx"

#endif 

