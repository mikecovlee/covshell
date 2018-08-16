#include "Plugin.h"


namespace csl
{
	class c_echo;
}


class csl::c_echo:public baseShellFunc
{
  public:
	c_echo()
	{
		m_name = "echo";
	}
	int exec(const std::string &);
};


static csl::c_echo echo;


void csl::plugin::install()
{
	csl::this_plugin.name("CSL_ECHO");
	csl::shell->regist_func(&echo);
}


void csl::plugin::uninstall()
{
	csl::shell->remove_func(&echo);
}


int csl::c_echo::exec(const std::string & cmd)
{
	using namespace std;
	auto v = split(cmd);
	if (v.size() > 0)
	{
		if (v[0] == "on")
		{
			shell->echo(true);
			cout << language->find(language->language(), "shell.echo.echo_on") << endl;
			return 0;
		}
		if (v[0] == "off")
		{
			shell->echo(false);
			cout << language->find(language->language(), "shell.echo.echo_off") << endl;
			return 0;
		}
		string str;
		str = v[0];
		for (int i = 1; i != v.size(); ++i)
			str += " " + v[i];
		cout << str << endl;
		return 0;
	}
	if (shell->echo())
	{
		cout << language->find(language->language(), "shell.echo.echo_on") << endl;
		return 0;
	}
	else
	{
		cout << language->find(language->language(), "shell.echo.echo_off") << endl;
		return 0;
	}
	return 0;
}
