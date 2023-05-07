#include <iostream>
#include <queue>
#include <vector>
#include <thread>
#include <atomic>
#include <functional>
#include "queue.cpp"

class join_threads
{
	std::vector<std::thread>& threads;
public:
	explicit join_threads(std::vector<std::thread>& threads_) :
		threads(threads_)
	{}
	~join_threads()
	{
		for (unsigned long i = 0; i < threads.size(); ++i)
		{
			if (threads[i].joinable())
				threads[i].join();
		}
	}
};

class thread_pool
{
protected:
	//const int pool_num = std::thread::hardware_concurrency();
	std::vector<std::thread> pool_threads;
	safe_queue<std::function<void()>> pool_queue;
	std::atomic_bool done;
	join_threads joiner;

	void work()
	{
		while (done)
		{
			std::function<void()> task;
			if (pool_queue.try_pop(task))
			{
				task();
			}
			else
			{
				std::this_thread::yield();
			}
		}
	}

public:
	thread_pool() : done(false), joiner(pool_threads)
	{
		const int pool_num = std::thread::hardware_concurrency();
		
		try
		{
			for (int i = 0; i < pool_num; ++i)
			{
				pool_threads.push_back(std::thread(&thread_pool::work, this));
			}
		}
		catch (...)
		{
			done = true;
			throw;
		}
	}

	~thread_pool()
	{
		done = true;
	}	

	template <typename T>
	void submit(T func)
	{
		pool_queue.push(std::function<void()>(func));
	}
};