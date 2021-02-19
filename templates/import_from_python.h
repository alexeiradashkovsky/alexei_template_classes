#ifndef IMPORT_FROM_PYTHON_H_
#define IMPORT_FROM_PYTHON_H_

#include <Python.h>
#include <string>
#include <iostream>
#include <map>

namespace alexei_prog_snob {

class ImportFromPython {
public:
  ImportFromPython(
        std::string&& _programName, 
        std::string&& _moduleName , 
        std::string&& _fullPathToPyFiles);
  ~ImportFromPython();
  int InitImportFromPython();
  int CallFunction(std::string&& _functionName);
  int CallFunction(std::string&& _functionName, int& _returnValue);
private:
    std::map<std::string,PyObject*>  m_callBackes;
    std::string m_programName;
    std::string m_moduleName;
    std::string m_fullPathToPyFiles;
    PyObject* m_module;
    PyObject* m_importedModule;

    PyObject* p_GetFuncObject(std::string&& _functionName);
};


ImportFromPython::ImportFromPython(
    std::string&& _programName, 
    std::string&& _moduleName,
    std::string&& _fullPathToPyFiles)
:m_programName(std::move(_programName)),
 m_moduleName(std::move(_moduleName)),
 m_fullPathToPyFiles(_fullPathToPyFiles),
 m_module(nullptr),
 m_importedModule(nullptr) {
}

ImportFromPython::~ImportFromPython() {
    Py_XDECREF(m_importedModule);
    Py_XDECREF(m_module);
    Py_Finalize();
}

int ImportFromPython::InitImportFromPython() {
    wchar_t* program = Py_DecodeLocale(m_programName.c_str(), NULL);
    if (program == nullptr) {
        std::cout<<"fail program\n";
        return -1;
    }

    Py_SetProgramName(program);
    Py_Initialize();
    PyRun_SimpleString("import sys\n");
    std::string commnad = "sys.path.append(\"" + m_fullPathToPyFiles + "\")\n";
    PyRun_SimpleString(commnad.c_str());

    m_module = PyUnicode_DecodeFSDefault(m_moduleName.c_str());
    if (m_module == nullptr) {
        std::cout<<"fail m_module\n";
        return -1;
    }

    m_importedModule = PyImport_Import(m_module);
    if (m_importedModule == nullptr) {
        std::cout<<"fail m_importedModule\n";
        return -1;
    }
    
    return 0;
}

int ImportFromPython::CallFunction(std::string&& _functionName) {
    PyObject* func = p_GetFuncObject(std::move(_functionName));
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


int ImportFromPython::CallFunction(std::string&& _functionName, int& _returnValue) {
    PyObject* func = p_GetFuncObject(std::move(_functionName));
    if (func == nullptr) {
        return -1;
    }

    PyObject* value = PyObject_CallObject(func, nullptr);
    if (value == nullptr) {
        return -1;
    }
    _returnValue = PyLong_AsLong(value);
    Py_XDECREF(value);
    return 0;
}

PyObject* ImportFromPython::p_GetFuncObject(std::string&& _functionName) {
    PyObject* ret_func = nullptr;
    auto itr = m_callBackes.find(_functionName);
    if (itr == m_callBackes.end()) {
        ret_func = PyObject_GetAttrString(m_importedModule, _functionName.c_str());
        if (ret_func == nullptr || PyCallable_Check(ret_func) == 0) {
            return nullptr;
        }
        m_callBackes[_functionName] = ret_func;
    } else {
        ret_func = itr->second;
    }

    return ret_func;
}


} // namespace alexei_prog_snob

#endif /* IMPORT_FROM_PYTHON_H_ */