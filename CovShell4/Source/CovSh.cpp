#ifndef COV_SHELL_CPP


#define COV_SHELL_CPP


#include "CovSh.h"


int csl::basic_shell::run(int args, char **argv)
{
	using namespace std;
	c_plug plug;
	c_lang lang;
	cout << "Covariant Shell " << version << endl;
	bool exit = false;
	while (!exit)
	{
		if (m_echo)
			cout << home << '>';
		string line;
		getline(cin, line);
		if (line == "exit")
			return 0;
		exec(line);
	}
	return 0;
}


int main(int args, char **argv)
{
	csl::init();
	csl::shell->run(args, argv);
	csl::destory();
}


#endif