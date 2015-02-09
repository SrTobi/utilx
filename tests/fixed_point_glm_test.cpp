#include <iostream>
#include "testx/testx.hpp"
#include "utilx/fixed_point_glm.hpp"


namespace fixed_point_glm_tests {

	TESTX_AUTO_TEST_CASE(_check_glm_functions)
	{
		std::cout << sizeof(utilx::dfixed_point) << std::endl;
		utilx::fixp fp(2);
		utilx::fixvec2 vec(1, 3);

		BOOST_CHECK_EQUAL(vec, utilx::fixvec2(1, 3));

		auto a1 = glm::distance(fp, fp);
		auto a2 = glm::distance(vec, vec);
		auto b1 = glm::length(fp);
		auto b2 = glm::length(vec);
		auto c1 = glm::dot(fp, fp);
		auto c2 = glm::dot(vec, vec);
		auto d1 = glm::normalize(fp);
		auto d2 = glm::normalize(vec);
		auto e = glm::length(d2);

		float _a1(a1);
		float _a2(a2);
		float _b1(b1);
		float _b2(b2);
		float len = glm::length(glm::vec2(vec));
		float _c1(c1);
		float _c2(c2);
		float _d1(d1);
		glm::vec2 _d2(d2);
		float _e(e);
	}
}