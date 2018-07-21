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
class string_cnt_struct {
public:
	typedef std::function<void(std::shared_ptr<DataType>, const std::string& _str)> Task;
	typedef std::map<std::string, Task> Dictionary;
	
	string_cnt_struct(Dictionary& _dictionary)
	:m_defaul_names(_dictionary),
	 new_data(nullptr),
	 m_next_task(nullptr) {
	}

	~string_cnt_struct(){};

	std::shared_ptr<DataType> operator()(const std::string& _str) {
		auto res = m_defaul_names.find(_str);
		std::shared_ptr<DataType> retval(nullptr);

		if( res != m_defaul_names.end()) {
			std::shared_ptr<DataType> tmp(new DataType); 
			new_data = tmp;
			m_next_task = m_defaul_names[_str];
		} else {
			if(m_next_task != nullptr) {
				m_next_task(new_data, _str);
				retval = new_data;
				m_next_task = nullptr;
			}
		}
		return retval;
	}

	std::shared_ptr<DataType> get_last() {
		return nullptr;
	}
private:
	Dictionary& m_defaul_names;
	std::shared_ptr<DataType> new_data;
	Task m_next_task;
};


template<class DataType>
class data_cnt_struct {
public:
	typedef std::function<void(std::shared_ptr<DataType>, const std::string& _str)> Task;
	typedef std::map<std::string, Task> Dictionary;
	
	data_cnt_struct(Dictionary& _dictionary)
	:m_defaul_names(_dictionary),
	 new_data(nullptr),
	 m_next_task(nullptr) {
	}

	~data_cnt_struct(){};

	std::shared_ptr<DataType> operator()(const std::string& _str) {
		auto res = m_defaul_names.find(_str);
		std::shared_ptr<DataType> retval(nullptr);
		if(res != m_defaul_names.end()) {
			m_next_task = m_defaul_names[_str];
		} else {
			if(_str[0] == '[') {
				std::shared_ptr<DataType> tmp(new DataType);
				retval = new_data;
				new_data = tmp;
				m_defaul_names["name"](new_data, _str);
				m_next_task == nullptr;
			}
			if(m_next_task != nullptr) {
				m_next_task(new_data, _str);
				m_next_task = nullptr;
			}
		}
		return retval;
	}
	
	std::shared_ptr<DataType> get_last() {
		return new_data;
	}
private:
	Dictionary& m_defaul_names;
	std::shared_ptr<DataType> new_data;
	Task m_next_task;
};


template<class StringContainer = std::queue<std::string> >
class init_file_parser_template {
public:
	explicit init_file_parser_template(const std::string& _delimiters);
	~init_file_parser_template();
	void parse_line(std::string _line, StringContainer& _cnt);
private:
	std::string m_data_delimiters;
};

template<
class DataType 	= std::string, 
class Creator 	= string_cnt_struct<DataType>
>
class config_file_reader_template {
public:
	typedef std::vector<std::string> StringContainer;
	
	config_file_reader_template(const std::string& _delimiters, Creator& _creator);
	~config_file_reader_template();
	bool read_init_file(const std::string& _file_name);
	void construct_all_data();
	bool get_new_data(std::shared_ptr<DataType>& _new_data);
private:
	std::queue<std::shared_ptr<DataType> > m_data_cnt;
	StringContainer m_str_cnt;
	init_file_parser_template<StringContainer> m_parser;
	Creator& m_creator;
};

template<class DataType, class Creator>
config_file_reader_template<DataType, Creator>::
config_file_reader_template(const std::string& _delimiters, Creator& _creator)
:m_parser(_delimiters),
 m_creator(_creator) {
}

template<class DataType, class Creator>
config_file_reader_template<DataType, Creator>::
~config_file_reader_template() {
}

template<class DataType, class Creator>
bool config_file_reader_template<DataType, Creator>::
read_init_file(const std::string& _file_name) {
	std::ifstream init_file(_file_name);
	if(init_file.is_open() == false) {
		return false;
	}

	std::string get_line;
	while(std::getline(init_file, get_line)){
		m_parser.parse_line(get_line, m_str_cnt);
	}
	init_file.close();
	return true;
}

template<class DataType, class Creator>
void config_file_reader_template<DataType, Creator>::
construct_all_data() {
	auto close_dll = [this](const std::string& _str) { 
			std::shared_ptr<DataType> new_data(m_creator(_str));
			if(new_data != nullptr) {
				m_data_cnt.push(new_data);
			} 
		};

	std::for_each(m_str_cnt.begin(), m_str_cnt.end(), close_dll);

	std::shared_ptr<DataType> new_data(m_creator.get_last());
	if(new_data != nullptr) {
		m_data_cnt.push(new_data);
	}
}

template<class DataType, class Creator>
bool config_file_reader_template<DataType, Creator>::
get_new_data(std::shared_ptr<DataType>& _new_data) {
	if(m_data_cnt.empty() == true) {
		return true;
	}

	_new_data = m_data_cnt.front();
	m_data_cnt.pop();
	return false;	
}

template<class StringContainer>
init_file_parser_template<StringContainer>::init_file_parser_template(const std::string& _delimiters)
:m_data_delimiters(_delimiters) {
}

template<class StringContainer>
init_file_parser_template<StringContainer>::~init_file_parser_template(){}

template<class StringContainer>
void init_file_parser_template<StringContainer>::parse_line(std::string _line, StringContainer& _cnt) {
	size_t pos = 0;
	while(true) {
		pos = _line.find(m_data_delimiters);
		std::fill_n(std::back_inserter(_cnt), 1, _line.substr(0,pos));
		if(pos == std::string::npos) {
			break;
		}
		_line.erase(0, pos + m_data_delimiters.length());
	}
}


} // end namespace alexei_prog snob

#endif /* CONFIG_FILE_READER_H_ */
