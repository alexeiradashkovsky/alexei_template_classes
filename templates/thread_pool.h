#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

/*
MIT License

Copyright (c) 2018 Alexei Radashkovsky

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/**
 *  @file thread_pool.h
 *  @brief Initiliaze all threads that provided what for task to apear
 *
 *  @details support only from c++11
 *
 *  @author Author Alexei Radashkovsky (alexeirada@gmail.com)
 *  @Update by Alexei Radashkovsky on 07/27/18
 *  @bug no bugs known.
 */

#include <vector>       // std::vector
#include <thread>       // std::thread
#include <chrono>       // std::chrono::milliseconds
#include <tuple>        // std::pair
#include <functional>   // std::function
#include <algorithm>    // std::for_each
#include "safe_queue.h" // alexei_prog_snob::SafePriorityQueue

namespace alexei_prog_snob {

template<typename Task = std::function<void()> >
class ThreadPool {
public:
    enum thread_pool_priority_enum {
        LOW = 0,
        MID,
        HIGH,
        LAST_PRIORITY
    };

    typedef std::pair<Task,thread_pool_priority_enum> Task_Priority_Pair;
    
    ThreadPool();
    ~ThreadPool();

    // uncopyable
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool(const ThreadPool&&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    /**
     * @brief initialize all threads
     * @param[in] _num_of_threads_in_the_system : number of threads to initialize.
     */
    void InitThreadPool(size_t _num_of_threads_in_the_system);

    /**
     * @brief add task to operate
     * @param[in] _priority : task priority.
     * @param[in] _task     : task to preforme.
     */
    void SubmitTask(thread_pool_priority_enum _priority, Task _task);

    /**
     * @brief add task to operate
     * @param[in] _priority_task : pair of task and priority.
     */
    void SubmitTask(const Task_Priority_Pair& _priority_task);

    /**
     * @brief terminate all thread
     */
    void Shutdown();

    /**
     * @brief terminate all thread
     * @param[in] _timeout_ms : time to wait before shutdown.
     */
    void Shutdown(size_t _timeout_ms);
private:
    bool m_terminate;

    struct CompairPriority {
        bool operator()(const Task_Priority_Pair& _lhs, const Task_Priority_Pair& _rhs) {
            return _lhs.second < _rhs.second;
        }
    };

    alexei_prog_snob::SafePriorityQueue<
    Task_Priority_Pair,
    std::vector<Task_Priority_Pair >,
    CompairPriority
    > m_task_container;

    std::vector<std::thread> m_threadContainer;
};

template<typename Task>
ThreadPool<Task>::ThreadPool()
:m_terminate(true) {
}

template<typename Task>
ThreadPool<Task>::~ThreadPool() {
    if(m_terminate == false) {
        Shutdown();
    }
}

template<typename Task>
void ThreadPool<Task>::InitThreadPool(size_t _num_of_threads_in_the_system) {
    m_terminate = false;
    auto get_tasks_in_threads = [this](){
        while (m_terminate != true) {
            Task_Priority_Pair newTask = std::move(m_task_container.Pop());
            newTask.first();
        }
    };

    for (size_t i = 0; i < _num_of_threads_in_the_system ; ++i) {
        m_threadContainer.emplace_back(get_tasks_in_threads);
    }
}

template<typename Task>
void ThreadPool<Task>::SubmitTask(thread_pool_priority_enum _priority, Task _task) {
    Task_Priority_Pair newPair(_task, _priority);
    SubmitTask(std::move(newPair));
}

template<typename Task>
void ThreadPool<Task>::SubmitTask(const Task_Priority_Pair& _priority_task) {
    m_task_container.Push(_priority_task);
}

template<typename Task>
void ThreadPool<Task>::Shutdown() {
    m_terminate = true;
    auto join_all_threads = [this](std::thread& _nextThread)->void{_nextThread.join();};
    auto kill_all_threads = [this]()->void{
        auto empty_task = [](){return;};
        SubmitTask(HIGH, empty_task);
    };
    
    for (size_t i = 0 ; i < m_threadContainer.size(); ++i) {
        kill_all_threads();
    }

    std::for_each(m_threadContainer.begin(), m_threadContainer.end(), join_all_threads);
}

template<typename Task>
void ThreadPool<Task>::Shutdown(size_t _timeout_ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(_timeout_ms));
    Shutdown();
}


} // end namespace alexei_prog_snob

#endif /* THREAD_POOL_H_ */