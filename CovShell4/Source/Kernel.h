#ifndef COV_SHELL_KERNEL_H


#define COV_SHELL_KERNEL_H


#include <list>
#include <map>
#include <vector>
#include <string>
#include <cstdlib>
#include <dlfcn.h>
#include <fstream>
#include <iostream>
#include <termios.h>


namespace csl
{
	const char *version = "4.0.0.1200";
	inline std::vector < std::string > split(const std::string &);
	inline int findchar(const std::string &, char);
	class baseShellFunc;
	class baseShellPlug;
	class basic_shell;
	class basic_lang;
	class basic_io;
}


class csl::baseShellFunc
{
  protected:
	std::string m_name;
  public:
	baseShellFunc();
	baseShellFunc(const baseShellFunc &) = delete;
	virtual ~ baseShellFunc();
	virtual int exec(const std::string & cmd)
	{
	}
	virtual const std::string & name()
	{
		return m_name;
	}
};


class csl::baseShellPlug
{
  protected:
	std::string m_name;
	void (*plug_construct) (basic_lang *, basic_shell *, basic_io *);
	void (*plug_destory) ();
  public:
	baseShellPlug():m_name("Covaraint_Shell_Plugin")
	{
		plug_construct = 0;
		plug_destory = 0;
	}
	baseShellPlug(const baseShellPlug &) = delete;
	virtual ~ baseShellPlug()
	{
	}
	virtual void construct(void (*func) (basic_lang *, basic_shell *, basic_io *))
	{
		plug_construct = func;
	}
	virtual void construct(basic_lang * lang, basic_shell * sh, basic_io * io)
	{
		if (plug_construct)
			plug_construct(lang, sh, io);
	}
	virtual void destory(void (*func) ())
	{
		plug_destory = func;
	}
	virtual void destory()
	{
		if (plug_destory)
			plug_destory();
	}
	virtual void name(const std::string & in)
	{
		m_name = in;
	}
	virtual const std::string & name()
	{
		return m_name;
	}
};


class csl::basic_shell
{
  protected:
	bool m_echo = true;
	std::list < baseShellFunc * >func_list;
  public:
#if not defined(__ANDROID__) || defined(ANDROID)
	std::string shelldir = getenv("HOME") + std::string("/.covsh/");
	std::string home = getenv("HOME") + std::string("/");
#else
	std::string shelldir = getenv("HOME") + std::string("/.covsh/");
	std::string home = "/mnt/sdcard/";
#endif
	bool echo()
	{
		return m_echo;
	}
	void echo(bool in)
	{
		m_echo = in;
	}
	baseShellFunc *search_func(const std::string & name)
	{
	  for (auto & it:func_list)
		{
			if (it->name() == name)
				return it;
		}
		return 0;
	}
	void regist_func(baseShellFunc * it)
	{
		func_list.push_back(it);
	}
	void remove_func(baseShellFunc * it)
	{
		func_list.remove(it);
	}
	int exec(const std::string &);
	int run(int, char **);
};


class csl::basic_lang
{
  protected:
	std::string m_lang;
	std::string config_direct = getenv("HOME") + std::string("/language/");
	std::ifstream inflow;
	std::list < std::string > langs;
	std::map < std::string, std::map < std::string, std::string >> data_base;
	void entry_db();
  public:
	basic_lang() = default;
	basic_lang(const std::string & dir):config_direct(dir)
	{
	}
	void init()
	{
		entry_db();
		if (!langs.empty())
			m_lang = langs.front();
	}
	bool language(const std::string & in)
	{
		bool find = false;
	  for (auto & it:langs)
			if (it == in)
				find = true;
		if (find)
			m_lang = in;
		return find;
	}
	const std::string & language()
	{
		return m_lang;
	}
	const std::list < std::string > &languages()
	{
		return langs;
	}
	std::string find(const std::string &, const std::string &);
};


class csl::basic_io
{
  protected:
	std::string config_direct = getenv("HOME") + std::string("/plugin/");
	std::ifstream inflow;
	std::list < std::string > m_plugins;
	std::map < baseShellPlug *, void *>plug_bundle;
  public:
	basic_io() = default;
	basic_io(const std::string & dir):config_direct(dir)
	{
	}
	~basic_io();
	bool open(const std::string &);
	bool close(const std::string &);
	const std::list < std::string > &plugins()
	{
		return m_plugins;
	}
	inline void clrscr();
	inline void pause();
	inline void echo(bool);
	void init();
};


#endif
