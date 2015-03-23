
namespace cppmt
{

template <typename T>
inline T Sqlite::row_t::getValue(int idx) const
{
	return fromString<T>(getValue(idx));
}

template <typename T>
inline T Sqlite::row_t::getValue(const string& col) const
{
	return fromString<T>(getValue(col));
}

template <typename T>
T Sqlite::result_t::getValue(int idx) const
{
	return fromString<T>(getValue(idx));
}

template <typename T>
T Sqlite::result_t::getValue(const string& col) const
{
	return fromString<T>(getValue(col));
}

} // End of: namespace cppmt

