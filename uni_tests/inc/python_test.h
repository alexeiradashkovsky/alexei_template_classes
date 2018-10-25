#ifndef PYTHON_TEST_H_
#define PYTHON_TEST_H_

typedef enum _python_test_result {
    PYTHON_TEST_PASS = 0,
    PYTHON_TEST_FAIL
}python_test_result;

python_test_result init_from_python_test();
python_test_result call_function_withou_params_and_without_return_value();
python_test_result call_function_without_params_and_with_return_value();
#endif /* PYTHON_TEST_H_ */