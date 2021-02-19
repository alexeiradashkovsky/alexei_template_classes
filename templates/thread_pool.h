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
    enum ThreadPoolPriorityEnum {
        LOW = 0,
        MID,
        HIGH,
        LAST_PRIORITY
    };

    typedef std::pair<Task,ThreadPoolPriorityEnum> TaskPriorityPair;
    
    ThreadPool();
    ~ThreadPool();

    // uncopyable
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool(const ThreadPool&&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    /**
     * @brief initialize all threads
     * @param[in] _numOfThreadsInTheSystem : number of threads to initialize.
     */
    void InitThreadPool(size_t _numOfThreadsInTheSystem);

    /**
     * @brief add task to operate
     * @param[in] _priority : task priority.
     * @param[in] _task     : task to preforme.
     */
    void SubmitTask(ThreadPoolPriorityEnum _priority, Task _task);

    /**
     * @brief add task to operate
     * @param[in] _priorityTask : pair of task and priority.
     */
    void SubmitTask(const TaskPriorityPair& _priorityTask);

    /**
     * @brief terminate all thread
     */
    void Shutdown();

    /**
     * @brief terminate all thread
     * @param[in] _msTimeout : time to wait before shutdown.
     */
    void Shutdown(size_t _msTimeout);
private:
    bool m_terminate;

    struct CompairPriority {
        bool operator()(const TaskPriorityPair& _lhs, const TaskPriorityPair& _rhs) {
            return _lhs.second < _rhs.second;
        }
    };

    alexei_prog_snob::SafePriorityQueue<
    TaskPriorityPair,
    std::vector<TaskPriorityPair >,
    CompairPriority
    > m_taskContainer;

    std::vector<std::thread> m_threadContainer;
};

template<typename Task>
ThreadPool<Task>::ThreadPool()
:m_terminate(true) {
}

template<typename Task>
ThreadPool<Task>::~ThreadPool() {
    if (m_terminate == false) {
        Shutdown();
    }
}

template<typename Task>
void ThreadPool<Task>::InitThreadPool(size_t _numOfThreadsInTheSystem) {
    m_terminate = false;
    auto getTasksInThreads = [this](){
        while (m_terminate != true) {
            TaskPriorityPair newTask = std::move(m_taskContainer.Pop());
            newTask.first();
        }
    };

    for (size_t i = 0; i < _numOfThreadsInTheSystem ; ++i) {
        m_threadContainer.emplace_back(getTasksInThreads);
    }
}

template<typename Task>
void ThreadPool<Task>::SubmitTask(ThreadPoolPriorityEnum _priority, Task _task) {
    TaskPriorityPair newPair(_task, _priority);
    SubmitTask(std::move(newPair));
}

template<typename Task>
void ThreadPool<Task>::SubmitTask(const TaskPriorityPair& _priorityTask) {
    m_taskContainer.Push(_priorityTask);
}

template<typename Task>
void ThreadPool<Task>::Shutdown() {
    m_terminate = true;
    auto joinAllThreads = [this](std::thread& _nextThread)->void{_nextThread.join();};
    auto killAllThreads = [this]()->void{
        auto emptyTask = [](){return;};
        SubmitTask(HIGH, emptyTask);
    };
    
    for (size_t i = 0 ; i < m_threadContainer.size(); ++i) {
        killAllThreads();
    }

    std::for_each(m_threadContainer.begin(), m_threadContainer.end(), joinAllThreads);
}

template<typename Task>
void ThreadPool<Task>::Shutdown(size_t _msTimeout) {
    std::this_thread::sleep_for(std::chrono::milliseconds(_msTimeout));
    Shutdown();
}


} // end namespace alexei_prog_snob

#endif /* THREAD_POOL_H_ */