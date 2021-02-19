#ifndef SAFE_QUEUE_H__
#define SAFE_QUEUE_H__

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

#include <queue> /*< std::queue >*/
#include <mutex> /*< std::mutex >*/
#include <condition_variable>


namespace alexei_prog_snob {

template<typename T> class SafeQueue {
public:
	SafeQueue(){
	}

	~SafeQueue(){
	}
	/**
     * @brief get element. Blocking if empty
     * @param[in] _msTimeout : time to wait before shutdown.
     */
	T Pop() {
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.wait(lock, [this]()->bool{ return m_q.empty() != true; });
		T ret_val = m_q.front();
		m_q.pop();
		return ret_val;
	}

	void Push(const T& _data) {
		std::lock_guard<std::mutex> lock(m_mutex);
		m_q.push(_data);
		m_cv.notify_one();
	}
	
	bool Empty() {
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_q.empty();
	}
	
private:
	std::mutex m_mutex;
	std::condition_variable m_cv;
	std::queue<T> m_q;
};



template<
typename T, 
typename Container = std::vector<T>,
typename Comparison = std::less<typename Container::value_type>
> 
class SafePriorityQueue {
public:
	SafePriorityQueue(){
	}

	~SafePriorityQueue(){
	}

	T Pop() {
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.wait(lock, [this]()->bool{ return m_q.empty() != true; });
        T ret_val = m_q.top();
        m_q.pop();
		return ret_val;
	}

	void Push(const T& _data){
		std::lock_guard<std::mutex> lock(m_mutex);
		m_q.push(_data);
        m_cv.notify_one();
	}

	bool Empty() {
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_q.empty();
	}
	
private:
	std::mutex m_mutex;
    std::condition_variable m_cv;
	std::priority_queue<T, Container, Comparison> m_q;
};

} // end namespace

#endif /* SAFE_QUEUE_H__ */