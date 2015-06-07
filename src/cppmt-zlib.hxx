namespace cppmt
{

template <flate_fn_t T>
Z_handler<T>::Z_handler(size_t intern_buf_size): buffer_size__(intern_buf_size)
{
	if (intern_buf_size == 0) {
		throw cppmt::exception("Zlib handler [construct]: internal_buffer can't be 0.");
	}

	memset(reinterpret_cast<char*>(&strm__), 0, sizeof(strm__));
	strm__.zalloc = Z_NULL;
	strm__.zfree = Z_NULL;
	strm__.opaque = Z_NULL;

	buf__ = new char[buffer_size__];
}

template <flate_fn_t T>
Z_handler<T>::~Z_handler()
{
	delete [] buf__;
}

template <flate_fn_t T>
int Z_handler<T>::filter(std::string& out, const std::string& in, bool flush)
{
	int zError;
	int zOutHave;
	int zFlush = (flush ? Z_FINISH: Z_NO_FLUSH);

	strm__.next_in = (Bytef*)in.data();;
	strm__.avail_in = in.size();
	do {
		strm__.avail_out = buffer_size__;
		strm__.next_out = (Bytef*)buf__;

		zError = T(&strm__, zFlush);
		if (zError < 0) {
			if (in.size() == 0) {
				return Z_OK;
			}
			else {
				return zError;
			}
		}

		zOutHave = buffer_size__ - strm__.avail_out;
		out.append(buf__, zOutHave);
	} while (strm__.avail_out == 0);

	return zError;
}

} // End of: namespace cppmt

