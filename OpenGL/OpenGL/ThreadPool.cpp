#include"ThreadPool.h"
ThreadPool::ThreadPool(size_t threads): stop(false)
{
	
	for (int i = 0; i < threads; i++) {

		workers.emplace_back([this]() 
			{
				while (true)
				{
					std::function<void()> task;
					{
						std::unique_lock<std::mutex> lock(this->queue_mutex);//这个锁是为了访问查询队列而设置的锁而非执行而设置的锁
						this->condition.wait(lock, [this]() {return this->stop || !this->tasks.empty(); }); //后面条件为false才会阻塞线程
						if (this->stop && this->tasks.empty())
							return;
						task = std::move(this->tasks.front());
						this->tasks.pop();
					}
					task();


				}


			});





	}


}

ThreadPool::~ThreadPool()
{
	{
		std::unique_lock<std::mutex> lock(queue_mutex);
		stop = true;
	}
	condition.notify_all();// 唤醒所有线程，清理任务
	for (std::thread& worker : workers)
		worker.join();// 阻塞，等待所有线程执行结束
}

//template<typename F, typename ...Args>
// auto ThreadPool::enqueue(F&& f, Args && ...args)
//{
//	using return_type = std::invoke_result_t<F, Args...>;
//	auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f),std::forward(args)...));
//	std::future<return_type>res = task->get_future();
//	{
//		std::unique_lock<std::mutex> lock(queue_mutex);
//		if (stop) {
//
//			throw std::runtime_error("enqueue on stopped Thread pool");
//		}
//		tasks.emplace([task = std::move(task)](){(*task)()});
//
//
//
//	}
//	condition.notify_one();
//	return res;
//
//
//}
