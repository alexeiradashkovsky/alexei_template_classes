#ifndef DLL_LOADER_H
#define DLL_LOADER_H

/*
MIT License

Copyright (c) 2018 Alexei Radashkovsky

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


/**
 *  @file dll_loader.h
 *  @brief load abstract objects from dll files every abstract class have to use the next create function
 *  T* create_t() {
 *      return new abstract_class;
 *  }
 *
 *  @details support only from c++11
 *
 *  @author Author Alexei Radashkovsky (alexeirada@gmail.com)
 *  @Update by Alexei Radashkovsky on 04/27/18
 *  @bug no bugs known.
 */


#include <string>
#include <map>
#include <memory>
#include <vector>
#include <algorithm>
#include <dlfcn.h>

namespace alexei_prog_snob {
    
template<class T> class dll_loader_template {
public:
    typedef T* creat_t();

     /**
     * @brief dll_loader_template constructor
     * @param[in] _path             : path to all dll files.
     * @param[in] _create_function  : name of the create functions
     * @param[in] _dll_ending       : type of the ending of dll files (by operating system).
     */
    dll_loader_template(const std::string& _path, const std::string& _create_function, const std::string& _dll_ending);
    
    ~dll_loader_template();

    /**
     * @brief load_all_dlls load all dll files
     * @param[in] _all_dll : all dll files names
     */
    void load_all_dlls(const std::vector<std::string>& _all_dll);

    /**
     * @brief load_dll_object load an abstract class by the constructor that you implemented in dll file
     * @param[in] _dll_name : dll file name
     * @return - the abstract class for polymorphism 
     */
    std::shared_ptr<T> load_dll_object(const std::string& _dll_name);
private:
    void* load_new_dll(const std::string& _dll_name);

    std::string m_path;
    std::string m_create;
    std::string m_dll_ending;
    std::map<std::string, void*> m_dll_libs;

    // uncopyable
    dll_loader_template(const dll_loader_template&);
    dll_loader_template operator=(const dll_loader_template&);
};

template<class T>
dll_loader_template<T>::dll_loader_template(
    const std::string& _path, 
    const std::string& _create_function, 
    const std::string& _dll_ending)
:m_path(_path),
 m_create(_create_function),
 m_dll_ending(_dll_ending) {
}


template<class T>
dll_loader_template<T>::~dll_loader_template() {
    auto close_dll = [](std::pair<const std::string, void*>& _dll_lib) { dlclose(_dll_lib.second); };
    std::for_each(m_dll_libs.begin(), m_dll_libs.end(), close_dll);
}

template<class T>
void dll_loader_template<T>::load_all_dlls(const std::vector<std::string>& _all_dll) {
    std::for_each(_all_dll.begin(), _all_dll.end() , [](const std::string& _dll_name){ load_new_dll(_dll_name); });
}

template<class T>
void* dll_loader_template<T>::load_new_dll(const std::string& _dll_name) {
    std::string full_path = m_path + _dll_name + m_dll_ending;
    void* newlib = dlopen(full_path.c_str(), RTLD_LAZY);
    if (newlib == nullptr) {
        std::string dll_error(dlerror());
        return nullptr;
    }

    m_dll_libs[_dll_name] = newlib;
    return newlib;
}

template<class T>
std::shared_ptr<T> dll_loader_template<T>::load_dll_object(const std::string& _dll_name) {
    void* correntLib = nullptr;
    std::map<std::string, void*>::iterator itr = m_dll_libs.find(_dll_name);

	correntLib = (itr == m_dll_libs.end()) ? load_new_dll(_dll_name):itr->second;
    if(correntLib == nullptr) {
        return nullptr;
    }
    
    creat_t* create_object = (creat_t*) dlsym(correntLib, m_create.c_str());
    if(create_object == nullptr) {
        return nullptr;
    }
    std::shared_ptr<T> new_ptr(create_object());
    return new_ptr;
}

} // end namespace alexei_prog_snob

#endif /* DLL_LOADER_H */