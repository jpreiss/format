/*

Format
Curly brace string formatting with braces, like Python and C#.

James Preiss, 2013.  Public domain.

*/

#ifndef _FORMAT_H_
#define _FORMAT_H_

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

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

	std::vector<std::string::const_iterator> find_all_substrings(std::string const &str, std::string const &find)
	{
		std::vector<std::string::const_iterator> iters;

		size_t find_length = find.length();

		if (find_length == 0)
		{
			return iters;
		}

		// probably more than enough for a typical formatting operation
		iters.reserve(4);

		std::string::const_iterator begin = str.begin();

		size_t index = str.find(find);

		while (index != std::string::npos)
		{
			iters.push_back(begin + index);
			size_t next_start = index + find_length;

			if (next_start >= str.length())
			{
				break;
			}

			index = str.find(find, next_start);
		}

		return iters;
	}

	std::string find_and_replace_iter(std::string const &str, std::string const &find, std::string const &replace)
	{
		auto substring_iters = find_all_substrings(str, find);
		size_t num_substrings = substring_iters.size();

		if (num_substrings == 0)
		{
			return str;
		}

		size_t str_size = str.size();
		size_t find_size = find.size();
		size_t replace_size = replace.size();

		ptrdiff_t delta_size = (replace_size - find_size) * num_substrings;
		size_t output_size = str_size + delta_size;

		// would prefer not to do initialization here,
		// but replace() doesn't play nice
		std::string output(output_size, '\0');

		auto src = str.begin();
		auto dst = output.begin();
		
		std::for_each(substring_iters.begin(), substring_iters.end(), [&](std::string::const_iterator iter)
		{
			// upto
			ptrdiff_t length_upto = iter - src;
			output.replace(dst, dst + length_upto,
			               src, iter);

			dst += length_upto;

			// replace
			output.replace(dst, dst + replace_size, replace);

			src = iter + find_size;
			dst += replace_size;
		});

		// to end
		output.replace(dst, output.end(),
		               src, str.end());

		return output;
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

		return find_and_replace_iter(fmt, search_string, arg_string);
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