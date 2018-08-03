#include "thread_pool_test.h"
#include "thread_pool.h"

thread_pool_result init_threads_and_terminate() {
    alexei_prog_snob::thread_pool<std::function<void()> > pool;
    pool.init_thread_pool(2);
    return THREAD_PASS;
}