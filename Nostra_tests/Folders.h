#pragma once


void Tests::CreateFolders(std::string addictor)
{

	ToConsoleS(__FUNCTION__);
	std::ifstream ifs;
	for (unsigned int i = 0; i < m_absoluteFileList.size(); ++i)
	{
		HANDLE			hFind = INVALID_HANDLE_VALUE;
		WIN32_FIND_DATA FindFileData;
		hFind = FindFirstFile(m_absoluteFileList[i].c_str(), &FindFileData);
		if ((FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY) == 0)
			CreateDir(addictor + m_relativeFileList[i]);

	}
}


void Tests::CreateDir(const std::string& directory) {
	// CREATE DIR WORKS PERFECTLY
	std::string t;
	if (CreateDirectory(directory.c_str(), NULL))
	{
		t = "- Directory " + (directory)+" created. ";
		ToConsoleS(t);
	}
	else {
		ToConsole(GetLastError());
		if (GetLastError() == ERROR_ALREADY_EXISTS) {
			t = "Directory " + directory + " already exists. ";

		}
		else if (GetLastError() == ERROR_PATH_NOT_FOUND) {
			t = "WARNING! Directory " + directory + " is not created. ";
		}
	}
	ToConsoleS(t);
}