#include <cppmt-zlib.h>
#include <cppmt-exceptions.h>
#include <cppmt-string.h>
#include <cstring>

namespace cppmt
{

deflate_handler::deflate_handler(int level, size_t intern_buf_size): buffer_size__(intern_buf_size)
{
	if (intern_buf_size == 0) {
		throw cppmt::exception("deflate_handler [construct]: internal_buffer can't be 0.");
	}

	memset(reinterpret_cast<char*>(&strm__), 0, sizeof(strm__));
	
	int zError;
	strm__.zalloc = Z_NULL;
	strm__.zfree = Z_NULL;
	strm__.opaque = Z_NULL;

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

	buf__ = new char[intern_buf_size];
}

deflate_handler::~deflate_handler()
{
	delete [] buf__;
	deflateEnd(&strm__);
}

int deflate_handler::compress(std::string& out, const std::string& in, bool flush)
{
	int zError;
	int zOutHave;
	int zFlush = (flush ? Z_FINISH: Z_NO_FLUSH);
	out.clear();

	strm__.next_in = (Bytef*)in.data();;
	strm__.avail_in = in.size();
	do {
		strm__.avail_out = buffer_size__;
		strm__.next_out = (Bytef*)buf__;

		zError = deflate(&strm__, zFlush);
		if (zError < 0) {
			return zError;
		}

		zOutHave = buffer_size__ - strm__.avail_out;
		out.append(buf__, zOutHave);
	} while (strm__.avail_out == 0);

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




inflate_handler::inflate_handler(size_t intern_buf_size): buffer_size__(intern_buf_size)
{
	if (intern_buf_size == 0) {
		throw cppmt::exception("inflate_handler [construct]: internal_buffer can't be 0.");
	}

	memset(reinterpret_cast<char*>(&strm__), 0, sizeof(strm__));
	
	int zError;
	strm__.zalloc = Z_NULL;
	strm__.zfree = Z_NULL;
	strm__.opaque = Z_NULL;

	zError = inflateInit(&strm__);
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

	buf__ = new char[intern_buf_size];
}

inflate_handler::~inflate_handler()
{
	delete [] buf__;
	inflateEnd(&strm__);
}

int inflate_handler::decompress(std::string& out, const std::string& in, bool flush)
{
	int zError;
	int zOutHave;
	int zFlush = (flush ? Z_FINISH: Z_NO_FLUSH);
	out.clear();

	strm__.next_in = (Bytef*)in.data();;
	strm__.avail_in = in.size();
	do {
		strm__.avail_out = buffer_size__;
		strm__.next_out = (Bytef*)buf__;

		zError = inflate(&strm__, zFlush);
		if (zError < 0) {
			return zError;
		}

		zOutHave = buffer_size__ - strm__.avail_out;
		out.append(buf__, zOutHave);
	} while (strm__.avail_out == 0);

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

