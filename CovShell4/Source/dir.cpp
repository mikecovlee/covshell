#include "Plugin.h"
#include <dirent.h>


static std::fstream fctrl;


inline bool openfile(const std::string & dir)
{
	fctrl.close();
	fctrl.open(dir);
	if (fctrl)
		return true;
	else
		return false;
}


inline void dir(const std::string & d)
{
	using namespace std;
	using namespace csl;
	dirent **dstr;
	if (!opendir(d.c_str()))
	{
		cout << language->find(language->language(), "shell.unknow_direct") << endl;
		return;
	}
	int n = scandir(d.c_str(), &dstr, 0, alphasort);
	std::vector < std::string > files;
	for (int i = 0; i < n; i++)
	{
		files.push_back(dstr[i]->d_name);
		free(dstr[i]);
	}
	std::string::size_type maxlen = 0;
	for (int i = 0; i != files.size(); ++i)
		maxlen = std::max(maxlen, files[i].size());
	int dirs = 0;
	int ffiles = 0;
	std::cout << " " + language->find(language->language(), "shell.dir") + " " + d << std::endl;
	for (int i = 0; i != files.size(); ++i)
	{
		std::string t = d + files[i];
		std::string space(maxlen + 7 - files[i].size(), ' ');
		std::string space0(maxlen + 2 - files[i].size(), ' ');
		if (opendir(t.c_str()))
		{
			std::cout << files[i] + space + "<DIR>" << std::endl;
			++dirs;
			continue;
		}
		else if (openfile(t))
		{
			int point = files[i].size();
			while (point != 0)
			{
				if (files[i][point] == '.')
					break;
				--point;
			}
			if (point == 0)
			{
				std::cout << files[i] + space0 + "----" << std::endl;
				++ffiles;
				continue;
			}
			std::string ttem = files[i].substr(point + 1, files[i].size() - point - 1);
			std::cout << files[i] + space0 + ttem << std::endl;
			++ffiles;
			continue;
		}
		std::cout << files[i] + space0 + "----" << std::endl;
	}
	std::string space1(maxlen + 2, ' ');
	std::cout << std::endl;
	std::cout << space1 << ffiles << " " << language->find(language->language(),
														   "shell.file") << std::endl;
	std::cout << space1 << dirs << " " << language->find(language->language(),
														 "shell.dir") << std::endl;
}


namespace csl
{
	class c_dir;
}


class csl::c_dir:public baseShellFunc
{
  public:
	c_dir()
	{
		m_name = "dir";
	}
	int exec(const std::string &);
};


static csl::c_dir cdir;


void csl::plugin::install()
{
	csl::this_plugin.name("CSL_DIR");
	csl::shell->regist_func(&cdir);
}


void csl::plugin::uninstall()
{
	csl::shell->remove_func(&cdir);
}


int csl::c_dir::exec(const std::string & cmd)
{
	using namespace std;
	auto v = split(cmd);
	if (v.size() > 0)
	{
		string directory;
		if (v[0] != "." && v[0] != "..")
		{
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
		}
		else if (v[0] == "..")
		{
			directory = shell->home;
			if (directory.size() != 1)
				directory.erase(directory.size() - 1);
			for (int k = directory.size() - 1; directory[k] != '/'; --k)
			{
				directory.erase(k);
			}
		}
		dir(directory);
		return 0;
	}
	dir(shell->home);
	return 0;
}
