#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <direct.h>
#include <Windows.h>
#include <iostream>
#include <filesystem>
#include <iomanip>
#include <algorithm>
#include <map>



class Tests{
public:
	Tests() {
		CreateInfo();
		///
		m_input_file_name = "nost_bin/";
		CreateDir(m_input_file_name);
		m_input_file_name += "ip.dat";
		///
		m_result_folder_dir = "nost_bin/results/";
		CreateDir(m_result_folder_dir);
		///
		m_tests_dir = "nost_bin/tests/";
		SetTestsList();
	}

	void SetTestsList() {

		std::ifstream tests_list(m_input_file_name);
		if (tests_list.is_open()) {
			std::string test_name;
			while (!tests_list.eof())
			{
				getline(tests_list, test_name);
				if (!test_name.empty())
					m_tests_list.push_back(test_name);
			}
			current_dir = m_tests_list[0];
			m_tests_list.erase(m_tests_list.begin());
		}
		else
			std::cout << m_input_file_name << " is not opened!" << std::endl;


	}

	void CreateDir(const std::string & directory) const {
		if (CreateDirectory(directory.c_str(), NULL))
			std::cout << "Directory " << directory << " created. " << std::endl;
		else {
			std::cout << GetLastError() <<" ";
			if (GetLastError() == ERROR_ALREADY_EXISTS)
				std::cout << "Directory " << directory << " already exists. " << std::endl;
			else if (GetLastError() == ERROR_PATH_NOT_FOUND)
				std::cout << "WARNING! Directory " << directory << " is not created. " << std::endl;
		}
	}

	void GetFilesList(const std::string & currentTest)
	{
		std::string path = m_tests_dir + currentTest + "/";
		for (const auto & entry : std::filesystem::recursive_directory_iterator(path)) {
			std::string entire = entry.path().string();
			m_fileList.push_back(entry.path().string());
			for (unsigned int i = 0; i < path.size(); i++)
				entire.erase(entire.begin());
			m_currentFile.push_back(entire);
		}
	}

	void ChangeLibrariesPath(const std::string & bippar_path)const  {
		std::ifstream bf(bippar_path);
		std::string current_string;
		std::vector<std::string> temp;
		try {
			if (bf.is_open())
			{
				while (!bf.eof()) {
					getline(bf, current_string);
					if (current_string.find("NAMCON") != -1)
						temp.push_back(" NAMCON=" + m_par.at(1) + "\n");
					else
						temp.push_back(current_string + "\n");
				}
				bf.close();
				std::ofstream bf(bippar_path);
				for (const auto & item : temp)
					bf << item;
			}
			else
				std::cout << "Can't open bippar files" << std::endl;
		}
		catch (std::exception & ex){
			std::cout << ex.what() << std::endl;
		}

		bf.close();
	}

	void CopyResults(const std::string&currentState)
	{
		// 1. Создать список файлов для копирования
		// 2. Создать директории в которые будет происходить копирование
		// 3. Скопировать
		///
		//// 1.
		Clear();
		std::string safe_result_Path = m_result_folder_dir + currentState+"/";
		std::string nostra_work_path = current_dir + "REZ" + "/";
		CreateDir(safe_result_Path);
		CreateDir(safe_result_Path + "REZ/");

		std::string entire;
		for (const auto & entry : std::filesystem::recursive_directory_iterator(nostra_work_path)) {
			entire = entry.path().string();
			savings.push_back(entire);
			for (unsigned int i = 0; i < current_dir.size(); i++)
				entire.erase(entire.begin());
			m_currentFile.push_back(safe_result_Path + entire);
		}
		///
		//// 2.
		for (unsigned int i = 0; i < savings.size(); i++) {
			std::string _old = savings[i];
			std::string _new = m_currentFile[i];
			int err_code = static_cast<int>(CopyFile(_old.c_str(), _new.c_str(), FALSE));
			if (err_code == 1) {
				std::cout << std::fixed << std::setprecision(25) << m_currentFile[i] << " ";
				std::cout << " ... OK";
			}
			else {
				if (static_cast<int>(GetLastError()) == 5) {
					std::cout << "error " << std::fixed << std::setprecision(25) << _new << " is folder\n";
					CreateDir(_new);
				}
				else
					std::cout << "unknown error";
			}
			std::cout << std::endl;

		}
	}

	std::vector<std::string> FilesList()
	{
		m_fileList.clear();
		m_currentFile.clear();
		return m_fileList;
	}

	void CopyInputFiles() {

		for (unsigned int i = 0; i < m_fileList.size(); i++) {
			// copying from _old to _new
			std::string _old = m_fileList[i];
			std::string _new = current_dir + m_currentFile[i];
			std::string TU = m_currentFile[i];
			std::transform(TU.begin(), TU.end(), TU.begin(), ::toupper);
			
			int err_code = static_cast<int>(CopyFile(_old.c_str(), _new.c_str(), FALSE));
			if (err_code == 1) {
				std::cout << std::fixed << std::setprecision(25) << m_currentFile[i] << " ";
				std::cout << " ... OK";
				if (TU.find("BIPPAR") != -1)
					m_par[0] = _new;
				if (TU.find("CONST") != -1)
					m_par[1] = _new;
			}
			else {
				if (static_cast<int>(GetLastError()) == 5)
					std::cout << "error " << std::fixed << std::setprecision(25) << _new << " is folder";
				else
					std::cout << "unknown error";
			}
			std::cout << std::endl;
		}
		ChangeLibrariesPath(m_par[0]);
	}

	std::vector<std::string> GetTestsList() const {
		return m_tests_list;
	}

	void CreateInfo() const 
	{
		std::ofstream info("read_me.txt");
		info << "\tHow work this application:\n";
		info << "\tPreviously, put this application in nostra root directory\n";
		info << "\tFirst step:     creating application folder 'nost_bin/' in current directory. Make sure you have administrator access\n";
		info << "\tSecond step:    reading file 'ip.dat'. This file content names of tests, that being calculate. Names precisely links to folders with input data. Assumed, that 'TN' - first test in tests list\n";
		info << "\tThird step:     creating directory 'nost_bin/results/TN/ that will be store results files\n";
		info << "\tThere is starts a cycle: \n";
		info << "\tFourth step:    opening directory 'nost_bin/tests/TN', that contains test with name TN.\n";
		info << "\tFifth step:	 reading file 'HINSTANCE.vmn' with list of file names, that will be copied in current nostra work directory\n";
		info << "\tSixth step:	 begining of calculations - run NOSTRA.exe\n";
		info << "\tSeventh step:   copying results files to 'nost_bin/results/', switch TN to TN+1, and back to step 3.\n";
	}

	void Clear() {
		m_fileList.clear();
		m_currentFile.clear();
		m_par.clear();
	}

private:
	std::string m_input_file_name;
	std::string m_result_folder_dir;
	std::string m_tests_dir;
	
	// Current NOSTRA DIR
	std::string current_dir;

	// Vector with tested folders
	std::vector<std::string> m_tests_list;

	// Relate to current test files
	std::vector<std::string> m_fileList;
	std::vector<std::string> m_currentFile;
	std::vector<std::string> savings;
	std::map<int, std::string> m_par;
};
