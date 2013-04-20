/*

Format
Curly brace string formatting with braces, like Python and C#.

James Preiss, 2013.  Public domain.

*/

#ifndef _FORMAT_H_
#define _FORMAT_H_

#include <string>
#include <sstream>

namespace
{
	//
	// recursive implementation, slow but correct
	// will replace with an iterative version ASAP
	//
	std::string find_and_replace(std::string const &str, std::string const &find, std::string const &replace)
	{
		size_t find_length = find.length();

		if (find_length == 0)
		{
			return str;
		}

		size_t index = str.find(find);

		if (index == std::string::npos)
		{
			return str;
		}

		std::string before = str.substr(0, index);
		std::string after = str.substr(index + find_length, std::string::npos);
		std::string recurse_after = find_and_replace(after, find, replace);

		return before + replace + recurse_after;
	}

	//
	// workhorse
	//
	template <typename T, int argnum>
	std::string format_onearg(std::string const &fmt, T const &value)
	{
		std::ostringstream arg_stream;
		arg_stream << value;
		std::string arg_string = arg_stream.str();

		std::ostringstream search_stream;
		search_stream << '{' << argnum << '}';
		std::string search_string = search_stream.str();

		return find_and_replace(fmt, search_string, arg_string);
	}
}

//
// should be a variadic template, but VS2010.
//
template <typename T>
std::string format(std::string const &fmt, T const &value)
{
	return format_onearg<T, 0>(fmt, value);
}

template <typename T0, typename T1>
std::string format(std::string const &fmt, T0 const &v0, T1 const &v1)
{
	return format_onearg<T1, 1>(format(fmt, v0), v1);
}

template <typename T0, typename T1, typename T2>
std::string format(std::string const &fmt, T0 const &v0, T1 const &v1, T2 const &v2)
{
	return format_onearg<T2, 2>(format(fmt, v0, v1), v2);
}

template <typename T0, typename T1, typename T2, typename T3>
std::string format(std::string const &fmt, T0 const &v0, T1 const &v1, T2 const &v2, T3 const &v3)
{
	return format_onearg<T3, 3>(format(fmt, v0, v1, v2), v3);
}

template <typename T0, typename T1, typename T2, typename T3, typename T4>
std::string format(std::string const &fmt, T0 const &v0, T1 const &v1, T2 const &v2, T3 const &v3, T4 const &v4)
{
	return format_onearg<T4, 4>(format(fmt, v0, v1, v2, v3), v4);
}

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
std::string format(std::string const &fmt, T0 const &v0, T1 const &v1, T2 const &v2, T3 const &v3, T4 const &v4, T5 const &v5)
{
	return format_onearg<T4, 4>(format(fmt, v0, v1, v2, v3, v4), v5);
}

//
// six format arguments ought to be enough for anyone
//

#endif // _FORMAT_H_