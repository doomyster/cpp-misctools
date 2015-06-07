#ifndef CPPMT_ZLIB_H_
#define CPPMT_ZLIB_H_

#include <iostream>
#include <string>
#include "zlib.h"

namespace cppmt
{

// Internal types. User won't need these
// Usage: avoid code duplication.
// {
typedef int (*flate_fn_t)(z_streamp, int);

template <flate_fn_t T>
class Z_handler
{
	protected:
	z_stream strm__;
	size_t buffer_size__;
	char* buf__;


	Z_handler(size_t intern_buf_size = 4096);
	virtual ~Z_handler();

	int filter(std::string& out, const std::string& in, bool flush = false);
};
// }

// Compression and decompression classes handlers
// {

// These handler classes can be used to compress/decompress data.
// You can call any 'compress'/'decompress' methods cumulatively. Just assure the last call
// must be flushed. Once flushed, the stream should not have any remaining data
// to be retrieved.
// The 'status' represents the return value of the inflate/deflate functions. 
// Basically, if not Z_OK or Z_STREAM_END, you should worry.
class deflate_handler: public Z_handler<deflate>
{
	public:
	deflate_handler(int level = 6, size_t intern_buf_size = 4096);
	~deflate_handler();

	int           compress(std::string& out, const std::string& in, bool flush = false); // 'out' will *NOT* be cleared. Data is APPENDED to it.
	std::string   compress(                  const std::string& in, int* status = 0, bool flush = false);
	std::ostream& compress(std::ostream& os, const std::string& in, int* status = 0, bool flush = false);
};

class inflate_handler: public Z_handler<inflate>
{
	public:
	inflate_handler(size_t intern_buf_size = 4096);
	~inflate_handler();

	int           decompress(std::string& out, const std::string& in, bool flush = false); // 'out' will *NOT* be cleared. Data is APPENDED to it.
	std::string   decompress(                  const std::string& in, int* status = 0, bool flush = false);
	std::ostream& decompress(std::ostream& os, const std::string& in, int* status = 0, bool flush = false);
};

// }

} // End of: namespace cppmt

#endif 

