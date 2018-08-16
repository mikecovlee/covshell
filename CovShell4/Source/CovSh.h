#ifndef COV_SHELL_H


#define COV_SHELL_H


#include "Kernel.cpp"


namespace csl
{
	class c_plug;
	class c_lang;
	static basic_lang *language;
	static basic_shell *shell;
	static basic_io *ioctrl;
	void init();
	void destory();
}


void csl::init()
{
	putenv("HOME=/mnt/sdcard/CovSh");
	language = new basic_lang;
	shell = new basic_shell;
	ioctrl = new basic_io;
	ioctrl->clrscr();
	language->init();
	ioctrl->init();
}


void csl::destory()
{
	delete language;
	delete ioctrl;
	delete shell;
}


int csl::basic_shell::exec(const std::string & in)
{
	std::string func, cmd;
	int i;
	for (i = 0; in[i] != ' ' && i < in.size(); ++i)
		func += in[i];
	for (++i; i < in.size(); ++i)
		cmd += in[i];
  for (auto & i:func_list)
		if (func == i->name())
			return i->exec(cmd);
	if (in.size() != 0)
		std::cout << language->find(language->language(), "shell.unknow_command") << std::endl;
}


csl::basic_io::~basic_io()
{
  for (auto & it:plug_bundle)
	{
		it.first->destory();
		dlclose(it.second);
	}
}


bool csl::basic_io::open(const std::string & file)
{
	void *bundle = dlopen(file.c_str(), RTLD_LAZY);
	if (bundle)
	{
		baseShellPlug *(*entry) () = (baseShellPlug * (*)())dlsym(bundle, "entry");
		plug_bundle[entry()] = bundle;
		entry()->construct(language, shell, ioctrl);
		m_plugins.push_back(entry()->name());
		return true;
	}
	else
		std::cout << "CovSh Open Plugin Error:" << dlerror << std::endl;
	return false;
}


bool csl::basic_io::close(const std::string & name)
{
	for (auto it = plug_bundle.begin(); it != plug_bundle.end(); ++it)
	{
		if (it->first->name() == name)
		{
			m_plugins.remove(name);
			it->first->destory();
			dlclose(it->second);
			plug_bundle.erase(it);
			return true;
		}
	}
	return false;
}


void csl::basic_io::init()
{
	inflow.open(config_direct + "plugins.config");
	if (!inflow)
	{
		std::cout << "Open Config File Error!" << std::endl;
		return;
	}
	std::string line;
	while (std::getline(inflow, line))
	{
		if (line[0] == '#')
			continue;
		open(config_direct + line);
	}
}


csl::baseShellFunc::baseShellFunc():m_name("Covariant_Shell_Function")
{
	shell->regist_func(this);
}


csl::baseShellFunc::~baseShellFunc()
{
	shell->remove_func(this);
}


class csl::c_plug:public baseShellFunc
{
  public:
	c_plug()
	{
		m_name = "plug";
		shell->regist_func(this);
	}
	int exec(const std::string &);
};


class csl::c_lang:public baseShellFunc
{
  public:
	c_lang()
	{
		m_name = "lang";
		shell->regist_func(this);
	}
	int exec(const std::string &);
};


int csl::c_plug::exec(const std::string & cmd)
{
	using namespace std;
	auto v = split(cmd);
	if (v.size() == 0)
	{
		cout << language->find(language->language(), "shell.plug.now") << ':' << endl;
	  for (auto & it:ioctrl->plugins())
			cout << it << endl;
		return 0;
	}
	int inst = 0;
	std::string syntax = "";
  for (auto & it:v)
	{
		if (findchar(it, '-'))
		{
			if (it == "-install")
			{
				if (inst != 0)
				{
					cout << language->find(language->language(),
										   "shell.unknow_parameters") << endl;
					return 0;
				}
				inst = 1;
				continue;
			}
			if (it == "-uninstall")
			{
				if (inst != 0)
				{
					cout << language->find(language->language(),
										   "shell.unknow_parameters") << endl;
					return 0;
				}
				inst = 2;
				continue;
			}
		}
		if (syntax == "")
			syntax = it;
		else
			syntax += " " + it;
	}
	if (inst == 1)
	{
		if (findchar(syntax, '/') == 0)
			syntax = shell->home + syntax;
		ioctrl->open(syntax);
		return 0;
	}
	if (inst == 2)
	{
		ioctrl->close(syntax);
		return 0;
	}
	cout << language->find(language->language(), "shell.too_many_parameters") << endl;
	return 0;
}


int csl::c_lang::exec(const std::string & cmd)
{
	using namespace std;
	auto v = split(cmd);
	if (v.size() == 0)
	{
		cout << language->find(language->language(), "shell.lang.now") << endl;
		return 0;
	}
	if (v.size() == 1 && findchar(v[0], '-') == 0)
	{
		if (language->language(v[0]))
		{
			cout << language->find(language->language(), "shell.lang.now") << endl;
			return 0;
		}
		else
		{
			cout << language->find(language->language(), "shell.lang.unknow") << endl;
			return 0;
		}
	}
	int params = 0;
	std::string param;
	bool list_support = false;
  for (auto & it:v)
	{
		if (findchar(it, '-') != 0)
		{
			if (it == "-support")
			{
				list_support = true;
			}
			else
			{
				cout << language->find(language->language(), "shell.unknow_parameters") << endl;
				return 0;
			}
		}
		else
		{
			++params;
			if (params > 1)
			{
				cout << language->find(language->language(), "shell.too_many_parameters") << endl;
				return 0;
			}
			param = it;
		}
	}
	cout << language->find(language->language(), "shell.lang.support") << ':' << endl;
  for (auto & it:language->languages())
		cout << it << endl;
	if (params > 0)
	{
		if (language->language(param))
		{
			cout << language->find(language->language(), "shell.lang.now") << endl;
			return 0;
		}
		else
		{
			cout << language->find(language->language(), "shell.lang.unknow") << endl;
			return 0;
		}
	}
}


#endif