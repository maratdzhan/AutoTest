#include "main.h"


int main() {

	SYSTEMTIME st;
	GetLocalTime(&st);

	Tests test;

	srand(st.wYear + st.wMonth + st.wDay + st.wHour + st.wMinute + st.wSecond);
	std::string _uid = std::to_string((rand()));
	int i = 0;
	try {
		if (test.IsInitialized()) {
			std::vector<std::string> container = test.GetTestsList();
			for (const auto& t : container)
			{
				std::cerr << "Test: ";
				std::cerr << i++ << "/" << container.size() << std::endl;

				test.Double(t, 1);
				auto t1 = std::chrono::high_resolution_clock::now();
				test.Calculation(t);

				auto t2 = std::chrono::high_resolution_clock::now();
				auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
				if ((duration / 1000000.) < 2) {
					test.Double("nostra_runtime duration:", 3);
					test.Double(std::to_string(duration / 1000000.), 4);
					test.Double("Result Copying Skipped", 3);
				}
				else
				{
					test.CopyRes(t);
				}
				system("cls");
				test.End(_uid);
			}
		}
	}
	catch (std::exception & some_error)
	{
		std::cerr << some_error.what();
		test.End(_uid);
	}

	return 0;
}