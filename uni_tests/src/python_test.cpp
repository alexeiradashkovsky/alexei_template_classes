#include "python_test.h"
#include "import_from_python.h"

python_test_result init_from_python_test() {
    alexei_prog_snob::import_from_python pf(
        "uni_test", 
        "unit_test_python_file", 
        "/home/alexei/worspace/gitlab_projects/alexei-template-classes/uni_tests/src/pyfiles");
    
    if (pf.init_import_from_python() != 0) {
        return PYTHON_TEST_FAIL;
    }
    return PYTHON_TEST_PASS;
}

python_test_result call_function_withou_params_and_without_return_value() {
    alexei_prog_snob::import_from_python pf(
        "uni_test", 
        "unit_test_python_file", 
        "/home/alexei/worspace/gitlab_projects/alexei-template-classes/uni_tests/src/pyfiles");
    
    if (pf.init_import_from_python() != 0) {
        return PYTHON_TEST_FAIL;
    }

    if (pf.call_function("basic_func_print") != 0) {
        return PYTHON_TEST_FAIL;
    }

    return PYTHON_TEST_PASS;
}

python_test_result call_function_without_params_and_with_return_value() {
    alexei_prog_snob::import_from_python pf(
        "uni_test", 
        "unit_test_python_file", 
        "/home/alexei/worspace/gitlab_projects/alexei-template-classes/uni_tests/src/pyfiles");
    
    if (pf.init_import_from_python() != 0) {
        return PYTHON_TEST_FAIL;
    }

    int return_value = 0;

    if (pf.call_function("basic_return_int", return_value) != 0) {
        return PYTHON_TEST_FAIL;
    }

    return return_value == 100 ? PYTHON_TEST_PASS:PYTHON_TEST_FAIL;
}