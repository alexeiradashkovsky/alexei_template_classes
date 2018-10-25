#ifndef IMPORT_FROM_PYTHON_H_
#define IMPORT_FROM_PYTHON_H_

#include <Python.h>
#include <string>
#include <iostream>
#include <map>

namespace alexei_prog_snob {

class import_from_python {
public:
  import_from_python(
        std::string&& _program_name, 
        std::string&& _module_name , 
        std::string&& _full_path_to_py_files);
  ~import_from_python();
  int init_import_from_python();
  int call_function(std::string&& _function_name);
  int call_function(std::string&& _function_name, int& _return_value);
private:
    std::map<std::string,PyObject*>  m_call_backes;
    std::string m_program_name;
    std::string m_module_name;
    std::string m_full_path_to_py_files;
    PyObject* m_module;
    PyObject* m_imported_module;

    PyObject* get_func_object(std::string&& _function_name);
};


import_from_python::import_from_python(
    std::string&& _program_name, 
    std::string&& _module_name,
    std::string&& _full_path_to_py_files)
:m_program_name(std::move(_program_name)),
 m_module_name(std::move(_module_name)),
 m_full_path_to_py_files(_full_path_to_py_files),
 m_module(nullptr),
 m_imported_module(nullptr) {
}

import_from_python::~import_from_python() {
    Py_XDECREF(m_imported_module);
    Py_XDECREF(m_module);
    Py_Finalize();
}

int import_from_python::init_import_from_python() {
    wchar_t* program = Py_DecodeLocale(m_program_name.c_str(), NULL);
    if(program == nullptr) {
        std::cout<<"fail program\n";
        return -1;
    }

    Py_SetProgramName(program);
    Py_Initialize();
    PyRun_SimpleString("import sys\n");
    std::string commnad = "sys.path.append(\"" + m_full_path_to_py_files + "\")\n";
    PyRun_SimpleString(commnad.c_str());

    m_module = PyUnicode_DecodeFSDefault(m_module_name.c_str());
    if(m_module == nullptr) {
        std::cout<<"fail m_module\n";
        return -1;
    }

    m_imported_module = PyImport_Import(m_module);
    if(m_imported_module == nullptr) {
        std::cout<<"fail m_imported_module\n";
        return -1;
    }
    
    return 0;
}

int import_from_python::call_function(std::string&& _function_name) {
    PyObject* func = get_func_object(std::move(_function_name));
    if (func == nullptr) {
        return -1;
    }

    PyObject* value = PyObject_CallObject(func, nullptr);
    if (value == nullptr) {
        return -1;
    }
    Py_XDECREF(value);

    return 0;
}


int import_from_python::call_function(std::string&& _function_name, int& _return_value) {
    PyObject* func = get_func_object(std::move(_function_name));
    if (func == nullptr) {
        return -1;
    }

    PyObject* value = PyObject_CallObject(func, nullptr);
    if (value == nullptr) {
        return -1;
    }
    _return_value = PyLong_AsLong(value);
    Py_XDECREF(value);
    return 0;
}

PyObject* import_from_python::get_func_object(std::string&& _function_name) {
    PyObject* ret_func = nullptr;
    auto itr = m_call_backes.find(_function_name);
    if (itr == m_call_backes.end()) {
        ret_func = PyObject_GetAttrString(m_imported_module, _function_name.c_str());
        if(ret_func == nullptr || PyCallable_Check(ret_func) == 0) {
            return nullptr;
        }
        m_call_backes[_function_name] = ret_func;
    } else {
        ret_func = itr->second;
    }

    return ret_func;
}


} // namespace alexei_prog_snob

#endif /* IMPORT_FROM_PYTHON_H_ */