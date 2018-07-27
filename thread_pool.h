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

#include <vector>
#include <thread>
#include <chrono>
#include <pair>
#include <functional>
#include "safe_queue.h"
namespace alexei_prog_snob {

class thread_pool {
public:
    enum thread_pool_priority_enum {
        LOW = 0,
        MID,
        HIGH,
        LAST_PRIORITY
    };

    typedef std::function<void()> Task;
    typedef std::pair<Task,thread_pool_priority_enum> Task_Priority_Pair;
    
    explicit thread_pool(size_t _num_of_threads_in_the_system);
    ~thread_pool();

    void submit_task(thread_pool_priority_enum _priority, Task _task);
    void submit_task(Task_Priority_Pair _priority_task);

    void shutdown();
    void shutdown(size_t _timeout_ms);
private:
    bool m_terminate;

    struct compair_priority {
        bool operator()(const Task_Priority_Pair& _lhs, const Task_Priority_Pair& _rhs) {
            return _lhs.second < _rhs.second;
        }
    }

    alexei_prog_snob::safe_priority_queue<
    Task,
    std::vector<Task_Priority_Pair> ,
    compair_priority
    > m_task_container;

    std::vector<std::thread> m_thread_container;

    // uncopyable
    thread_pool(const thread_pool&);
    thread_pool& operatoe=(const thread_pool&);
};

thread_pool::thread_pool(size_t _num_of_threads_in_the_system)
:m_terminate(false)
 {
    auto get_tasks_in_threads = [this](){
        while(true)
        {
            if (m_terminate == true && m_task_container.empty() == true) {
                break;
            }
            Task newTask = std::move(m_task_container.pop());
            newTask();
        }
    };

    for(size_t i = 0; i < _num_of_threads_in_the_system ; ++i) {
        m_thread_container.emplace_back(get_tasks_in_threads());
    }
}

thread_pool::~thread_pool() {
    if(m_terminate == false) {
        shutdown();
    }
}

void thread_pool::submit_task(thread_pool_priority_enum _priority, Task _task) {
    Task_Priority_Pair newPair(_task, _priority);
    submit_task(std::move(newPair));
}

void thread_pool::submit_task(const Task_Priority_Pair& _priority_task) {
    m_task_container.push(std::move(_priority_task));
}

void thread_pool::shutdown() {
    m_terminate = true;
    auto join_all_threads = [](std::thread& _nextThread) { _nextThread.join(); };
    std::for_each(m_thread_container.begin(), m_thread_container.end(), join_all_threads);
}

void thread_pool::shutdown(size_t _timeout_ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(_timeout_ms));
    shutdown();
}


} // end namespace alexei_prog_snob

#endif /* THREAD_POOL_H_ */