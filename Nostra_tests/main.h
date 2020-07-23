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


class Tests {
public:
	Tests();
	template<class T>
	void ToConsole(const T& message);
	void ToConsoleS(const std::string& message);
	void CreateFolders(std::string addictor);
	void CreateDir(const std::string& directory);
	void GetFilesList(const std::string& folder);
	void CreateInfo() const;
	
	void ChangeLibrariesPath();
	void Calculation(const std::string& t);
	std::string GetLastWord(const std::string& stc) const;
	void FoldersReplacement(const std::string t);
	void DeleteFolder();
	void CopyRes(const std::string t);
	void Log();
	std::string GetTime();
	std::vector<std::string> log;
	void ErrorMessages(const std::string& _msg);
	void End(const std::string &);
	void Double(const std::string & _msg, int parameter);

	//
	std::vector<std::string> GetTestsList() const { return m_tests_list; }
	bool IsInitialized() const { return isInitialized; }
	~Tests() { Log(); }

private:
	void GrabTestsList(const std::string& inputFile);
	void CopyFiles(const std::vector<std::string>& source, const std::vector<std::string>& destination,
		const std::string addictor);
	std::string GetLibrary(const std::string& str) const;
	void Clear();
	bool SetInitialParameters();
private:
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
	std::vector<std::string> m_err_msgs;
	
	bool isInitialized;
	int _err;
};

#include "logs.h"
#include "testing_results.h"
#include "filesys.h"
#include "test_handling.h"

Tests::Tests() {
	Double("", 0);
	_err = 0;
	CreateInfo();
	////
	m_input_file_name = "nost_bin/";
	CreateDir(m_input_file_name);
	m_input_file_name += "ip.dat";
	////
	m_result_folder_dir = "nost_bin/results/";
	CreateDir(m_result_folder_dir);
	////
	m_tests_dir = "nost_bin/tests/";
	GrabTestsList(m_input_file_name);
	isInitialized = SetInitialParameters();

	//// BUILD FILESYS FOR NOSTRA
	{
		CreateDir("REZ");
		CreateDir("REZ/REZ");
		CreateDir("REZ/REZ_BAYES");
		CreateDir("REZ/REZ_BURNUP");
		CreateDir("REZ/REZ_DATA");
		CreateDir("REZ/REZ_DATA_EXCH");
		CreateDir("REZ/REZ_XENON");
	}
}

void Tests::CreateInfo() const
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
	info.close();
}

std::string Tests::GetLastWord(const std::string& stc) const
{
	std::string s;
	int t = 0;
	int v1 = max(stc.find_last_of('\\'), stc.find_last_of('/'));

	std::copy_if(stc.begin(), stc.end(), std::back_inserter(s),
		[&t, &v1](char c) { 
			return (t++ > v1); 
		});

	return s;
}

void Tests::GrabTestsList(const std::string& inputFile) {
	
	Double(__FUNCTION__, 2);

	std::ifstream tests_list(inputFile);
	if (tests_list.is_open())
	{
		std::string test_name;
		while (!tests_list.eof())
		{
			getline(tests_list, test_name);
			if (!test_name.empty())
				m_tests_list.push_back(test_name);
		}
	}
	else {
		Double(m_input_file_name + " is not opened! (Fill container)\n", 3);
	}
}

std::string Tests::GetLibrary(const std::string& str) const {
	std::string temp = str;
	std::transform(temp.begin(), temp.end(), temp.begin(), ::toupper);
	for (auto& t : m_par)
	{
		std::string currentName = GetLastWord(t.second);
		std::transform(currentName.begin(), currentName.end(), currentName.begin(), ::toupper);
		if (temp.find(currentName) != -1)
			return t.second;
	}

	throw (std::invalid_argument("!!!WARNING\nNOT FIND LIBRARY\n" + str + "\n!!!\n"));
	return "0";
}

void Tests::Clear()
{
	m_folderToCreating.clear();
	m_relativeFileList.clear();
	m_absoluteFileList.clear();
	m_par.clear();
	
}

std::string Tests::GetTime() {
	SYSTEMTIME st;
	GetLocalTime(&st);
	std::string date;
	date =
		std::to_string(st.wYear) + "-"
		+ std::to_string(st.wMonth) + "-"
		+ std::to_string(st.wDay) + " "
		+ std::to_string(st.wHour) + ":"
		+ std::to_string(st.wMinute) + ":"
		+ std::to_string(st.wSecond) + "."
		+ std::to_string(st.wMilliseconds);

	return date;
}


