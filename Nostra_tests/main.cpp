#include "main.h"


int main() {
	Tests test;
	std::vector<std::string> container = test.GetTestsList();
	
	for (const auto & t : container)
	{

	//	test.Calculation(t);
		test.CopyRes(t);
	}


	return 0;
}