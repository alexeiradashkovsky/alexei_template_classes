#ifndef THREAD_POOL_TEST_H_
#define THREAD_POOL_TEST_H_

typedef enum _thread_pool_result {
    THREAD_PASS = 0,
    THREAD_FAIL
}thread_pool_result;

thread_pool_result init_threads_and_terminate();
#endif /* THREAD_POOL_TEST_H_ */