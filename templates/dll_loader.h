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
    
template<class T> class DllLaderTemplate {
public:
    typedef T* creat_t();

     /**
     * @brief DllLaderTemplate constructor
     * @param[in] _path             : path to all dll files.
     * @param[in] _createFunction  : name of the create functions
     * @param[in] _dllEnding       : type of the ending of dll files (by operating system).
     */
    DllLaderTemplate(const std::string& _path, const std::string& _createFunction, const std::string& _dllEnding);
    
    ~DllLaderTemplate();

    /**
     * @brief LoadAllDlls load all dll files
     * @param[in] _allDll : all dll files names
     */
    void LoadAllDlls(const std::vector<std::string>& _allDll);

    /**
     * @brief LoadDllObject load an abstract class by the constructor that you implemented in dll file
     * @param[in] _dllName : dll file name
     * @return - the abstract class for polymorphism 
     */
    std::shared_ptr<T> LoadDllObject(const std::string& _dllName);
private:
    void* p_LoadNewDll(const std::string& _dllName);

    std::string m_path;
    std::string m_create;
    std::string m_dllEnding;
    std::map<std::string, void*> m_dllLibs;

    // uncopyable
    DllLaderTemplate(const DllLaderTemplate&);
    DllLaderTemplate operator=(const DllLaderTemplate&);
};

template<class T>
DllLaderTemplate<T>::DllLaderTemplate(
    const std::string& _path, 
    const std::string& _createFunction, 
    const std::string& _dllEnding)
:m_path(_path),
 m_create(_createFunction),
 m_dllEnding(_dllEnding) {
}


template<class T>
DllLaderTemplate<T>::~DllLaderTemplate() {
    auto closeDll = [](std::pair<const std::string, void*>& _dllLib) { dlclose(_dllLib.second); };
    std::for_each(m_dllLibs.begin(), m_dllLibs.end(), closeDll);
}

template<class T>
void DllLaderTemplate<T>::LoadAllDlls(const std::vector<std::string>& _allDll) {
    std::for_each(_allDll.begin(), _allDll.end() , [](const std::string& _dllName){ p_LoadNewDll(_dllName); });
}

template<class T>
void* DllLaderTemplate<T>::p_LoadNewDll(const std::string& _dllName) {
    std::string fullPath = m_path + _dllName + m_dllEnding;
    void* newLib = dlopen(fullPath.c_str(), RTLD_LAZY);
    if (newLib == nullptr) {
        std::string dllError(dlerror());
        return nullptr;
    }

    m_dllLibs[_dllName] = newLib;
    return newLib;
}

template<class T>
std::shared_ptr<T> DllLaderTemplate<T>::LoadDllObject(const std::string& _dllName) {
    void* correntLib = nullptr;
    std::map<std::string, void*>::iterator itr = m_dllLibs.find(_dllName);

	correntLib = (itr == m_dllLibs.end()) ? p_LoadNewDll(_dllName) : itr->second;
    if (correntLib == nullptr) {
        return nullptr;
    }
    
    creat_t* createObject = (creat_t*) dlsym(correntLib, m_create.c_str());
    if (createObject == nullptr) {
        return nullptr;
    }
    std::shared_ptr<T> newPtr(createObject());
    return newPtr;
}

} // end namespace alexei_prog_snob

#endif /* DLL_LOADER_H */