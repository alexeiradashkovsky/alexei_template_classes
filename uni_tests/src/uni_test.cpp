#include "uni_test.h"
#include "thread_pool_test.h"
// #include "python_test.h"
#include "creational_design_patterns.h"
#include "aps/templates/template_utils.h"
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

UNIT(binary_utils)
	ASSERT_THAT(0 == alexei_prog_snob::binary<0>::value);
	ASSERT_THAT(1 == alexei_prog_snob::binary<1>::value);
	ASSERT_THAT(3 == alexei_prog_snob::binary<11>::value);
	ASSERT_THAT(2 == alexei_prog_snob::binary<10>::value);
	ASSERT_THAT(7 == alexei_prog_snob::binary<111>::value);
	ASSERT_THAT(15 == alexei_prog_snob::binary<1111>::value);
	ASSERT_THAT(31 == alexei_prog_snob::binary<11111>::value);
	ASSERT_THAT(63 == alexei_prog_snob::binary<111111>::value);
	ASSERT_THAT(127 == alexei_prog_snob::binary<1111111>::value);
	ASSERT_THAT(255 == alexei_prog_snob::binary<11111111>::value);
	ASSERT_THAT(511 == alexei_prog_snob::binary<111111111>::value);
END_UNIT

// UNIT(construct_and_init_from_python)
// 	ASSERT_THAT(init_from_python_test() == PYTHON_TEST_PASS);
// END_UNIT

// UNIT(basic_flow_call_func_from_python)
// 	ASSERT_THAT(CallFunction_withou_params_and_without_return_value() == PYTHON_TEST_PASS);
// END_UNIT


// UNIT(get_return_value_from_python_function)
// 	ASSERT_THAT(CallFunction_without_params_and_with_return_value() == PYTHON_TEST_PASS);
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
	TEST(binary_utils)
END_SUITE
