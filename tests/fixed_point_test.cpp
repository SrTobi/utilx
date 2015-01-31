#include <iostream>

#include "testx/testx.hpp"
#include "utilx/fixed_point.hpp"


TESTX_AUTO_TEST_CASE(_check_glm_types)
{
	numeric::Fixed<24, 8> f = 3;
	numeric::Fixed<24, 8> g = 4;

	std::cout << f / g;
}