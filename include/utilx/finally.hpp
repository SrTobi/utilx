#pragma once
#ifndef _UTILX_FINALLY_HPP
#define _UTILX_FINALLY_HPP

/** \file */

#include <functional>
#include "noexcept.hpp"


namespace utilx {
	/**
	 *	\ingroup utils
	 *	\brief A helper class to execute code, when exiting the scope.
	 *	
	 *	finally calls a given function, when the scope in which it was defined
	 *	ends. In contrast to a function
	 *	that is called at the end of a block directly, finally also gets called
	 *	when the block is exited at an unexpected location (i.e. a exception was thrown).
	 *	In the following example `Scope exited!` will be printed, even if `other_func()` throws an exception.
	 *	
	 *		void foo()
	 *		{
	 *			finally f = []() {
	 *				std::cout << "Scope exited!" << std::endl;
	 *			};
	 *			
	 *			...
	 *			
	 *			other_func();
	 *			
	 *			...
	 *		}
	 *		
	 *	\note 
	 *		Note that two or more finally will be executed in the inverse order as they where constructed.
	 *		
	 *			{
	 *				finally b = ...;
	 *				finally a = ...;
	 *				
	 *				// a will be executed
	 *				// b will be executed
	 *			}
	 *		
	 *
	 *	\note
	 *		Note that finally can neither be copied nor be moved.
	 */
	class finally
	{
	public:
		/**
		 *	\brief Constructs finally from a given function.
		 *	
		 *	The given function must be convertible to `std::function< void() >`.
		 *
		 *	\param _func	a handler that is called when finally gets destroyed.
		 */
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
