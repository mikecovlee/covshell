#ifndef COV_SHELL_PLUGIN_H


#define COV_SHELL_PLUGIN_H


#include "Kernel.cpp"


namespace csl
{
	static basic_lang *language = 0;
	static basic_shell *shell = 0;
	static basic_io *ioctrl = 0;
	static baseShellPlug this_plugin;
	namespace plugin
	{
		void install();
		void uninstall();
		namespace linker
		{
			void construct(basic_lang *, basic_shell *, basic_io *);
			void destory();
		}
	}
}


csl::baseShellFunc::baseShellFunc():m_name("Basic_Shell_Function")
{
}


csl::baseShellFunc::~baseShellFunc()
{
}


void csl::plugin::linker::construct(basic_lang * lang, basic_shell * sh, basic_io * io)
{
	language = lang;
	shell = sh;
	ioctrl = io;
	install();
}


void csl::plugin::linker::destory()
{
	uninstall();
	language = 0;
	shell = 0;
	ioctrl = 0;
}


extern "C"
{
	csl::baseShellPlug * entry()
	{
		csl::this_plugin.construct(csl::plugin::linker::construct);
		csl::this_plugin.destory(csl::plugin::linker::destory);
		return &csl::this_plugin;
	}
}


#endif