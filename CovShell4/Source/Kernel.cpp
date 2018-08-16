#ifndef COV_SHELL_KERNEL_CPP


#define COV_SHELL_KERNEL_CPP


#include "Kernel.h"


std::vector < std::string > csl::split(const std::string & s)
{
	std::vector < std::string > input;
	typedef std::string::size_type string_size;
	string_size i = 0;
	while (i != s.size())
	{
		while (i != s.size() && std::isspace(s[i]))
			++i;
		string_size j = i;
		while (j != s.size() && !std::isspace(s[j]))
			++j;
		if (i != j)
		{
			input.push_back(s.substr(i, j - i));
			i = j;
		}
	}
	return input;
}


int csl::findchar(const std::string & chars, char ch)
{
	for (int i = 0; i != chars.size(); ++i)
	{
		if (chars[i] == ch)
		{
			return i + 1;
		}
	}
	return 0;
}

void csl::basic_lang::entry_db()
{
	inflow.open(config_direct + "languages.config");
	if (!inflow)
	{
		std::cout << "Open Config File Error!" << std::endl;
		return;
	}
	std::map < std::string, std::string > config;
	std::string tmp0;
	std::string tmp1;
	std::string line;
	while (std::getline(inflow, line))
	{
		if (line[0] == '#')
			continue;
		tmp0 = tmp1 = "";
		int i;
		for (i = 0; line[i] != '='; ++i)
			tmp0.push_back(line[i]);
		for (++i; i < line.size(); ++i)
			tmp1.push_back(line[i]);
		langs.push_back(tmp0);
		config[tmp0] = tmp1;
	}
	tmp0 = tmp1 = line = "";
  for (auto & it:config)
	{
		inflow.close();
		tmp0 = config_direct + it.second;
		inflow.open(tmp0.c_str());
		if (!inflow)
			std::cout << "Open Language File Error!" << std::endl;
		std::map < std::string, std::string > db;
		while (std::getline(inflow, line))
		{
			if (line[0] == '#')
				continue;
			tmp0 = tmp1 = "";
			int i;
			for (i = 0; line[i] != '='; ++i)
				tmp0.push_back(line[i]);
			for (++i; i < line.size(); ++i)
				tmp1.push_back(line[i]);
			db[tmp0] = tmp1;
		}
		data_base[it.first] = db;
	}
}


std::string csl::basic_lang::find(const std::string & lang, const std::string & code)
{
	if (data_base.find(lang) == data_base.end())
		return "Find Language Fail!Code="+code;
	std::map < std::string, std::string > &db = data_base.find(lang)->second;
	if (db.find(code) == db.end())
		return "Find Config Fail!Code="+code;
	return db.find(code)->second;
}


void csl::basic_io::clrscr()
{
	printf("\x1B[2J\x1B[0;0f");
	fflush(stdout);
}


void csl::basic_io::pause()
{
	termios newt;
	tcgetattr(0, &newt);
	newt.c_lflag &= ~ICANON;
	newt.c_lflag &= ~ECHO;
	tcsetattr(0, TCSANOW, &newt);
	std::cin.get();
	newt.c_lflag |= ICANON;
	newt.c_lflag |= ECHO;
	tcsetattr(0, TCSANOW, &newt);
}


void csl::basic_io::echo(bool in)
{
	struct termios oldt, newt;
	tcgetattr(0, &oldt);
	newt = oldt;
	if (in)
	{
		printf("\33[?25h");
		fflush(stdout);
		newt.c_lflag |= ECHO;
		newt.c_lflag |= ICANON;
	}
	else
	{
		printf("\33[?25l");
		fflush(stdout);
		newt.c_lflag &= ~ECHO;
		newt.c_lflag &= ~ICANON;
	}
	tcsetattr(0, TCSANOW, &newt);
}


#endif
