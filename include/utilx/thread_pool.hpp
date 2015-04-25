#pragma once
#ifndef _UTILX_THREAD_POOL_HPP
#define _UTILX_THREAD_POOL_HPP

/** \file */

#include <thread>
#include <atomic>
#include <map>
#include <condition_variable>


namespace utilx {
	/**
	 * \ingroup utils
	 * \brief Creates and manages a pool of threads.
	 *
	 * All created threads will execute a function given at the creation time of the pool.
	 * The number of threads can be changed dynamically. The handler that is called by
	 * the threads must be convertible to `std::function<bool()>`. If one handler returns
	 * false, the thread pool is stopped.
	 */
	class thread_pool
	{
	public:
		/**
		 * \brief Type of the handler called by every created thread.
		 *
		 */
		typedef std::function<bool()> func_type;
	public:
		/**
		 * \brief Creates a new thread pool.
		 *
		 * \param func The function that is executed by the created threads.
		 * \param threadCount Number of threads created and managed by this pool.
		 *
		 * \note Note that this function will not start the the thread pool. To start call thread_pool:start().
		 */
		thread_pool(func_type func, std::size_t threadCount = 1);
		~thread_pool();

		/**
		 * \brief Returns the number of created threads.
		 *
		 * \return number of created threads.
		 *
		 */
		std::size_t thread_count() const;

		/**
		 * \brief Returns the number of active threads
		 *
		 * \return number of active threads
		 *
		 */
		std::size_t working_count() const;

		/**
		 * \brief Sets the number of active threads.
		 *
		 * Increases or decreases the number of active threads.
		 * This will not stop or destroy any thread actively.
		 * 
		 * \param count The new number of threads.
		 * \note Note that this function will not start the the thread pool. To start call thread_pool:start().
		 *
		 */
		void thread_count(std::size_t count);

		/**
		 * \brief Starts the thread pool.
		 *
		 * Creates new threads that will execute the given function.
		 *
		 */
		void start();

		/**
		 * \brief Stops all threads.
		 *
		 * Marks all threads to stop and returns immediately.
		 * This will not stop or destroy any thread actively.
		 *
		 */
		void stop();

		/**
		 * \brief Waits until all threads have finished their work.
		 * 
		 * \note Note that this function will not stop the thread pool! To stop call thread_pool::stop().
		 *
		 */
		void finish();

		/**
		 * \brief Stops and destroys all threads.
		 *
		 * This will not stop or destroy any thread actively.
		 * It will wait until all threads have finished their work.
		 * After that all threads will be destroyed.
		 *
		 */
		void destroy();

		/**
		 * \brief Returns if there should be and is any active thread.
		 *
		 * \return if there should be and is any active thread.
		 *
		 */
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