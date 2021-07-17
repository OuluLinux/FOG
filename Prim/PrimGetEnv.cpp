#include <Prim/PrimIncludeAll.h>





#ifndef NEEDS_BOOL
//
//  	Construction of a boolean GetEnv variable first assigns the defaultValue and then
//  	assigns true if envName is defined.
//
template <> PrimGetEnv<bool>::PrimGetEnv(const char* envName, const bool& defaultValue)
	:
	_value(defaultValue) {
	if (getenv(envName))
		_value = true;
}
#endif

//
//  	Construction of an int GetEnv variable first assigns the defaultValue and then
//  	assigns the value if envName is defined.
//
template <> PrimGetEnv<int>::PrimGetEnv(const char* envName, const int& defaultValue)
	:
	_value(defaultValue) {
	const char* p = getenv(envName);
	
	if (p)
		_value = atoi(p);
}

//
//  	Construction of a character GetEnv variable first assigns the defaultValue unless
//  	envName is defined in which case the value of enb]vName is assigned.
//
template <> PrimGetEnv<const char*>::PrimGetEnv(const char* envName, const char* const& defaultValue)
	:
	_value(getenv(envName)) {
	if (!_value)
		_value = defaultValue;
}
