#pragma once

//Source: https://github.com/notoes/RefEngine

#include <functional>

/**
 * A variadic template implmentation of std::hash<T> that allows hashing of any number of arguments.
 */

// Provide a no argument version to handle a parameter pack expansion with no arguments. Returns 0.
size_t vhash();

// Handle a parameter pack expansion with one argument.
// Acts a terminal for the "recursive" vash.
template<typename T>
size_t vhash(const T& t)
{
	return std::hash<T>()(t);
}

// A "recursive" variadic template for computing a variable number of hashes and combining the result.
template<typename T, typename... Ts>
size_t vhash(const T& t, const Ts&... ts)
{
	int hash = std::hash<T>()(t);
	return hash ^ 1 << vhash(ts...);
}
