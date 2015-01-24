#pragma once
#ifndef _UTILX_UID_HPP
#define _UTILX_UID_HPP

#include <atomic>
#include <limits>
#include <random>

namespace utilx {
	template<typename>
	class UIDGen;

	template<typename _Ty = std::size_t, std::size_t _N = ((sizeof(_Ty) * 8) / 4)>
	class UID
	{
		friend class UIDGen<UID>;
	public:
		bool operator ==(const UID& _other) const
		{
			return mId == _other.mId;
		}

		bool operator !=(const UID& _other) const
		{
			return mId != _other.mId;
		}

		bool operator <(const UID& _other) const
		{
			return mId < _other.mId;
		}

		bool operator >(const UID& _other) const
		{
			return mId > _other.mId;
		}

		bool operator >=(const UID& _other) const
		{
			return mId >= _other.mId;
		}

		bool operator <=(const UID& _other) const
		{
			return mId <= _other.mId;
		}

		static std::size_t MaxNoise()
		{
			return std::numeric_limits<_Ty>::max() >> (sizeof(_Ty)* 8 - _N);
		}

		static UID New() { return UID(++Next); }
	private:
		UID(_Ty _id)
			: mId(_id)
		{
		}
	private:
		_Ty mId;
		static std::atomic<_Ty> Next;
	};

	template<typename _Ty, std::size_t _N>
	std::atomic<_Ty> UID<_Ty, _N>::Next = ATOMIC_VAR_INIT(0);



	typedef UID<> uid;
}

#endif