#include <list>
#include <string>
#include <iostream>
// 核心实现部分
namespace csl
{
	class basic_io;
	class baseShellFunc;
	  std::list < baseShellFunc * >func_list;
	int exec(const std::string &, const std::string &);
}


class csl::baseShellFunc
{
  protected:
	std::string m_name;
  public:
	baseShellFunc():m_name("Basic_Shell_Function")
	{
		func_list.push_back(this);
	}
	baseShellFunc(const baseShellFunc &) = delete;
	virtual ~ baseShellFunc()
	{
		func_list.remove(this);
	}
	virtual int exec(const std::string & cmd)
	{
	}
	virtual const std::string & name()
	{
		return m_name;
	}
};


int csl::exec(const std::string & func, const std::string & cmd)
{
  for (auto & i:func_list)
		if (func == i->name())
			return i->exec(cmd);
}


// 功能实现部分
namespace csl
{
	class c_echo;
	class c_ver;
}


class csl::c_echo:public baseShellFunc
{
  public:
	c_echo()
	{
		m_name = "echo";
		func_list.push_back(this);
	}
	int exec(const std::string & cmd)
	{
		std::cout << cmd << std::endl;
		return 0;
	}
};


int main()
{
	csl::c_echo echo;
	using namespace std;
	string input;
	getline(cin, input);
	string func, cmd;
	int i;
	for (i = 0; input[i] != ' ' && i < input.size(); ++i)
		func += input[i];
	for (++i; i < input.size(); ++i)
		cmd += input[i];
	cout << func << "@" << cmd << endl;
	return csl::exec(func, cmd);
}