/*

Format
Curly brace string formatting with braces, like Python and C#.

James Preiss, 2013.  Public domain.

*/

#ifndef _FORMAT_H_
#define _FORMAT_H_

#include <string>
#include <sstream>
#include <exception>

class FormatStringException : public std::runtime_error
{
public:
	FormatStringException(char const *msg) : std::runtime_error(msg) {}
};

namespace
{
	bool is_digit(char c)
	{
		return ('0' <= c) && (c <= '9');
	}

	int to_digit(char c)
	{
		return c - '0';
	}

	/*
	concept TraverseHandler
	{
		void on_marker(int marker_number);
		void on_escape_left();
		void on_escape_right();
		void on_chars(char *chars, int count);
	}
	*/

	template <int N>
	class Counter
	{
	public:
		int count[N];
		int brace_escapes;
		int plain_chars;

		Counter() : brace_escapes(0), plain_chars(0)
		{ 
			memset(count, 0, sizeof(int) * N);
		}

		void on_marker(int marker_number)
		{ 
			count[marker_number] += 1;
		}

		void on_escape_left() { ++brace_escapes; }

		void on_escape_right() { ++brace_escapes; }

		// inlined.  verified in ASM
		void on_char(char c) { ++plain_chars; }
	};

	template <int N>
	class Formatter
	{
	public:
		// assumes dst is already the right size
		Formatter(std::string &destination, std::string values[N]) : 
			dest(destination),
			i(0),
			vals(values)
		{
		}

		void on_marker(int marker_number)
		{
			size_t len = vals[marker_number].length();
			dest.replace(i, len, vals[marker_number]);
			i += len;
		}

		void on_escape_left()
		{
			dest[i] = '{';
			++i;
		}

		void on_escape_right()
		{
			dest[i] = '}';
			++i;
		}

		// inlined.  verified in ASM
		void on_char(char c)
		{
			dest[i] = c;
			++i;
		}

	private:
		std::string &dest;
		size_t i;
		std::string *vals;
	};

	template <int N, typename Handler>
	int traverse(char const *fmt, Handler &callback)
	{
		char const *c = fmt;
		while (*c != '\0')
		{
			if (*c == '{')
			{
				// lookahead for escaped braces
				if (c[1] == '{')
				{
					callback.on_escape_left();
					c += 2;
				}
				else
				{
					int number = 0;
					++c;

					if (*c == '\0')
						throw FormatStringException("Unexpected end of string");

					// must be at least one number
					if (!is_digit(*c))
						throw FormatStringException("Invalid brace contents: must be positive integer");

					number = to_digit(*c);
					++c;

					while (is_digit(*c))
					{
						number *= 10;
						number += to_digit(*c);
						++c;
					}

					if (*c == '\0')
						throw FormatStringException("Unexpected end of string");

					if (*c != '}')
						throw FormatStringException("Invalid brace contents: must be positive integer");

					if (number >= N)
						throw FormatStringException("Invalid format value number");

					callback.on_marker(number);

					++c;
				}
			}
			else if (*c == '}')
			{
				if (c[1] == '}')
				{
					callback.on_escape_right();
					c += 2;
				}
				else
				{
					throw FormatStringException("Un-escaped right brace");
				}
			}
			else
			{
				callback.on_char(*c);
				++c;
			}
		}
	}

	template <int N>
	size_t formatted_total(std::string *values, int *counts)
	{
		size_t total = 0;
		for (int i = 0; i < N; ++i)
		{
			total += values[i].length() * counts[i];
		}
		return total;
	}

	template <int N>
	std::string format_array(char const *format, std::string values[N])
	{
		Counter<N> counter;
		traverse<N>(format, counter);

		size_t formats_size = formatted_total<N>(values, counter.count);
		size_t output_total = formats_size + counter.brace_escapes + counter.plain_chars;

		std::string output(output_total, '\0');
		Formatter<N> formatter(output, values);
		traverse<N>(format, formatter);

		return output;
	}

	template <typename T>
	void add(std::stringstream &ss, std::string values[], T const &value, int index)
	{
		ss.str("");
		ss << value;
		values[index] = ss.str();
	}
}


//
// should be a variadic template, but VS2010.
//
template <typename T>
std::string format(char const *fmt, T const &value)
{
	std::string values[1];
	std::stringstream ss;
	add(ss, values, value, 0);
	return format_array<1>(fmt, values);
}

template <typename T0, typename T1>
std::string format(char const *fmt, T0 const &v0, T1 const &v1)
{
	std::string values[2];
	std::stringstream ss;
	add(ss, values, v0, 0);
	add(ss, values, v1, 1);
	return format_array<2>(fmt, values);
}

template <typename T0, typename T1, typename T2>
std::string format(char const *fmt, T0 const &v0, T1 const &v1, T2 const &v2)
{
	std::string values[3];
	std::stringstream ss;
	add(ss, values, v0, 0);
	add(ss, values, v1, 1);
	add(ss, values, v2, 2);
	return format_array<3>(fmt, values);
}

template <typename T0, typename T1, typename T2, typename T3>
std::string format(char const *fmt, T0 const &v0, T1 const &v1, T2 const &v2, T3 const &v3)
{
	std::string values[4];
	std::stringstream ss;
	add(ss, values, v0, 0);
	add(ss, values, v1, 1);
	add(ss, values, v2, 2);
	add(ss, values, v3, 3);
	return format_array<4>(fmt, values);
}

template <typename T0, typename T1, typename T2, typename T3, typename T4>
std::string format(char const *fmt, T0 const &v0, T1 const &v1, T2 const &v2, T3 const &v3, T4 const &v4)
{
	std::string values[5];
	std::stringstream ss;
	add(ss, values, v0, 0);
	add(ss, values, v1, 1);
	add(ss, values, v2, 2);
	add(ss, values, v3, 3);
	add(ss, values, v4, 4);
	return format_array<5>(fmt, values);
}

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
std::string format(char const *fmt, T0 const &v0, T1 const &v1, T2 const &v2, T3 const &v3, T4 const &v4, T5 const &v5)
{
	std::string values[6];
	std::stringstream ss;
	add(ss, values, v0, 0);
	add(ss, values, v1, 1);
	add(ss, values, v2, 2);
	add(ss, values, v3, 3);
	add(ss, values, v4, 4);
	add(ss, values, v5, 5);
	return format_array<6>(fmt, values);
}

//
// six format arguments ought to be enough for anyone
//

#endif // _FORMAT_H_