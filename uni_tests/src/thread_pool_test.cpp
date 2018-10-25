#include "thread_pool_test.h"
#include "thread_pool.h"

thread_pool_result init_threads_and_terminate() {
    alexei_prog_snob::thread_pool<std::function<void()> > pool;
    pool.init_thread_pool(2);
    return THREAD_PASS;
}

thread_pool_result init_threads_and_terminate_with_timeout() {
    alexei_prog_snob::thread_pool<std::function<void()> > pool;
    pool.init_thread_pool(2);
    pool.shutdown(200);
    return THREAD_PASS;
}

thread_pool_result submit_one_task() {
    using thread_pool = alexei_prog_snob::thread_pool<std::function<void()> >;
    thread_pool pool;
    pool.init_thread_pool(2);

    thread_pool_result flag = THREAD_FAIL;
    auto task_change_flag_to_pass = [&flag](void)->void{flag = THREAD_PASS;};
    pool.submit_task(thread_pool::MID, task_change_flag_to_pass);
    pool.shutdown(15);
    return flag;
}

thread_pool_result submit_priority_taks() {
    using thread_pool = alexei_prog_snob::thread_pool<std::function<void()> >;
    thread_pool pool;
    pool.init_thread_pool(2);

    std::array<uint8_t,3> arr = {0,0,0};
    size_t place = 0;
    std::mutex mutex;
    auto low_task = [&](void)->void {
        std::lock_guard<std::mutex> guard(mutex);
        arr[place] = 1;
        ++place;
    };
    
    auto mid_task = [&](void)->void {
        std::lock_guard<std::mutex> guard(mutex);
        arr[place] = 2;
        ++place;
    };

    auto high_task = [&](void)->void {
        std::lock_guard<std::mutex> guard(mutex);
        arr[place] = 3;
        ++place;
    };


    // start test
    auto two_task_for_wait = [](void)->void{std::this_thread::sleep_for(std::chrono::milliseconds(100));};
    for(size_t i = 0; i < 2 ; ++i) {
        pool.submit_task(thread_pool::LOW, two_task_for_wait);
    }

    pool.submit_task(thread_pool::LOW, low_task);
    pool.submit_task(thread_pool::MID, mid_task);
    pool.submit_task(thread_pool::HIGH, high_task);
    pool.shutdown(100);

    std::array<uint8_t,3> arr_result = {3,2,1};
    for(size_t i = 0 ; i < 3 ; ++i) {
        if(arr_result[i] != arr[i]) {
            return THREAD_FAIL;
        }
    }
    return THREAD_PASS;
}