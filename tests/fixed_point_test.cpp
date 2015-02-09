#include <iostream>
#include <glm/glm.hpp>

#include "testx/testx.hpp"
#include "utilx/fixed_point.hpp"

using namespace utilx;

namespace fixed_point_tests {


	TESTX_AUTO_TEST_CASE(_check_fixed_type)
	{
		fixed_point f = 3;

		int i = (int)f;

		std::cout << f;
	}

	TESTX_AUTO_TEST_CASE(_check_constructors)
	{
		{
			utilx::fixed_point f1(3);
			utilx::fixed_point f2(long(3));
			utilx::fixed_point f3((unsigned int)(3));
			utilx::fixed_point f5(3.2);
			utilx::fixed_point f6(3.2f);
		}

		{
			utilx::fixed_point f1 = 3;
			utilx::fixed_point f2 = long(3);
			utilx::fixed_point f3 = unsigned int(3);
			utilx::fixed_point f5 = static_cast<fixed_point>(3.2);
			utilx::fixed_point f6 = static_cast<fixed_point>(3.2f);
		}
	}

	TESTX_AUTO_TEST_CASE(_check_casts)
	{
		utilx::fixed_point f = 7;

		{
			/*BOOST_CHECK_EQUAL((int)f, 3);
			BOOST_CHECK_EQUAL((long)f, 3);
			BOOST_CHECK_EQUAL((unsigned int)f, 3);*/
		}

		{
			auto x = std::floor(f / fixp(3));
			std::cout << x;
		}

	}
}