#ifndef FILESYS_H
#define FILESYS_H

void Tests::FoldersReplacement(const std::string t)
{
	CreateDir(m_result_folder_dir + "/" + t);
	CreateDir(m_result_folder_dir + "/" + t + "/REZ");
	CreateFolders(m_result_folder_dir + "/" + t + "/" + "REZ/");
	CreateFolders(m_current_dir + "REZ/");
}

void Tests::GetFilesList(const std::string& folder)
{
	Double(__FUNCTION__, 2);
	try {
		std::string path = folder + char(92);
		for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
			std::string entire = entry.path().string();
			m_absoluteFileList.push_back(entry.path().string());

			for (unsigned int i = 0; i < path.size(); i++)
				entire.erase(entire.begin());

			m_relativeFileList.push_back(entire);
		}
	}
	catch (std::exception & rec_it)
	{
		ToConsoleS(rec_it.what());
		throw(rec_it);
	}
}

void Tests::CreateFolders(std::string addictor)
{
	Double(__FUNCTION__, 2);
	try {
		std::ifstream ifs;
		for (unsigned int i = 0; i < m_absoluteFileList.size(); ++i)
		{
			ifs.open(m_absoluteFileList[i]);
			if (ifs.is_open())
				ifs.close();
			else
				CreateDir(addictor + m_relativeFileList[i]);
		}
	}
	catch (std::exception & cf_err)
	{
		Double(cf_err.what(), 3);
	}
}

void Tests::CreateDir(const std::string& directory) {
	// CREATE DIR WORKS PERFECTLY
	std::string t;
	if (CreateDirectory(directory.c_str(), NULL))
		t = "- Directory " + (directory)+" created. ";
	else {
		if (GetLastError() == ERROR_ALREADY_EXISTS)
			t = "Directory " + directory + " already exists.";
		else if (GetLastError() == ERROR_PATH_NOT_FOUND)
			t = "WARNING! Directory " + directory + " is not created.";
	}
	ToConsoleS(t);
}

void Tests::DeleteFolder()
{
	Double(__FUNCTION__, 2);
	std::string path = "REZ/";
	std::filesystem::remove_all(path);
}

void Tests::CopyRes(const std::string t)
{
	Double(__FUNCTION__, 2);
	try {
		GetFilesList(m_current_dir + "REZ");
		FoldersReplacement(t);
		CopyFiles(m_absoluteFileList, m_relativeFileList, m_result_folder_dir + "/" + t + "/REZ/");
		DeleteFolder();
	}
	catch (std::exception & exc_c) {
		Double(exc_c.what(), 3);
	}
	Log();
	Clear();
}

void Tests::CopyFiles(const std::vector<std::string>& source, const std::vector<std::string>& destination,
	const std::string addictor) {

	Double(__FUNCTION__, 2);

	try {
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
				if (static_cast<int>(GetLastError()) == 5)
					t1 = "error when copy file " + source[i] + "(copy folder)";
				else
					t1 = "unknown error";
			}
			ToConsoleS(t1);
		}
	}
	catch (std::exception & cp_err)
	{
		ToConsoleS(cp_err.what());
		ErrorMessages(cp_err.what());
	}
}


void Tests::ChangeLibrariesPath() {

	Double(__FUNCTION__, 2);

	try {
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
					current_string = "NAMCON='" + m_current_dir +
						GetLibrary(current_string) + "',\n";
				}
				temp.push_back(current_string + "\n");
			}
			bf.close();
			std::ofstream bf(m_par.at("BIPPAR"));
			for (const auto& item : temp)
				bf << item;
		}
		else
			Double("Can't open bippar files", 3);

		bf.close();
	}
	catch (std::exception & exc)
	{
		ToConsoleS(exc.what());
		throw(exc);
	}
}

#endif // !FILESYS_H
