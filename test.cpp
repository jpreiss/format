#include "format.hpp"

#include <cassert>

template <typename T>
void check_throws(char const *fmt, T const &arg)
{
	bool throwed = false;
	try
	{
		std::string s = format(fmt, arg);
	}
	catch (FormatStringException &ex)
	{
		throwed = true;
	}
	assert(throwed);
}

int main()
{
	std::string s;

	s = format("", 12, "null");
	assert(s == "");

	s = format("o hai", 42);
	assert(s == "o hai");

	s = format("i can has {0}", "formatting");
	assert(s == "i can has formatting");

	s = format("{0} {0}", 2.5);
	assert(s == "2.5 2.5");

	s = format("{0} {1}", 0, 1);
	assert(s == "0 1");

	s = format("{1} {0} {1}", "right", "left");
	assert(s == "left right left");

	s = format("{0} {1} {2} {1} {2} {3}", 0, 1, 2, 3);
	assert(s == "0 1 2 1 2 3");

	check_throws("{", 0);

	check_throws("{{{", 0);
	
	check_throws("{}}", 0);

	check_throws("}}}}}", 0);

	check_throws("{hello}", 0);

	check_throws("{0hi}", 0);

	check_throws("{1}", 0);

	check_throws("{12hello}", 0);

	check_throws("{0{1}}", 0);
}
