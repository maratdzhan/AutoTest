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

	void CreateFolders(std::string addictor) const
	{
		std::ifstream ifs;
		for (unsigned int i = 0; i < m_absoluteFileList.size(); ++i)
		{
			ifs.open(m_absoluteFileList[i]);
			if (ifs.is_open())
				ifs.close();
			else {
				CreateDir(addictor+m_relativeFileList[i]);
			}

		}
		std::cout << std::endl;
	}

	void CreateDir(const std::string & directory) const {
		// CREATE DIR WORKS PERFECTLY
		if (CreateDirectory(directory.c_str(), NULL))
			std::cout << "- Directory " << directory << " created. " << std::endl;
		else {
			std::cout << GetLastError() <<" ";
			if (GetLastError() == ERROR_ALREADY_EXISTS)
				std::cout << "Directory " << directory << " already exists. " << std::endl;
			else if (GetLastError() == ERROR_PATH_NOT_FOUND)
				std::cout << "WARNING! Directory " << directory << " is not created. " << std::endl;
		}
	}

	void GetFilesList(const std::string & folder)
	{
		std::string path = folder+char(92);
		try {
			for (const auto & entry : std::filesystem::recursive_directory_iterator(path)) {
				std::string entire = entry.path().string();

				m_absoluteFileList.push_back(entry.path().string());

				for (unsigned int i = 0; i < path.size(); i++) {
					entire.erase(entire.begin());
				}

				m_relativeFileList.push_back(entire);
			}
		}
		catch (std::exception& ex2){
			std::cout << ex2.what() << std::endl;
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

	void ChangeLibrariesPath() const {
		try {
			std::ifstream bf(m_par.at("BIPPAR"));
			std::string current_string;
			std::vector<std::string> temp;
			try {
				if (bf.is_open())
				{
					while (!bf.eof()) {
						getline(bf, current_string);
						if (current_string.find("NAMCON") != -1)
						{
							temp.push_back("NAMCON='" + m_current_dir +
								GetLibrary(current_string) + "',\n");
						}
						else
							temp.push_back(current_string + "\n");
					}
					bf.close();
					std::ofstream bf(m_par.at("BIPPAR"));
					for (const auto & item : temp)
						bf << item;
				}
				else
					std::cout << "Can't open bippar files" << std::endl;
			}
			catch (std::exception & ex) {
				std::cout << ex.what() << std::endl;
			}

			bf.close();
		}
		catch (std::exception & ex)
		{
			std::cout << ex.what() << std::endl;
		}
	}

	void Calculation(const std::string & t)
	{
		GetFilesList(m_tests_dir + t);
		CreateFolders("");
		CopyFiles(m_absoluteFileList, m_relativeFileList, "");
		ChangeLibrariesPath();
		int i_err = -1;
		system("pause");
		i_err = system("nostra.exe");
		i_err == 0 ? (std::cout << "End of state\n\n") : (std::cout << GetLastError() << std::endl);
		Clear();
	}

	std::string GetLastWord(const std::string &stc) const
	{
		std::string r,s;
		bool t = true;
		for (int i = static_cast<int>(r.size()); i != -1; i--)
		{
			if (t) {
				if ((stc[i] != '\ ') && (stc[i] != '/') && (stc[i] != 92)) {
					r += stc[i];
				}
				else
					t = false;
			}
		}
		for (int i = static_cast<int>(r.size())-1; i !=-1; i--)
		{
			s += r[i];
		}

		return s;
	}

	void FoldersReplacement(const std::string t) const
	{
		CreateDir(m_result_folder_dir + "/" + t);
		CreateDir(m_result_folder_dir + "/" + t + "/REZ");
		CreateFolders(m_result_folder_dir + "/" + t + "/" + "REZ/");
		CreateFolders(m_current_dir + "REZ/");

	}

	void DeleteFolder()
	{
		try {
			std::string path = "REZ/";
			std::filesystem::remove_all(path);
		}
		catch (std::exception & ex1) {
			std::cout << ex1.what() << std::endl;
		}
	}

	void CopyRes(const std::string t)
	{
		GetFilesList(m_current_dir+"REZ");
		FoldersReplacement(t);
		CopyFiles(m_absoluteFileList, m_relativeFileList, m_result_folder_dir + "/"+t + "/REZ/");


		DeleteFolder();
		Clear();
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
		else
			std::cout << m_input_file_name << " is not opened!" << std::endl;
	}

	void CopyFiles(const std::vector<std::string> & source, const std::vector<std::string> & destination,
		const std::string addictor) {

		for (unsigned int i = 0; i < source.size(); i++) {
			std::string TU = destination[i];
			std::transform(TU.begin(), TU.end(), TU.begin(), ::toupper);
			std::string dest = addictor + destination[i];
			int err_code = static_cast<int>(CopyFile(source[i].c_str(), dest.c_str(), FALSE));
			if (err_code == 1) {
				std::cout << std::fixed << std::setprecision(25) << destination[i] << " ";
				std::cout << " ... OK";
				if (TU == "BIPPAR")
					m_par["BIPPAR"] = destination[i];
				if (TU.find("CONST") != -1)
					m_par[TU] = destination[i];
			}
			else {
				if (static_cast<int>(GetLastError()) == 5)
					std::cout << "error " << std::fixed << std::setprecision(25) << source[i] << " is folder";
				else
					std::cout << "unknown error";
			}
			std::cout << std::endl;
		}
	}

	std::string GetLibrary(const std::string & str) const {
		for (auto & t : m_par)
		{
			std::string currentName = GetLastWord(t.second);
			if (str.find(currentName) != -1)
				return t.second;
		}

		throw (std::invalid_argument("NOT FIND LIBRARY"));
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
