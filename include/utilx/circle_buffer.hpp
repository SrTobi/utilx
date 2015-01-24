#pragma once
#ifndef _UTILX_CIRCLE_BUFFER_HPP
#define _UTILX_CIRCLE_BUFFER_HPP

#include <vector>

namespace utilx{

	template<typename T, typename Alloc = std::allocator<T>>
	class CircleBuffer
	{
	public:
		typedef T value_type;
		typedef std::vector<T, Alloc> back_type;
	public:
		CircleBuffer()
			: mOffset(0)
		{
		}

		T& operator[](std::size_t idx)
		{
			return at(idx);
		}

		T& at(std::size_t idx)
		{
			axAssert(idx >= 0 && idx < mBuffer.size());
			return mBuffer.at((idx + mOffset) % mBuffer.size());
		}

		const T& at(std::size_t idx) const
		{
			axAssert(idx >= 0 && idx < mBuffer.size());
			return mBuffer.at((idx + mOffset) % mBuffer.size());
		}

		void push(const T& _val)
		{
			axAssert(mOffset == 0);
			mBuffer.push_back(_val);
		}

		template<typename... Args>
		T& emplace(Args&&... args)
		{
			mBuffer.emplace_back(args...);
			return mBuffer.back();
		}

		void pop_and_push(const T& _Val)
		{
			T* slot = &at(0);
			Alloc alloc;
			alloc.destroy(slot);
			alloc.construct(slot, _Val);
			++mOffset;
		}

		template<typename... Args>
		void pop_and_emplace(Args&&... args)
		{
			T* slot = &at(0);
			Alloc alloc;
			alloc.destroy(slot);
			alloc.construct(slot, args...);
			++mOffset;
		}

		std::size_t size() const
		{
			return mBuffer.size();
		}

		bool empty() const
		{
			return mBuffer.empty();
		}

		T&  front()
		{
			return at(0);
		}

		const T&  front() const
		{
			return at(0);
		}


		T&  back()
		{
			return at(mBuffer.size() - 1);
		}


		const T&  back() const
		{
			return at(mBuffer.size() - 1);
		}

	private:
		std::size_t mOffset;
		std::vector<T, Alloc> mBuffer;
	};
}





#endif // !_CIRCLE_BUFFER_HPP
