// From http://www.codef00.com/code/Fixed.h
// See also: http://stackoverflow.com/questions/79677/whats-the-best-way-to-do-fixed-point-math

/*
 * Copyright (c) 2008
 * Evan Teran
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appears in all copies and that both the
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the same name not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission. We make no representations about the
 * suitability this software for any purpose. It is provided "as is"
 * without express or implied warranty.
 */

#ifndef _UTILX_FIXED_POINT_HPP
#define _UTILX_FIXED_POINT_HPP

#include <ostream>
#include <exception>
#include <cstddef> // for std::size_t
#include <climits> // for CHAR_BIT
#include <stdint.h>
#include <type_traits>
#include <cmath>

#include <boost/operators.hpp>

namespace utilx {

	template <std::size_t I, std::size_t F>
	class basic_fixed_point;

	namespace detail {
		template <class T>
		struct bit_size {
			static const std::size_t size = sizeof(T) * CHAR_BIT;
		};

		// helper templates to make magic with types :)
		// these allow us to determine resonable types from
		// a desired size, they also let us infer the next largest type
		// from a type which is nice for the division op
		template <std::size_t T>
		struct type_from_size {
			static const bool is_specialized = false;
			typedef void      value_type;
		};

#if defined(__GNUC__) && defined(__x86_64__)
		template <>
		struct type_from_size<128> {
			static const bool           is_specialized = true;
			static const std::size_t    size = 128;
			typedef __int128            value_type;
			typedef type_from_size<128> next_size;
		};
#endif

		template <>
		struct type_from_size < 64 > {
			static const bool           is_specialized = true;
			static const std::size_t    size = 64;
			typedef int64_t             value_type;
			typedef type_from_size<128> next_size;
		};

		template <>
		struct type_from_size < 32 > {
			static const bool          is_specialized = true;
			static const std::size_t   size = 32;
			typedef int32_t            value_type;
			typedef type_from_size<64> next_size;
		};

		template <>
		struct type_from_size < 16 > {
			static const bool          is_specialized = true;
			static const std::size_t   size = 16;
			typedef int16_t            value_type;
			typedef type_from_size<32> next_size;
		};

		template <>
		struct type_from_size < 8 > {
			static const bool          is_specialized = true;
			static const std::size_t   size = 8;
			typedef int8_t             value_type;
			typedef type_from_size<16> next_size;
		};

		// this is to assist in adding support for non-native base
		// types (for adding big-int support), this should be fine
		// unless your bit-int class doesn't nicely support casting
		template<class B, class N>
		B next_to_base(const N& rhs) {
			return static_cast<B>(rhs);
		}

		struct divide_by_zero : std::exception {
		};

		template <std::size_t I, std::size_t F>
		void divide(const basic_fixed_point<I, F> &numerator, const basic_fixed_point<I, F> &denominator, basic_fixed_point<I, F> &quotient, basic_fixed_point<I, F> &remainder, typename boost::enable_if_c<detail::type_from_size<I + F>::next_size::is_specialized>::type* = 0) {

			BOOST_STATIC_ASSERT(detail::type_from_size<I + F>::next_size::is_specialized);

			typedef typename basic_fixed_point<I, F>::next_type next_type;
			typedef typename basic_fixed_point<I, F>::base_type base_type;
			static const std::size_t fractional_bits = basic_fixed_point<I, F>::fractional_bits;

			next_type t(numerator.to_raw());
			t <<= fractional_bits;

			quotient = basic_fixed_point<I, F>::from_raw(detail::next_to_base<base_type>(t / denominator.to_raw()));
			remainder = basic_fixed_point<I, F>::from_raw(detail::next_to_base<base_type>(t % denominator.to_raw()));
		}

		template <std::size_t I, std::size_t F>
		void divide(basic_fixed_point<I, F> numerator, basic_fixed_point<I, F> denominator, basic_fixed_point<I, F> &quotient, basic_fixed_point<I, F> &remainder, typename boost::disable_if_c<detail::type_from_size<I + F>::next_size::is_specialized>::type* = 0) {

			// NOTE: division is broken for large types :-(
			// especially when dealing with negative quantities

			typedef typename basic_fixed_point<I, F>::base_type base_type;
			static const int bits = basic_fixed_point<I, F>::total_bits;

			if (denominator == 0) {
				throw divide_by_zero();
			}
			else {

				int sign = 0;

				if (numerator < 0) {
					sign ^= 1;
					numerator = -numerator;
				}

				if (denominator < 0) {
					sign ^= 1;
					denominator = -denominator;
				}

				base_type n = numerator.to_raw();
				base_type d = denominator.to_raw();
				base_type x = 1;
				base_type answer = 0;


				while ((n >= d) && (((d >> (bits - 1)) & 1) == 0)) {
					x <<= 1;
					d <<= 1;
				}

				while (x != 0) {
					if (n >= d) {
						n -= d;
						answer |= x;
					}

					x >>= 1;
					d >>= 1;
				}

				quotient = answer;
				remainder = n;

				if (sign) {
					quotient = -quotient;
				}
			}
		}

		// this is the usual implementation of multiplication
		template <std::size_t I, std::size_t F>
		void multiply(const basic_fixed_point<I, F> &lhs, const basic_fixed_point<I, F> &rhs, basic_fixed_point<I, F> &result, typename boost::enable_if_c<detail::type_from_size<I + F>::next_size::is_specialized>::type* = 0) {

			BOOST_STATIC_ASSERT(detail::type_from_size<I + F>::next_size::is_specialized);

			typedef typename basic_fixed_point<I, F>::next_type next_type;
			typedef typename basic_fixed_point<I, F>::base_type base_type;

			static const std::size_t fractional_bits = basic_fixed_point<I, F>::fractional_bits;

			next_type t(static_cast<next_type>(lhs.to_raw()) * static_cast<next_type>(rhs.to_raw()));
			t >>= fractional_bits;
			result = basic_fixed_point<I, F>::from_raw(next_to_base<base_type>(t));
		}

		// this is the fall back version we use when we don't have a next size
		// it is slightly slower, but is more robust since it doesn't
		// require and upgraded type
		template <std::size_t I, std::size_t F>
		void multiply(const basic_fixed_point<I, F> &lhs, const basic_fixed_point<I, F> &rhs, basic_fixed_point<I, F> &result, typename boost::disable_if_c<detail::type_from_size<I + F>::next_size::is_specialized>::type* = 0) {

			typedef typename basic_fixed_point<I, F>::base_type base_type;

			static const std::size_t fractional_bits = basic_fixed_point<I, F>::fractional_bits;
			static const std::size_t integer_mask = basic_fixed_point<I, F>::integer_mask;
			static const std::size_t fractional_mask = basic_fixed_point<I, F>::fractional_mask;

			// more costly but doesn't need a larger type
			const base_type a_hi = (lhs.to_raw() & integer_mask) >> fractional_bits;
			const base_type b_hi = (rhs.to_raw() & integer_mask) >> fractional_bits;
			const base_type a_lo = (lhs.to_raw() & fractional_mask);
			const base_type b_lo = (rhs.to_raw() & fractional_mask);

			const base_type x1 = a_hi * b_hi;
			const base_type x2 = a_hi * b_lo;
			const base_type x3 = a_lo * b_hi;
			const base_type x4 = a_lo * b_lo;

			result = basic_fixed_point<I, F>::from_raw((x1 << fractional_bits) + (x3 + x2) + (x4 >> fractional_bits));

		}
	}


	// lets us do things like "typedef utilx::fixed_from_type<int32_t>::fixed_type fixed";
	// NOTE: that we will use a type of equivalent size, not neccessarily the type
	// specified. Should make little to no difference to the user
	template <class T>
	struct fixed_from_type {
		typedef basic_fixed_point<detail::bit_size<T>::size / 2, detail::bit_size<T>::size / 2> fixed_type;
	};


	/*
	 * inheriting from boost::operators enables us to be a drop in replacement for base types
	 * without having to specify all the different versions of operators manually
	 */
	template <std::size_t I, std::size_t F>
	class basic_fixed_point : boost::operators<basic_fixed_point<I, F> > //, boost::shiftable < basic_fixed_point<I, F> >
	{
		static_assert(detail::type_from_size<I + F>::is_specialized, "Fixed<I, F> is not specialized");

	public:
		static const std::size_t fractional_bits = F;
		static const std::size_t integer_bits = I;
		static const std::size_t total_bits = I + F;

		typedef detail::type_from_size<total_bits>             base_type_info;

		typedef typename base_type_info::value_type            base_type;
		typedef typename base_type_info::next_size::value_type next_type;

	public:
		static const std::size_t base_size = base_type_info::size;
		static const base_type fractional_mask = ~((~base_type(0)) << fractional_bits);
		static const base_type integer_mask = ~fractional_mask;

	public:
		static const base_type one = base_type(1) << fractional_bits;

	public: // constructors
		basic_fixed_point() = default;

		template<typename From>
		basic_fixed_point(From n, typename std::enable_if<std::is_convertible<From, base_type>::value && std::is_integral<From>::value>::type* = nullptr)
			: mData(base_type(n) << fractional_bits) {
		}

		template<typename From>
		explicit basic_fixed_point(From n, typename std::enable_if<std::is_convertible<From, base_type>::value && !std::is_integral<From>::value>::type* = nullptr)
			: mData(static_cast<base_type>(n * one)) {

		}

		basic_fixed_point(const basic_fixed_point &o) = default;
		basic_fixed_point& operator=(const basic_fixed_point &o) = default;

	private:
		// this makes it simpler to create a fixed point object from
		// a native type without scaling
		// use "Fixed::from_raw" in order to perform this.
		struct no_scale {};

		basic_fixed_point(base_type n, const no_scale &) : mData(n) {
		}

	public:
		static basic_fixed_point from_raw(base_type n) {
			return basic_fixed_point(n, no_scale());
		}

	public: // comparison operators
		bool operator==(const basic_fixed_point &o) const {
			return mData == o.mData;
		}

		bool operator<(const basic_fixed_point &o) const {
			return mData < o.mData;
		}

	public: // unary operators
		bool operator!() const {
			return !mData;
		}

		basic_fixed_point operator~() const {
			basic_fixed_point t(*this);
			t.mData = ~t.mData;
			return t;
		}

		basic_fixed_point operator-() const {
			basic_fixed_point t(*this);
			t.mData = -t.mData;
			return t;
		}

		basic_fixed_point& operator++() {
			mData += one;
			return *this;
		}

		basic_fixed_point& operator--() {
			mData -= one;
			return *this;
		}

	public: // basic math operators
		basic_fixed_point& operator+=(const basic_fixed_point &n) {
			mData += n.mData;
			return *this;
		}

		basic_fixed_point& operator-=(const basic_fixed_point &n) {
			mData -= n.mData;
			return *this;
		}

		basic_fixed_point& operator&=(const basic_fixed_point &n) {
			mData &= n.mData;
			return *this;
		}

		basic_fixed_point& operator|=(const basic_fixed_point &n) {
			mData |= n.mData;
			return *this;
		}

		basic_fixed_point& operator^=(const basic_fixed_point &n) {
			mData ^= n.mData;
			return *this;
		}

		basic_fixed_point& operator*=(const basic_fixed_point &n) {
			detail::multiply(*this, n, *this);
			return *this;
		}

		basic_fixed_point& operator/=(const basic_fixed_point &n) {
			basic_fixed_point temp;
			detail::divide(*this, n, *this, temp);
			return *this;
		}

		basic_fixed_point& operator>>=(const basic_fixed_point &n) {
			mData >>= n.to_int();
			return *this;
		}

		basic_fixed_point& operator<<=(const basic_fixed_point &n) {
			mData <<= n.to_int();
			return *this;
		}

	public: // conversion to basic types
		template<typename To>
		typename std::enable_if<
			std::is_convertible<base_type, To>::value && std::is_integral<To>::value, To>
		::type
			to() const {
			return static_cast<To>((mData & integer_mask) >> fractional_bits);
		}

		template<typename To>
		typename std::enable_if<
			std::is_convertible<base_type, To>::value && !std::is_integral<To>::value, To>
		::type
			to() const {
			return static_cast<To>(mData) / one;
		}

		unsigned int to_int() const {
			return to<int>();
		}
		
		unsigned int to_uint() const {
			return to<unsigned int>();
		}

		long to_long() const {
			return to<long>();
		}

		float to_float() const {
			return to<float>();
		}

		double to_double() const        {
			return to<double>();
		}

		base_type to_raw() const {
			return mData;
		}

		template<typename To>
		explicit operator To () const
		{
			return to<To>();
		}

		/*operator float () const
		{
			return to<float>();
		}*/

		/*explicte operator double () const
		{
			return to<double>();
		}*/
	public:
		void swap(basic_fixed_point &rhs) {
			using std::swap;
			swap(mData, rhs.mData);
		}

	public:
		base_type mData;
	};

	template <std::size_t I, std::size_t F>
	std::ostream &operator<<(std::ostream &os, const basic_fixed_point<I, F> &f) {
		os << f.to_double();
		return os;
	}

	template <std::size_t I, std::size_t F>
	const std::size_t basic_fixed_point<I, F>::fractional_bits;

	template <std::size_t I, std::size_t F>
	const std::size_t basic_fixed_point<I, F>::integer_bits;

	template <std::size_t I, std::size_t F>
	const std::size_t basic_fixed_point<I, F>::total_bits;

	typedef basic_fixed_point<24, 8> fixed_point;
	typedef basic_fixed_point<48, 16> dfixed_point;
	typedef fixed_point fixp;
	typedef dfixed_point dfixp;

	static_assert(sizeof(fixed_point) == sizeof(fixed_point::base_type), "fixed_point must have the same size as its base type!");
	static_assert(std::is_trivial<fixed_point>::value, "fixed_point must be trivial!");
	static_assert(std::is_standard_layout<fixed_point>::value, "fixed_point must be standard layouted!");
}

namespace std {
	template <std::size_t I, std::size_t F>
	utilx::basic_fixed_point<I, F> abs(utilx::basic_fixed_point<I, F> _fp)
	{
		return _fp.to_raw() < 0 ? -_fp : _fp;
	}

	template <std::size_t I, std::size_t F>
	utilx::basic_fixed_point<I, F> floor(utilx::basic_fixed_point<I, F> _fp)
	{
		return static_cast<typename utilx::basic_fixed_point<I, F>::base_type>(_fp);
	}

	template <std::size_t I, std::size_t F>
	utilx::basic_fixed_point<I, F> sqrt(utilx::basic_fixed_point<I, F> _fp)
	{
		typedef utilx::basic_fixed_point<I, F>	target_type;
		typedef typename target_type::base_type base_type;

		const double source_raw = _fp.to_raw();
		const double source_sqrt = std::sqrt(static_cast<double>(source_raw));
		base_type result_sqrt = static_cast<base_type>(source_sqrt);

		do { ++result_sqrt; } while(result_sqrt * result_sqrt <= source_raw);
		do { --result_sqrt; } while(result_sqrt * result_sqrt > source_raw);

		return target_type::from_raw(result_sqrt * (target_type::fractional_bits << 1));
	}
}
#endif
