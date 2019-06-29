#ifndef CREATIONAL_DESIGN_PATTERNS_H
#define CREATIONAL_DESIGN_PATTERNS_H

typedef enum _cdp_test_result {
    CDP_TEST_PASS = 0,
    CDP_TEST_FAIL
} cdp_test_result;


cdp_test_result abstract_factory_create_object();
cdp_test_result abstract_factory_check_constructed_correct_obj();
cdp_test_result singleton_check_that_only_one_instance_of_the_class();
#endif // CREATIONAL_DESIGN_PATTERNS_H