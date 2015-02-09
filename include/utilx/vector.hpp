#pragma once
#ifndef _VECTOR_HPP
#define _VECTOR_HPP

#include <sstream>
#include <glm/glm.hpp>

namespace utilx {

	typedef glm::vec2	vec2;
	typedef glm::ivec2	point;
	typedef glm::uvec2	rect;
}

namespace glm {

	template<typename C, typename T, enum glm::precision P>
	std::basic_ostream<C>& operator << (std::basic_ostream<C>& os, const tvec2<T, P>& v)
	{
		os << v.x << C(',') << C(' ') << v.y;
		return os;
	}
}
#endif
