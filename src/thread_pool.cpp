#include <map>
#include <iostream>
#include <assert.h>
//#include <boost/exception/all.hpp>

#include "utilx/thread_pool.hpp"
#include "utilx/finally.hpp"

namespace utilx {

	ThreadPool::ThreadPool(func_type _func)
		: mTargetThreadCount(1)
		, mRun(false)
		, mDestroy(false)
		, mFunc(_func)
	{
	}

	ThreadPool::~ThreadPool()
	{
		stop();
		finish();
		destroy();
		/*
		{
		// Hmmm, a lock will cause an exception on exit cleanup
		// std::lock_guard<std::mutex> lock(mMutex);
		mDestroy = true;
		mWork.reset();
		mService.stop();
		}

		for(auto& p : mThreads)
		{
		axAssert(p.second.joinable());
		p.second.join();
		}*/
	}

	void ThreadPool::setThreadCount(size_t count)
	{
		std::lock_guard<std::mutex> lock(mMutex);
		if (mTargetThreadCount == count)
			return;

		bool addThreads = mTargetThreadCount < count;

		mTargetThreadCount = count;
		//log.info() << "thread count is now " << mTargetThreadCount;
	}


	void ThreadPool::start()
	{
		std::lock_guard<std::mutex> lck(mMutex);
		mRun = true;
		mNotifier.notify_all();
		_start_thread();
	}

	void ThreadPool::stop()
	{
		std::lock_guard<std::mutex> lck(mMutex);
		mRun = false;
	}

	void ThreadPool::finish()
	{
		std::unique_lock<std::mutex> lck(mMutex);
		mNotifier.wait(lck, [this]{return mWorkingThreads == 0 && !mRun; });
	}

	void ThreadPool::destroy()
	{
		{
			std::lock_guard<std::mutex> lock(mMutex);
			mDestroy = true;
			mNotifier.notify_all();
		}

		for (auto& p : mThreads)
		{
			assert(p.second.joinable());
			p.second.join();
		}
	}


	bool ThreadPool::processing() const
	{
		return mWorkingThreads > 0 || mRun;
	}


	void ThreadPool::_run_thread()
	{
		finally unregister = [this](){
			std::lock_guard<std::mutex> lck(mMutex);
			if (!mDestroy) {
				auto it = mThreads.find(std::this_thread::get_id());
				assert(it != mThreads.end());
				it->second.detach();
				mThreads.erase(it);
			}
		};

		try {
			while (true)
			{
				{
					std::unique_lock<std::mutex> lck(mMutex);
					_start_thread();
					mNotifier.wait(lck, [this]{return mRun || mDestroy; });
				}

				if (mDestroy)
					break;

				++mWorkingThreads;
				bool run = true;
				while (run)
				{
					run = mFunc();
				}
				std::lock_guard<std::mutex> lck(mMutex);
				mRun = false;

				if (--mWorkingThreads == 0)
				{
					mNotifier.notify_all();
				}
				if (mThreads.size() > mTargetThreadCount)
					break;
			}
		}
		catch (...)
		{
			std::cerr
				<< "Exception in worker thread!"
				<< "\n------------------------\n"
				//<< boost::current_exception_diagnostic_information()
				<< "\n------------------------";

#ifdef TILENET_RETHROW_THREAD_EXCEPTIONS
			throw;
#endif
		}
	}

	void ThreadPool::_start_thread()
	{
		if (mTargetThreadCount > mThreads.size() && !mDestroy)
		{
			std::thread thread(std::bind(&ThreadPool::_run_thread, this));
			mThreads.emplace(thread.get_id(), std::move(thread));
		}
	}

}
