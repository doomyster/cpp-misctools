#include "cppmt-encoding.h"

#include <sstream>

#include <b64/encode.h>
#include <b64/decode.h>

namespace cppmt
{

namespace base64
{

std::string encode(const char* in, size_t s)
{
	std::string r(in, s);
	return encode(r);
}

std::string encode(const std::string& in)
{
	std::string r;
	encode(in, r);
	return r;
}

void encode(const std::string& in, std::string& out)
{
	std::istringstream is(in);
	std::ostringstream os;

	encode(is, os);
	out = os.str();
}

void encode(std::istream& is, std::ostream& os)
{
	::base64::encoder enc;
	enc.encode(is, os);
}


std::string decode(const char* in, size_t s)
{
	std::string r(in, s);
	return decode(r);
}

std::string decode(const std:: string& in)
{
	std::string r;
	decode(in, r);
	return r;
}

void decode(const std::string& in, std::string& out)
{
	std::istringstream is(in);
	std::ostringstream os;
	decode(is, os);
	out = os.str();
}

void decode(std::istream& is, std::ostream& os)
{
	::base64::decoder dec;
	dec.decode(is, os);
}

} // End of: namespace cppmt

} // End of: namespace cppmt






