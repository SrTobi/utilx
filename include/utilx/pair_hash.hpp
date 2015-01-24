#pragma once
#ifndef _UTILX_PAIR_HASH_HPP
#define _UTILX_PAIR_HASH_HPP

#include <functional>

namespace std
{
	template<typename Ty1, typename Ty2>
	struct hash<std::pair<Ty1, Ty2>>
		: public std::unary_function<const std::pair<Ty1, Ty2>&, std::size_t>
	{
		std::size_t operator() (const std::pair<Ty1, Ty2>& _p) const
		{
			std::hash<Ty1> h1;
			std::hash<Ty2> h2;

			return h1(_p.first) ^ h2(_p.second);
		}
	};
}


#endif


