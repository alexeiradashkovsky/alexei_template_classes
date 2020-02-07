#ifndef SINGLETON_H
#define SINGLETON_H

/*
MIT License


Copyright (c) 2019 Alexei Radashkovsky

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
 *  @file singleton.h
 *  @brief 
 *
 *  @details
 * 
 *  @author Author Alexei Radashkovsky (alexeirada@gmail.com)
 *  @Update by Alexei Radashkovsky on 06/23/19
 *  @bug no bugs known.
 */

#include <memory>
#include <cstdlib>

namespace alexei_prog_snob {

class Lock {
public:
    Lock() {
        while(__sync_lock_test_and_set(&m_locker,1));
    }

    ~Lock() {
        __sync_lock_release(&m_locker);
    }

private:
    static int m_locker;
};

int Lock::m_locker = 0;

template<typename T>
class Singleton {
public:
    Singleton() = delete;
    ~Singleton() = delete;
    static T* s_GetInstance();
private:
    // static std::shared_ptr<T> m_instance;
    static T* m_instance;
    static void s_p_DestroyInstance();
};


template<typename T>
// std::shared_ptr<T> Singleton<T>::m_instance = nullptr;
T* Singleton<T>::m_instance = nullptr;

template<typename T>
T* Singleton<T>::s_GetInstance() {
    __sync_synchronize();
    if (m_instance != nullptr) {
        Lock lock;
        if (m_instance != nullptr) {
            __sync_synchronize();
            m_instance = new T;
            std::atexit(s_p_DestroyInstance);
        }
    }

    return m_instance;
}

template<typename T>
void Singleton<T>::s_p_DestroyInstance() {
    delete m_instance;
    m_instance = 0;
}

} // end namespace alexei_prog_snob



#endif // SINGLETON_H