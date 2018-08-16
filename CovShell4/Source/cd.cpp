#include "Plugin.h"
#include <dirent.h>


namespace csl
{
	class c_cd;
}


class csl::c_cd:public baseShellFunc
{
  public:
	c_cd()
	{
		m_name = "cd";
	}
	int exec(const std::string &);
};

static csl::c_cd cd;


void csl::plugin::install()
{
	csl::this_plugin.name("CSL_CD");
	csl::shell->regist_func(&cd);
}


void csl::plugin::uninstall()
{
	csl::shell->remove_func(&cd);
}


int csl::c_cd::exec(const std::string & cmd)
{
	using namespace std;
	auto v = split(cmd);
	if (v.size() == 0)
	{
		cout << language->find(language->language(), "shell.syntax_fault") << endl;
		return 0;
	}
	if (v[0] != "." && v[0] != "..")
	{
		string directory;
		string tmp_dir = v[0];
		for (int i = 1; i != v.size(); ++i)
			tmp_dir = tmp_dir + " " + v[i];
		if (findchar(tmp_dir, '/') == 0)
		{
			directory = shell->home + tmp_dir + "/";
		}
		else
		{
			if (tmp_dir[tmp_dir.size() - 1] != '/')
			{
				directory = tmp_dir + "/";
			}
			else
			{
				directory = tmp_dir;
			}
		}
		if (!opendir(directory.c_str()))
		{
			cout << language->find(language->language(), "shell.unknow_direct") << endl;
			return 0;
		}
		shell->home = directory;
	}
	else if (v[0] == "..")
	{
		if (shell->home.size() != 1)
			shell->home.erase(shell->home.size() - 1);
		for (int k = shell->home.size() - 1; shell->home[k] != '/'; --k)
		{
			shell->home.erase(k);
		}
	}
	return 0;
}
