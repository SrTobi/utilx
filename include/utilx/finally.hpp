#pragma once
#ifndef _UTILX_FINALLY_HPP
#define _UTILX_FINALLY_HPP


#include <functional>
#include "noexcept.hpp"

namespace utilx {
	class finally
	{
	public:
		template<typename Func>
		finally(Func _func)
			: mFunctor(_func)
		{}

		inline ~finally()
		{
			mFunctor();
		}

		finally(finally&& _old) = delete;
		finally(const finally&) = delete;
		finally& operator=(const finally&) = delete;
	private:
		std::function<void(void)> mFunctor;
	};
}



#endif
