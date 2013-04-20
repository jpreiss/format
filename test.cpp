#include "format.hpp"

#include <cassert>

int main()
{
	std::string s;

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
}