#include "main.h"

std::string GetTime() {
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

int main() {
	Tests test;
	std::vector<std::string> container = test.GetTestsList();
	

	std::string t1;
	for (const auto & t : container)
	{

		test.ToConsoleS("BEGIN OF CALCULATING:\n");
		test.ToConsoleS(GetTime());
		test.ToConsoleS("\n");

		test.Calculation(t);
		test.CopyRes(t);
		
		test.ToConsoleS("END OF CALCULATING:\n");
		test.ToConsoleS(GetTime());
		test.ToConsoleS("\n######################################\n");
	}

//	test.Log();
//	system("pause");
	return 0;
}