#include "Plugin.h"


namespace csl
{
	class c_clear;
}


class csl::c_clear:public baseShellFunc
{
  public:
	c_clear()
	{
		m_name = "clear";
	}
	int exec(const std::string &);
};


static csl::c_clear clear;


void csl::plugin::install()
{
	csl::this_plugin.name("CSL_CLEAR");
	csl::shell->regist_func(&clear);
}


void csl::plugin::uninstall()
{
	csl::shell->remove_func(&clear);
}


int csl::c_clear::exec(const std::string & cmd)
{
	ioctrl->clrscr();
	return 0;
}
