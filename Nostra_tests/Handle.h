#pragma once


void Tests::GetFilesList(const std::string& folder)
{
	ToConsoleS(__FUNCTION__);
	std::string path = folder + char(92);
	for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
		std::string entire = entry.path().string();

		m_absoluteFileList.push_back(entry.path().string());

		for (unsigned int i = 0; i < path.size(); i++) {
			entire.erase(entire.begin());
		}

		m_relativeFileList.push_back(entire);
	}
}

std::vector<std::string> Tests::GetTestsList() const {
	return m_tests_list;
}

void Tests::ChangeLibrariesPath() {


	std::string t;
	std::ifstream bf(m_par.at("BIPPAR"));
	std::string current_string;
	std::vector<std::string> temp;
	ToConsoleS(__FUNCTION__);


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
		for (const auto& item : temp)
			bf << item;
	}


	bf.close();
}

void Tests::Calculation(const std::string& t)
{
	GetErrorMessage(GetTime());
	ToConsoleS(__FUNCTION__);
		GetFilesList(m_tests_dir + t);
		CreateFolders("");
		CopyFiles(m_absoluteFileList, m_relativeFileList, "");
		ChangeLibrariesPath();
		if (!GetErrors()) {
			ToConsoleS("... Try to start NOSTRA.exe\n");
			system("nostra.exe");
		}
	Clear();
}

std::string Tests::GetLastWord(const std::string& stc) const
{
	std::string r, s;
	bool t = true;
	for (size_t i = stc.size() - 1; i != -1; i--)
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
	for (size_t i = r.size() - 1; i != -1; i--)
	{
		s += r[i];
	}

	return s;
}

void Tests::FoldersReplacement(const std::string t)
{
	ToConsoleS(__FUNCTION__);
	CreateDir(m_result_folder_dir + "/" + t);
	CreateDir(m_result_folder_dir + "/" + t + "/REZ");
	CreateFolders(m_result_folder_dir + "/" + t + "/" + "REZ/");
	CreateFolders(m_current_dir + "REZ/");
}

void Tests::DeleteFolder()
{
	try {
		ToConsoleS(__FUNCTION__);
		std::string path = "REZ/";
		std::filesystem::remove_all(path);
	}
	catch (std::exception delete_folder)  {
		ToConsoleS(delete_folder.what());
	}
}

void Tests::CopyRes(const std::string t)
{

	GetFilesList(m_current_dir + "REZ");
	FoldersReplacement(t);
	CopyFiles(m_absoluteFileList, m_relativeFileList, m_result_folder_dir + "/" + t + "/REZ/");


	DeleteFolder();

	Log();

	Clear();
}

void Tests::Log()
{
	std::ofstream ofs("autotest.log");
	for (const auto& t : log)
	{
		ofs << t << "\n";
	}
	ofs.close();
}

void Tests::GetTestsCases(const std::string& inputFile) {
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

void Tests::CopyFiles(const std::vector<std::string>& source, const std::vector<std::string>& destination,
	const std::string addictor) {


	std::string t1;
	ToConsoleS(__FUNCTION__);
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
				t1 = "copy error" + source[i] + " - that is folder";
			}
			else {
				t1 = "##>>> UNKNOWN ERROR <<<##";

			//	GetErrorMessage("##>>> UNKNOWN ERROR <<<##");
			//	SetErrors(true);
			//	return;
			}
		}

		ToConsoleS(t1);
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

void Tests::Clear() {
	m_folderToCreating.clear();
	m_relativeFileList.clear();
	m_absoluteFileList.clear();
	m_par.clear();
}

void Tests::SetErrors(bool p)
{
	m_error = p;
}

bool Tests::GetErrors() const
{
	return m_error;
}

void Tests::GetErrorMessage(const std::string& err_message)
{
	m_error_msg.push_back(err_message);
}


void Tests::WriteToErrors(int _time)
{
	std::ofstream ofs("autotests_errors.log", std::ios_base::app);
	ofs << ">>>>>>>";
	for (const auto& item : m_error_msg)
		ofs << item << "\n";
	if (static_cast<int>(_time) < 10000000)
		ofs << "NOSTRA  TIMERUNNNING ERROR\n";
	ofs << "Elapsed time duration: " << _time << "\n";
	if (GetErrors())
		ofs << "##>>> ERRORS FOUND! <<<##\n";
	ofs << "\n";

	m_error_msg.clear();
	ofs.close();
}