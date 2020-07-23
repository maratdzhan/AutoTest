#ifndef TESTING_RESULTS_H
#define TESTING_RESULTS_H

void Tests::ErrorMessages(const std::string& _message)
{
	m_err_msgs.push_back(_message);
}

void Tests::Double(const std::string& _msg, int parameter)
{
	ToConsoleS(_msg);

	std::string _color;
	std::string  _font;
	std::string _size;

	switch (parameter)
	{
	case 1:
		// YELLOW
		_font = "<p class=\"info\"><h3>" + GetTime() + " " + _msg + "</h3></p>";
		break;
	case 2:
		// GREEN
		_font = "<p class=\"start\"><font color = \"lightgreen\">" + GetTime() + " " + _msg + "</font></p>";
		break;
	case 3:
		// RED
		_font = "<p class=\"err\"><font color = \"red\"><u>" + GetTime() + " " + _msg + "</u></font></p>";
		break;
	case -1:
		_font = "<p class=\"hdr\"><font color = \"red\"><h2>######################</h2></font></p><details>";
		break;
	case 4:
		_err = 1;
		_font = "<p class=\"dur\"><font color = \"red\">" + _msg + "</font></p>";
		break;
	case 5:
		_font = "</details>";
		break;
	case 0:
		_font = "<p class=\"initial\"><h1>~~~~#" + GetTime() + "#~~~~~</h1></p>";
		break;
	default:

		break;
	}

	ErrorMessages(_font);

}

void Tests::End(const std::string & value)
{
	Double("", 5);
	std::string name = "autotest_error_" + value + ".html";
	std::ofstream ofs(name, std::ios_base::app);

	for (size_t k = 0; k < m_err_msgs.size(); k++)
	{
		ofs << m_err_msgs[k] << std::endl;
	}

	ofs << "\n\n";
	m_err_msgs.clear();
	ofs.close();
}


#endif // !TESTING_RESULTS_H