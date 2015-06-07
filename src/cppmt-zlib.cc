#include <cppmt-zlib.h>
#include <cppmt-exceptions.h>
#include <cppmt-string.h>
#include <cstring>

namespace cppmt
{

deflate_handler::deflate_handler(int level, size_t intern_buf_size): Z_handler(intern_buf_size)
{
	int zError;
	zError = deflateInit(&strm__, level);
	if (zError != Z_OK) {
		switch(zError) {
			case Z_MEM_ERROR:
				throw cppmt::exception("deflate_handler [construct]: deflateInit: Not enough memory.");
			case Z_STREAM_ERROR:
				throw cppmt::exception("deflate_handler [construct]: deflateInit: Invalid compression level.");
			case Z_VERSION_ERROR:
				throw cppmt::exception("deflate_handler [construct]: deflateInit: zlib version mismatch between headers and library.");
			default:
				throw cppmt::exception("deflate_handler [construct]: deflateInit: unknown error occured (" + cppmt::toString(zError) + ").");
		}
	}
}

deflate_handler::~deflate_handler()
{
	deflateEnd(&strm__);
}

int deflate_handler::compress(std::string& out, const std::string& in, bool flush)
{
	int zError = filter(out, in, false);
	if (zError < 0) {
		return zError;
	}

	if (flush) {
		zError = filter(out, "", true);
	}
	return zError;
}

std::string deflate_handler::compress(const std::string& in, int* status, bool flush)
{
	std::string out;
	int s = compress(out, in, flush);
	if (status) {
		*status = s;
	}
	return out;
}

std::ostream& deflate_handler::compress(std::ostream& os, const std::string& in, int* status, bool flush)
{
	std::string out;
	int s = compress(out, in, flush);
	if (status) {
		*status = s;
	}
	os << out;
	return os;
}




inflate_handler::inflate_handler(size_t intern_buf_size): Z_handler(intern_buf_size)
{
	int zError = inflateInit(&strm__);
	if (zError != Z_OK) {
		switch(zError) {
			case Z_MEM_ERROR:
				throw cppmt::exception("inflate_handler [construct]: deflateInit: Not enough memory.");
			case Z_STREAM_ERROR:
				throw cppmt::exception("inflate_handler [construct]: deflateInit: Invalid compression level.");
			case Z_VERSION_ERROR:
				throw cppmt::exception("inflate_handler [construct]: deflateInit: zlib version mismatch between headers and library.");
			default:
				throw cppmt::exception("inflate_handler [construct]: deflateInit: unknown error occured (" + cppmt::toString(zError) + ").");
		}
	}
}

inflate_handler::~inflate_handler()
{
	inflateEnd(&strm__);
}

int inflate_handler::decompress(std::string& out, const std::string& in, bool flush)
{
	int zError = filter(out, in, false);
	if (zError < 0) {
		return zError;
	}

	if (flush) {
		zError = filter(out, "", true);
	}
	return zError;
}

std::string inflate_handler::decompress(const std::string& in, int* status, bool flush)
{
	std::string out;
	int s = decompress(out, in, flush);
	if (status) {
		*status = s;
	}
	return out;
}

std::ostream& inflate_handler::decompress(std::ostream& os, const std::string& in, int* status, bool flush)
{
	std::string out;
	int s = decompress(out, in, flush);
	if (status) {
		*status = s;
	}
	os << out;
	return os;
}

} // End of: namespace cppmt

#include "cppmt-zlib.hxx"

