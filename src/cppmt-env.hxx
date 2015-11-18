namespace cppmt
{

template <typename T>
env_t<T>::env_t(const char* n, T def): Name(n), Default(def) {}

template <typename T>
env_bounded_t<T>::env_bounded_t(const char* n, T minval, T maxval, T def): env_t<T>(n, def), minVal(minval), maxVal(maxval) {}

namespace getEnv
{

template <typename T>
static T safe(const env_t<T>& env)
{
	return safe(env.Name, env.Default);
}

template <typename T>
static T safe(const char* envName, T defaultValue)
{
	T r;
	status_t st = check(envName, &r, defaultValue);
	if (st.hasError or st.wasNull) {
		return defaultValue;
	}
	return r;
}

template <typename T>
static status_t check(const env_t<T>& env, T* res)
{
	return check(env.Name, res, env.Default);
}

template <typename T>
static status_t check(const char* envName, T* res, T defaultValue)
{
	status_t st;
	const char* e = getenv(envName);
	if (e == NULL) {
		st.wasNull = true;
		*res = defaultValue;
	}
	else {
		bool success;
		*res = fromString<T, const char*>(e, &success);
		if (not success) {
			*res = defaultValue;
			st.hasError = true;
		}
	}
	return st;
}

template <typename T>
static T safeBounded(const env_bounded_t<T>& env)
{
	return safeBounded(env.Name, env.minVal, env.maxVal, env.Default);
}

template <typename T>
static T safeBounded(const char* envName, T vmin, T vmax, T defaultValue)
{
	T r;
	status_t st = checkBounded(envName, &r, vmin, vmax, defaultValue);
	if (st.hasError or st.wasNull) {
		return defaultValue;
	}
	else {
		return r;
	}
}

template <typename T>
static status_t checkBounded(const env_bounded_t<T>& env, T* res)
{
	return checkBounded(env.Name, res, env.minVal, env.maxVal, env.Default);
}

template <typename T>
static status_t checkBounded(const char* envName, T* res, T vmin, T vmax, T defaultValue)
{
	status_t st = check(envName, res, defaultValue);
	if (st.wasNull == false and st.hasError == false) {
		if (*res < vmin or *res > vmax) {
			st.wasOutBound = true;
			if (*res < vmin) {
				*res = vmin;
			}
			else {
				*res = vmax;
			}
		}
	}
	return st;
}



} // End of: namespace getEnv

} // End of: namespace cppmt

