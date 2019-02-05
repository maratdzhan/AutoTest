#include "main.h"


int main() {
	Tests test;
	std::vector<std::string> container = test.GetTestsList();
	int i_err = -1;
	for (const auto & t : container)
	{
		test.GetFilesList(t);
		test.CopyInputFiles();

		
//		system("pause");
		i_err=system("nostra.exe");
		i_err == 0 ? (std::cout << "End of state\n\n") : (std::cout << GetLastError() << std::endl);


		test.CopyResults(t);
		test.Clear();
	}


	return 0;
}