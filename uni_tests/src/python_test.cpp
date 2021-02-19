#include "python_test.h"
#include "ImportFromPython.h"

python_test_result init_from_python_test() {
    alexei_prog_snob::ImportFromPython pf(
        "uni_test", 
        "unit_test_python_file", 
        "/home/alexei/worspace/gitlab_projects/alexei-template-classes/uni_tests/src/pyfiles");
    
    if (pf.InitImportFromPython() != 0) {
        return PYTHON_TEST_FAIL;
    }
    return PYTHON_TEST_PASS;
}

python_test_result CallFunction_withou_params_and_without_return_value() {
    alexei_prog_snob::ImportFromPython pf(
        "uni_test", 
        "unit_test_python_file", 
        "/home/alexei/worspace/gitlab_projects/alexei-template-classes/uni_tests/src/pyfiles");
    
    if (pf.InitImportFromPython() != 0) {
        return PYTHON_TEST_FAIL;
    }

    if (pf.CallFunction("basic_func_print") != 0) {
        return PYTHON_TEST_FAIL;
    }

    return PYTHON_TEST_PASS;
}

python_test_result CallFunction_without_params_and_with_return_value() {
    alexei_prog_snob::ImportFromPython pf(
        "uni_test", 
        "unit_test_python_file", 
        "/home/alexei/worspace/gitlab_projects/alexei-template-classes/uni_tests/src/pyfiles");
    
    if (pf.InitImportFromPython() != 0) {
        return PYTHON_TEST_FAIL;
    }

    int return_value = 0;

    if (pf.CallFunction("basic_return_int", return_value) != 0) {
        return PYTHON_TEST_FAIL;
    }

    return return_value == 100 ? PYTHON_TEST_PASS:PYTHON_TEST_FAIL;
}