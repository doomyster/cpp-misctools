#ifndef CPPMT_ENCODING_H_
#define CPPMT_ENCODING_H_

#include <string>
#include <iostream>

namespace cppmt
{

namespace base64
{

std::string encode(const char* in, size_t s);
std::string encode(const std::string& in);
void        encode(const std::string& in, std::string&  out);
void        encode(std::istream&      is, std::ostream& os );

std::string decode(const char* in, size_t s);
std::string decode(const std:: string& in);
void        decode(const std:: string& in, std:: string& out);
void        decode(      std::istream& is, std::ostream& os );

} // End of: namespace base64

} // End of: namespace cppmt




#endif 

