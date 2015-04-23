#pragma once
#ifndef _UTILX_NOEXCEPT_HPP
#define _UTILX_NOEXCEPT_HPP

/**
 *	\file 
 *
 *	\defgroup macros Macros
 *	Contains helper macros.
 *	
 *	\ingroup macros
 *	@{
 */


/**
 *	\def UTILX_NOEXCEPT
 *	Wraps noexcepts into a macro. This can be used to maintain compatibility for compilers that do not support noexcept.
 *	
 */
#ifdef _NOEXCEPT
#	define UTILX_NOEXCEPT _NOEXCEPT
#else
#	define UTILX_NOEXCEPT noexcept
#endif


/*! @} */

#endif