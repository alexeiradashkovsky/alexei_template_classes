#ifndef CONFIG_FILE_READER_H_
#define CONFIG_FILE_READER_H_

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
 *  @file config_file_reader.h
 *  @brief read config file and parse every line by decery type of data
 *  @details support only from c++11
 *
 *  @author Author Alexei Radashkovsky (alexeirada@gmail.com)
 *  @Update by Alexei Radashkovsky on 07/21/18
 *  @bug no bugs known.
 */

#include <map>
#include <queue>
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <algorithm>

namespace alexei_prog_snob {

template<class DataType = std::string>
class StringCntStruct {
public:
	
	typedef std::function<void(std::shared_ptr<DataType>, const std::string& _str)> Task;
	typedef std::map<std::string, Task> Dictionary;
	
	StringCntStruct(Dictionary& _dictionary)
	:m_defaultNames(_dictionary),
	 m_newData(nullptr),
	 m_nextTask(nullptr) {
	}

	~StringCntStruct(){};

	std::shared_ptr<DataType> operator()(const std::string& _str) {
		auto res = m_defaultNames.find(_str);
		std::shared_ptr<DataType> retval(nullptr);

		if (res != m_defaultNames.end()) {
			std::shared_ptr<DataType> tmp(new DataType); 
			m_newData = tmp;
			m_nextTask = m_defaultNames[_str];
		} else {
			if (m_nextTask != nullptr) {
				m_nextTask(m_newData, _str);
				retval = m_newData;
				m_nextTask = nullptr;
			}
		}
		return retval;
	}

	std::shared_ptr<DataType> GetLast() {
		return nullptr;
	}
private:
	Dictionary& m_defaultNames;
	std::shared_ptr<DataType> m_newData;
	Task m_nextTask;
};


template<class DataType>
class DataCntStruct {
public:
	typedef std::function<void(std::shared_ptr<DataType>, const std::string& _str)> Task;
	typedef std::map<std::string, Task> Dictionary;
	
	DataCntStruct(Dictionary& _dictionary)
	:m_defaultNames(_dictionary),
	 m_newData(nullptr),
	 m_nextTask(nullptr) {
	}

	~DataCntStruct(){};

	std::shared_ptr<DataType> operator()(const std::string& _str) {
		auto res = m_defaultNames.find(_str);
		std::shared_ptr<DataType> retval(nullptr);
		if (res != m_defaultNames.end()) {
			m_nextTask = m_defaultNames[_str];
		} else {
			if (_str[0] == '[') {
				std::shared_ptr<DataType> tmp(new DataType);
				retval = m_newData;
				m_newData = tmp;
				m_defaultNames["name"](m_newData, _str);
				m_nextTask == nullptr;
			}
			if (m_nextTask != nullptr) {
				m_nextTask(m_newData, _str);
				m_nextTask = nullptr;
			}
		}
		return retval;
	}
	
	std::shared_ptr<DataType> GetLast() {
		return m_newData;
	}
private:
	Dictionary& m_defaultNames;
	std::shared_ptr<DataType> m_newData;
	Task m_nextTask;
};


template<class StringContainer = std::queue<std::string> >
class InitFileParser {
public:
	explicit InitFileParser(const std::string& _delimiters);
	~InitFileParser();
	void ParseLine(std::string _line, StringContainer& _cnt);
private:
	std::string m_dataDelimiters;
};

template<
class DataType 	= std::string, 
class Creator 	= StringCntStruct<DataType>
>
class ConfigFileReader {
public:
	typedef std::vector<std::string> StringContainer;
	
	ConfigFileReader(const std::string& _delimiters, Creator& _creator);
	~ConfigFileReader();
	bool ReadInitFile(const std::string& _fileName);
	void ConstructAllData();
	bool GetNewData(std::shared_ptr<DataType>& _newData);
private:
	std::queue<std::shared_ptr<DataType> > m_dataCnt;
	StringContainer m_strCnt;
	InitFileParser<StringContainer> m_parser;
	Creator& m_creator;
};

template<class DataType, class Creator>
ConfigFileReader<DataType, Creator>::
ConfigFileReader(const std::string& _delimiters, Creator& _creator)
:m_parser(_delimiters),
 m_creator(_creator) {
}

template<class DataType, class Creator>
ConfigFileReader<DataType, Creator>::
~ConfigFileReader() {
}

template<class DataType, class Creator>
bool ConfigFileReader<DataType, Creator>::
ReadInitFile(const std::string& _fileName) {
	std::ifstream initFile(_fileName);
	if (initFile.is_open() == false) {
		return false;
	}

	std::string getLine;
	while(std::getline(initFile, getLine)){
		m_parser.ParseLine(getLine, m_strCnt);
	}
	initFile.close();
	return true;
}

template<class DataType, class Creator>
void ConfigFileReader<DataType, Creator>::
ConstructAllData() {
	auto close_dll = [this](const std::string& _str) { 
			std::shared_ptr<DataType> newData(m_creator(_str));
			if (newData != nullptr) {
				m_dataCnt.Push(newData);
			} 
		};

	std::for_each(m_strCnt.begin(), m_strCnt.end(), close_dll);

	std::shared_ptr<DataType> newData(m_creator.GetLast());
	if (newData != nullptr) {
		m_dataCnt.Push(newData);
	}
}

template<class DataType, class Creator>
bool ConfigFileReader<DataType, Creator>::
GetNewData(std::shared_ptr<DataType>& _newData) {
	if (m_dataCnt.Empty() == true) {
		return true;
	}

	_newData = m_dataCnt.front();
	m_dataCnt.Pop();
	return false;	
}

template<class StringContainer>
InitFileParser<StringContainer>::InitFileParser(const std::string& _delimiters)
:m_dataDelimiters(_delimiters) {
}

template<class StringContainer>
InitFileParser<StringContainer>::~InitFileParser(){}

template<class StringContainer>
void InitFileParser<StringContainer>::ParseLine(std::string _line, StringContainer& _cnt) {
	size_t pos = 0;
	while(true) {
		pos = _line.find(m_dataDelimiters);
		std::fill_n(std::back_inserter(_cnt), 1, _line.substr(0,pos));
		if (pos == std::string::npos) {
			break;
		}
		_line.erase(0, pos + m_dataDelimiters.length());
	}
}


} // end namespace alexei_prog snob

#endif /* CONFIG_FILE_READER_H_ */
