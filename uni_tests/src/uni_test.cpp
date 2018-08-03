#include "uni_test.h"
#include "thread_pool_test.h"

UNIT(basic_thread_pool_test)
	ASSERT_THAT(init_threads_and_terminate() == THREAD_PASS); 
END_UNIT


TEST_SUITE(Test Templates)
	TEST(basic_thread_pool_test)
END_SUITE
