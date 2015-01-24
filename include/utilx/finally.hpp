#pragma once
#ifndef _UTILX_FINALLY_HPP
#define _UTILX_FINALLY_HPP


#include <functional>

namespace utilx {
	class finally
	{
		std::function<void(void)> functor;
	public:
		template<typename Func>
		finally(Func _func)
			: functor(_func)
		{}

		~finally()
		{
			functor();
		}
	};
}



#endif
