#ifndef CPPMT_ZLIB_H_
#define CPPMT_ZLIB_H_

#include <iostream>
#include <string>
#include "zlib.h"

namespace cppmt
{

class deflate_handler
{
	private:
	z_stream strm__;
	size_t buffer_size__;
	char* buf__;

	public:
	deflate_handler(int level = 6, size_t intern_buf_size = 4096);
	~deflate_handler();

	int           compress(std::string& out, const std::string& in, bool flush = false);
	std::string   compress(                  const std::string& in, int* status = 0, bool flush = false);
	std::ostream& compress(std::ostream& os, const std::string& in, int* status = 0, bool flush = false);
};

class inflate_handler
{
	private:
	z_stream strm__;
	size_t buffer_size__;
	char* buf__;

	public:
	inflate_handler(size_t intern_buf_size = 4096);
	~inflate_handler();

	int           decompress(std::string& out, const std::string& in, bool flush = false);
	std::string   decompress(                  const std::string& in, int* status = 0, bool flush = false);
	std::ostream& decompress(std::ostream& os, const std::string& in, int* status = 0, bool flush = false);
};






} // End of: namespace cppmt

#endif 

