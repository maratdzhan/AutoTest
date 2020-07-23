#ifndef TEST_HANDLING_H
#define TEST_HANDLING_H

bool Tests::SetInitialParameters()
{
	try {
		if (m_tests_list.size() > 1)
		{
			m_current_dir = m_tests_list[0];
			m_tests_list.erase(m_tests_list.begin());
			return true;
		}
		else
			ToConsoleS("Not enought datas for testing");
	}
	catch(std::exception & exc)
	{
		ToConsoleS("some errors when initializing:");
		ToConsoleS(exc.what());
	}
	return false;
}

void Tests::Calculation(const std::string& t)
{
	Double("", -1);
	Double(__FUNCTION__, 2);
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
		{
			std::string r = "Nostra running result " + std::to_string(i_err);
			ToConsoleS(r);
			Double(r, 3);
		}
		else
			ToConsoleS("End of state");
	}
	catch (std::exception calc_ex)
	{
		Double(calc_ex.what(), 3);
	}

	Clear();
}

#endif // !TEST_HANDLING_H

