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
#include <ctime>
#include <chrono>



class Tests {
public:
	//// √Œ“Œ¬Œ
	Tests();
	void CreateFolders(std::string addictor);
	void CreateDir(const std::string& directory);

	template<class T>
	void ToConsole(const T& message);
	void ToConsoleS(const std::string& message);
	void CreateInfo() const;
	void GetFilesList(const std::string& folder);
	std::vector<std::string> GetTestsList() const;
	void ChangeLibrariesPath();
	void Calculation(const std::string& t);
	std::string GetLastWord(const std::string& stc) const;
	void FoldersReplacement(const std::string t);
	void DeleteFolder();
	void CopyRes(const std::string t);
	void Log();
	void Begin();
	void End();
	bool GetErrors()const;
	void SetErrors(bool _parameter);
	std::vector<std::string> log;
	std::string GetTime();
	void GetErrorMessage(const std::string& err_message);
	void WriteToErrors(int _time);

	~Tests() {
		Log();
	}

protected:
	void GetTestsCases(const std::string& inputFile);
	void CopyFiles(const std::vector<std::string>& source, const std::vector<std::string>& destination,
		const std::string addictor);
	std::string GetLibrary(const std::string& str) const;
	void Clear();


	std::string m_input_file_name;
	std::string m_result_folder_dir;
	std::string m_tests_dir;
	// Current NOSTRA DIR
	std::string m_current_dir;
	// Vector with tested folders
	std::vector<std::string> m_tests_list;
	// Relate to current test files 
	std::vector<int> m_folderToCreating;
	std::vector<std::string> m_absoluteFileList;
	std::vector<std::string> m_relativeFileList;
	std::map<std::string,std::string> m_par;
	bool m_error;
	std::vector<std::string> m_error_msg;
};


Tests::Tests() {
	CreateInfo();
	//// Create "nost_bin"
	m_input_file_name = "nost_bin/";
	CreateDir(m_input_file_name);
	m_input_file_name += "ip.dat";
	//// create "nost_bin/results"
	m_result_folder_dir = "nost_bin/results/";
	CreateDir(m_result_folder_dir);
	//// Get tests list
	m_tests_dir = "nost_bin/tests/";
	CreateDir(m_tests_dir);
	GetTestsCases(m_input_file_name);
	m_current_dir = m_tests_list[0];
	m_tests_list.erase(m_tests_list.begin());

	// Create additional directories, that may absent
	CreateDir("REZ");
	CreateDir("REZ/REZ");
	CreateDir("REZ/REZ_BAYES");
	CreateDir("REZ/REZ_BURNUP");
	CreateDir("REZ/REZ_DATA");
	CreateDir("REZ/REZ_DATA_EXCH");
	CreateDir("REZ/REZ_XENON");

	m_error = false;

}

#include "Folders.h"
#include "Console.h"
#include "Handle.h"