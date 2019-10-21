#pragma once



template<class T>
void Tests::ToConsole(const T& message)
{
	std::string v = std::to_string(message);
	ToConsoleS(v);
}

void Tests::ToConsoleS(const std::string& message)
{
	log.push_back(message);
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
}

void Tests::Begin()
{
	ToConsoleS("BEGIN OF CALCULATING:\n");
	ToConsoleS(GetTime());
	ToConsoleS("\n");
}


void Tests::End()
{
	ToConsoleS("END OF CALCULATING:\n");
	ToConsoleS(GetTime());
	ToConsoleS("\n######################################\n");
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