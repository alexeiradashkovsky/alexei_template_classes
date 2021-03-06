#ifndef FACTORY_H
#define FACTORY_H

/*
MIT License

Copyright (c) 2019 Alexei Radashkovsky

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
 *  @file factory.h
 *  @brief 
 *
 *  @details
 *
 *  @author Author Alexei Radashkovsky (alexeirada@gmail.com)
 *  @Update by Alexei Radashkovsky on 06/23/19
 *  @bug no bugs known.
 */
#include <functional>
#include <string>
#include <memory>
#include <map>


namespace alexei_prog_snob {
template<typename T>
class AbstractFactory {
public:
    using MakeMethod = std::function<std::shared_ptr<T>(void)>;

    AbstractFactory& operator=(const AbstractFactory&) = delete;
    AbstractFactory(const AbstractFactory&&) = delete;
    AbstractFactory(const AbstractFactory&) = delete;

    AbstractFactory(){}
    ~AbstractFactory(){}

    void RegisterToFactory(const std::string& _className, MakeMethod _method);
    std::shared_ptr<T> MakeObject(const std::string& _className) const;
private:
    std::map<std::string, MakeMethod> m_ctors;
};


template<typename Klass, typename abstract_class>
std::shared_ptr<abstract_class> Maker() {
    return std::make_shared<Klass>();
}

template<typename T>
void AbstractFactory<T>::RegisterToFactory(const std::string& _className, MakeMethod _method) {
    m_ctors[_className] = _method;
}


template<typename T>
std::shared_ptr<T> AbstractFactory<T>::MakeObject(const std::string& _className) const {
    auto itr = m_ctors.find(_className);
    if (itr == m_ctors.end()) {
        return nullptr;
    }

    return (itr->second)();
}

} // end namespace alexei_prog_snob

#define REG(Factory, Klass, AbstractKlass) Factory.RegisterToFactory(#Klass, alexei_prog_snob::Maker<Klass,AbstractKlass> )

#endif // FACTORY_H