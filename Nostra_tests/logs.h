#ifndef LOGS_H
#define LOGS_H

template<class T>
void Tests::ToConsole(const T& message)
{
	std::string v;
	if (message == 183)
		v = "Not exist file";
	ToConsoleS(v);
}

void Tests::ToConsoleS(const std::string& message)
{
	log.push_back(message);
}

void Tests::Log()
{
	std::ofstream ofs("autotest.log");
	for (const auto& t : log)
	{
		ofs << t << std::endl;
	}
	ofs.close();
}


#endif // !LOGS_H