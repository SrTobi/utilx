#pragma once
#ifndef _UTILX_FIXED_POINT_GLM_HPP
#define _UTILX_FIXED_POINT_GLM_HPP

#include "fixed_point.hpp"
#include "vector.hpp"

namespace utilx {
	typedef glm::tvec2<fixp> fixvec2;
	typedef glm::tvec2<dfixp> dfixvec2;
}

namespace glm {

	// length
	template<std::size_t I, std::size_t F>
	GLM_FUNC_QUALIFIER utilx::basic_fixed_point<I, F> length(utilx::basic_fixed_point<I, F> const & v)
	{
		return std::abs(v);
	}
	
	template <std::size_t I, std::size_t F, precision P, template <typename, precision> class vecType>
	GLM_FUNC_QUALIFIER utilx::basic_fixed_point<I, F> length(vecType<utilx::basic_fixed_point<I, F>, P> const & v)
	{
		return std::sqrt(dot(v, v));
	}

	// distance
	template <std::size_t I, std::size_t F>
	GLM_FUNC_QUALIFIER utilx::basic_fixed_point<I, F> distance(utilx::basic_fixed_point<I, F> const & p0, utilx::basic_fixed_point<I, F> const & p1)
	{
		return length(p1 - p0);
	}

	template <std::size_t I, std::size_t F, precision P, template <typename, precision> class vecType>
	GLM_FUNC_QUALIFIER utilx::basic_fixed_point<I, F> distance(vecType<utilx::basic_fixed_point<I, F>, P> const & p0, vecType<utilx::basic_fixed_point<I, F>, P> const & p1)
	{
		return length(p1 - p0);
	}

	// dot
	template <std::size_t I, std::size_t F>
	GLM_FUNC_QUALIFIER utilx::basic_fixed_point<I, F> dot(utilx::basic_fixed_point<I, F> const & x, utilx::basic_fixed_point<I, F> const & y)
	{
		return x * y;
	}

	template <std::size_t I, std::size_t F, precision P, template <typename, precision> class vecType>
	GLM_FUNC_QUALIFIER utilx::basic_fixed_point<I, F> dot(vecType<utilx::basic_fixed_point<I, F>, P> const & x, vecType<utilx::basic_fixed_point<I, F>, P> const & y)
	{
		return detail::compute_dot<vecType, utilx::basic_fixed_point<I, F>, P>::call(x, y);
	}

	// normalize
	template <std::size_t I, std::size_t F>
	GLM_FUNC_QUALIFIER utilx::basic_fixed_point<I, F> normalize(utilx::basic_fixed_point<I, F> const & x)
	{
		typedef utilx::basic_fixed_point<I, F> target_type;
		return x < target_type(0) ? target_type(-1) : target_type(1);
	}

	template <std::size_t I, std::size_t F, precision P, template <typename, precision> class vecType>
	GLM_FUNC_QUALIFIER vecType<utilx::basic_fixed_point<I, F>, P> normalize(vecType<utilx::basic_fixed_point<I, F>, P> const & x)
	{
		return x * inversesqrt(dot(x, x));
	}
	
}


#endif