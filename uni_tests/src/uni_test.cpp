#include "uni_test.h"
#include "thread_pool_test.h"
// #include "python_test.h"
#include "creational_design_patterns.h"

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

// UNIT(construct_and_init_from_python)
// 	ASSERT_THAT(init_from_python_test() == PYTHON_TEST_PASS);
// END_UNIT

// UNIT(basic_flow_call_func_from_python)
// 	ASSERT_THAT(call_function_withou_params_and_without_return_value() == PYTHON_TEST_PASS);
// END_UNIT


// UNIT(get_return_value_from_python_function)
// 	ASSERT_THAT(call_function_without_params_and_with_return_value() == PYTHON_TEST_PASS);
// END_UNIT

UNIT(cdp_abstract_factory_create_object)
	ASSERT_THAT(abstract_factory_create_object() == CDP_TEST_PASS);
END_UNIT

UNIT(cdp_abstract_factory_check_constructed_correct_obj)
	ASSERT_THAT(abstract_factory_check_constructed_correct_obj() == CDP_TEST_PASS);
END_UNIT

UNIT(cdp_singleton_check_that_only_one_instance_of_the_class)
	ASSERT_THAT(singleton_check_that_only_one_instance_of_the_class() == CDP_TEST_PASS);
END_UNIT

TEST_SUITE(Test Templates)
	TEST(basic_thread_pool_test)
	TEST(basic_thread_pool_test_with_timeout)
	TEST(submit_one_task_to_threads)
	TEST(submit_task_and_check_priority)

	// TEST(construct_and_init_from_python)
	// TEST(basic_flow_call_func_from_python)
	// TEST(get_return_value_from_python_function)

	TEST(cdp_abstract_factory_create_object)
	TEST(cdp_abstract_factory_check_constructed_correct_obj)
	TEST(cdp_singleton_check_that_only_one_instance_of_the_class)
END_SUITE
