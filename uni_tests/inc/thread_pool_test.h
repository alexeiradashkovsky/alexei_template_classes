#ifndef THREAD_POOL_TEST_H_
#define THREAD_POOL_TEST_H_

typedef enum _thread_pool_result {
    THREAD_PASS = 0,
    THREAD_FAIL
}thread_pool_result;

// init number of threads and terminate
// expect THREAD_PASS and not crash
thread_pool_result init_threads_and_terminate();

thread_pool_result init_threads_and_terminate_with_timeout();

// init number of threads 
// submit one task, sleep for 15 milliseconds and then terminate all threads
// expect THREAD_PASS and not crash
thread_pool_result submit_one_task();

thread_pool_result submit_priority_taks();
#endif /* THREAD_POOL_TEST_H_ */