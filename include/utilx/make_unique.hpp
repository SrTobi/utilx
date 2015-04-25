#pragma once
#ifndef _UTILX_MAKE_UNIQUE_HPP
#define _UTILX_MAKE_UNIQUE_HPP

#include <memory>
#include <type_traits>
#include <utility>

#ifdef _MSC_VER
#	define UTILX_FEATURES_HAS_MAKE_UNIQUE 1
#else
#	define UTILX_FEATURES_HAS_MAKE_UNIQUE 0
#endif


#if !UTILX_FEATURES_HAS_MAKE_UNIQUE

namespace std {

	namespace detail {
		template <typename T, typename... Args>
		std::unique_ptr<T> make_unique_helper(std::false_type, Args&&... args) {
			return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
		}

		template <typename T, typename... Args>
		std::unique_ptr<T> make_unique_helper(std::true_type, Args&&... args) {
			static_assert(std::extent<T>::value == 0,
				"make_unique<T[N]>() is forbidden, please use make_unique<T[]>().");

			typedef typename std::remove_extent<T>::type U;
			return std::unique_ptr<T>(new U[sizeof...(Args)]{std::forward<Args>(args)...});
		}

	}

}

#endif



namespace utilx {
#if UTILX_FEATURES_HAS_MAKE_UNIQUE && !defined(DOXYGEN_DOCS)
	using std::make_unique;
#else
	template <typename T, typename... Args>
	std::unique_ptr<T> make_unique(Args&&... args) {
		return detail::make_unique_helper<T>(std::is_array<T>(), std::forward<Args>(args)...);
	}
#endif
}

#endif
