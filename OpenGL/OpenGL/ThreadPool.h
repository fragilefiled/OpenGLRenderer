#pragma once
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <type_traits>
#include <stdexcept>

class ThreadPool
{

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;

public:
	explicit ThreadPool(size_t threads=std::thread::hardware_concurrency());

	//template<typename F, typename... Args>
	//auto enqueue(F&& f, Args &&...args);//右值引用 和Const F& 类似
	template<typename F, typename... Args>
	auto enqueue(F&& f, Args &&...args)
	{

		using return_type = std::invoke_result_t<F, Args...>;
		auto task = std::make_shared<std::packaged_task<return_type()>>(
			std::bind(std::forward<F>(f), std::forward<Args>(args)...));
		std::future<return_type> res = task->get_future();
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			if (stop) {

				throw std::runtime_error("enqueue on stopped Thread pool");
			}
			tasks.emplace([task = std::move(task)]() { (*task)(); });



		}
		condition.notify_one();
		return res;


	}


	~ThreadPool();






};


