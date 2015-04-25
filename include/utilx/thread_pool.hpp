#pragma once
#ifndef _UTILX_THREAD_POOL_HPP
#define _UTILX_THREAD_POOL_HPP


#include <thread>
#include <atomic>
#include <map>
#include <condition_variable>


namespace utilx {
	class thread_pool
	{
	public:
		typedef std::function<bool()> func_type;
	public:
		thread_pool(func_type func, std::size_t threadCount = 1);
		~thread_pool();

		std::size_t thread_count() const;

		std::size_t working_count() const;
		void thread_count(std::size_t count);
		void start();
		void stop();
		void finish();
		void destroy();

		bool processing() const;
	private:
		void _run_thread();
		void _start_thread();

	private:
		std::mutex mMutex;
		std::condition_variable mNotifier;
		std::atomic<size_t> mTargetThreadCount;
		std::map<std::thread::id, std::thread> mThreads;

		const func_type mFunc;
		std::atomic<std::size_t> mWorkingThreads;
		bool mDestroy;
		std::atomic<bool> mRun;
	};
}




#endif