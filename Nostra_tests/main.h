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
	//// √Œ“Œ¬Œ
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
		FillContainer(m_input_file_name);
		m_current_dir = m_tests_list[0];
		m_tests_list.erase(m_tests_list.begin());


		CreateDir("REZ");
		CreateDir("REZ/REZ");
		CreateDir("REZ/REZ_BAYES");
		CreateDir("REZ/REZ_BURNUP");
		CreateDir("REZ/REZ_DATA");
		CreateDir("REZ/REZ_DATA_EXCH");
		CreateDir("REZ/REZ_XENON");

	}

	template<class T>
	void ToConsole(const T & message)
	{
		std::string v = std::to_string(message);
		log.push_back(v);
		std::cout << message;
	}

	void ToConsoleS(const std::string & message)
	{
		log.push_back(message);
		std::cout << message;
	}

	void CreateFolders(std::string addictor)
	{
		std::ifstream ifs;
		for (unsigned int i = 0; i < m_absoluteFileList.size(); ++i)
		{
			ifs.open(m_absoluteFileList[i]);
			if (ifs.is_open())
				ifs.close();
			else {
				CreateDir(addictor + m_relativeFileList[i]);
			}

		}
		ToConsoleS(t_ns);
	}

	void CreateDir(const std::string & directory){
		// CREATE DIR WORKS PERFECTLY
		std::string t;
		if (CreateDirectory(directory.c_str(), NULL))
		{
			t = "- Directory " + (directory)+" created. " + t_ns;
			ToConsoleS(t);
		}
		else {
			ToConsole(GetLastError());
			if (GetLastError() == ERROR_ALREADY_EXISTS) {
				t = "Directory " + directory + " already exists. " + t_ns;
				
			}
			else if (GetLastError() == ERROR_PATH_NOT_FOUND) {
				t = "WARNING! Directory " + directory + " is not created. " + t_ns;
			}
		}
		ToConsoleS(t);
	}

	void GetFilesList(const std::string & folder)
	{
		std::string path = folder + char(92);
		for (const auto & entry : std::filesystem::recursive_directory_iterator(path)) {
			std::string entire = entry.path().string();

			m_absoluteFileList.push_back(entry.path().string());

			for (unsigned int i = 0; i < path.size(); i++) {
				entire.erase(entire.begin());
			}

			m_relativeFileList.push_back(entire);
		}

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

	std::vector<std::string> GetTestsList() const {
		return m_tests_list;
	}

	void ChangeLibrariesPath() {
		std::string t;
		std::ifstream bf(m_par.at("BIPPAR"));
		std::string current_string;
		std::vector<std::string> temp;

		if (bf.is_open())
		{
			while (!bf.eof()) {
				getline(bf, current_string);
				if (current_string.find("NAMCON") != -1)
				{
					std::string val = "NAMCON='" + m_current_dir +
						GetLibrary(current_string) + "',\n";
					temp.push_back(val);
					int k = 0;
				}
				else
					temp.push_back(current_string + "\n");
			}
			bf.close();
			std::ofstream bf(m_par.at("BIPPAR"));
			for (const auto & item : temp)
				bf << item;
		}
		else {
			t = ("Can't open bippar files\n");
			ToConsoleS(t);
		}


		bf.close();

	}

	void Calculation(const std::string & t)
	{
		try {
			GetFilesList(m_tests_dir + t);
			CreateFolders("");
			CopyFiles(m_absoluteFileList, m_relativeFileList, "");
			ChangeLibrariesPath();
			int i_err = -1;
			ToConsoleS("... Try to start NOSTRA.exe\n");
			i_err = system("nostra.exe");
			std::string t1;
			if (i_err != 0)
				ToConsole(GetLastError());
			else
			{
				t1 = "End of state";
				ToConsoleS(t1);
				ToConsoleS(t_ns);
			}
		}
		catch (std::exception & exc){
			ToConsoleS(exc.what());
			Log();
		}
		Clear();
	}

	std::string GetLastWord(const std::string &stc) const
	{
		std::string r, s;
		bool t = true;
		for (int i = stc.size() - 1; i != -1; i--)
		{
			if (t) {
				if ((stc[i] != '/') && (stc[i] != 92)) {
					r += stc[i];
				}
				else {
					t = false;
				}
			}
		}
		for (int i = r.size() - 1; i != -1; i--)
		{
			s += r[i];
		}

		return s;
	}

	void FoldersReplacement(const std::string t)
	{
		CreateDir(m_result_folder_dir + "/" + t);
		CreateDir(m_result_folder_dir + "/" + t + "/REZ");
		CreateFolders(m_result_folder_dir + "/" + t + "/" + "REZ/");
		CreateFolders(m_current_dir + "REZ/");

	}

	void DeleteFolder()
	{
			std::string path = "REZ/";
			std::filesystem::remove_all(path);
	}

	void CopyRes(const std::string t)
	{
		try {
			GetFilesList(m_current_dir + "REZ");
			FoldersReplacement(t);
			CopyFiles(m_absoluteFileList, m_relativeFileList, m_result_folder_dir + "/" + t + "/REZ/");


			DeleteFolder();
		}
		catch (std::exception & exc_c){
			ToConsoleS(exc_c.what());
			Log();
		}
		Clear();
	}

	void Log()
	{
		std::ofstream ofs("autotest.log");
		for (const auto & t : log)
		{
			ofs << t;
		}
		ofs.close();
	}

	std::vector<std::string> log;

	~Tests() {
		Log();
	}

private:
	void FillContainer(const std::string & inputFile) {
		// FILL CONTINER WORKS PERFECTLY
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
			ToConsoleS(m_input_file_name + " is not opened! (Fill container)\n");
			
		}
	}

	void CopyFiles(const std::vector<std::string> & source, const std::vector<std::string> & destination,
		const std::string addictor) {
		std::string t1;
		ToConsoleS("Start copy files: \n");
		for (unsigned int i = 0; i < source.size(); i++) {
			std::string TU = destination[i];
			std::transform(TU.begin(), TU.end(), TU.begin(), ::toupper);
			std::string dest = addictor + destination[i];
			int err_code = static_cast<int>(CopyFile(source[i].c_str(), dest.c_str(), FALSE));
			if (err_code == 1) {
				t1 = dest + "... OK";
				if (TU == "BIPPAR")
					m_par["BIPPAR"] = destination[i];
				if (TU.find("CONST") != -1)
					m_par[TU] = destination[i];
			}
			else {
				if (static_cast<int>(GetLastError()) == 5) {
					t1="error when copy file " +source[i] + " - that is folder";
				}
				else {
					t1= "unknown error";
				}
			}
			t1 += "\n";
			ToConsoleS(t1);
		}
	}

	std::string GetLibrary(const std::string & str) const {
		std::string temp = str;
		std::transform(temp.begin(), temp.end(), temp.begin(), ::toupper);
		for (auto & t : m_par)
		{
			std::string currentName = GetLastWord(t.second);
			std::transform(currentName.begin(), currentName.end(), currentName.begin(), ::toupper);
			if (temp.find(currentName) != -1)
				return t.second;
		}

		throw (std::invalid_argument("!!!WARNING\nNOT FIND LIBRARY\n"+ str+"\n!!!\n"));
		return "0";
	}

	void Clear()
	{
		m_folderToCreating.clear();
		m_relativeFileList.clear();
		m_absoluteFileList.clear();
		m_par.clear();
	}

private:
	std::string m_input_file_name;
	std::string m_result_folder_dir;
	std::string m_tests_dir;
	std::string t_ns = "\n";
	// Current NOSTRA DIR
	std::string m_current_dir;

	// Vector with tested folders
	std::vector<std::string> m_tests_list;

	// Relate to current test files
	std::vector<int> m_folderToCreating;
	std::vector<std::string> m_absoluteFileList;
	std::vector<std::string> m_relativeFileList;
	std::map<std::string,std::string> m_par;
	
};
