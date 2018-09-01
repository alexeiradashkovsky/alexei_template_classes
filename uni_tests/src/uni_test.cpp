#include "uni_test.h"
#include "thread_pool_test.h"

UNIT(basic_thread_pool_test)
	ASSERT_THAT(init_threads_and_terminate() == THREAD_PASS); 
END_UNIT

UNIT(basic_thread_pool_test_with_timeout)
	ASSERT_THAT(init_threads_and_terminate_with_timeout() == THREAD_PASS); 
END_UNIT

UNIT(submit_one_task_to_threads)
	ASSERT_THAT(submit_one_task() == THREAD_PASS); 
END_UNIT

UNIT(submit_task_and_check_priority)
	ASSERT_THAT(submit_priority_taks() == THREAD_PASS); 
END_UNIT

TEST_SUITE(Test Templates)
	TEST(basic_thread_pool_test)
	TEST(basic_thread_pool_test_with_timeout)
	TEST(submit_one_task_to_threads)
	TEST(submit_task_and_check_priority)
END_SUITE
