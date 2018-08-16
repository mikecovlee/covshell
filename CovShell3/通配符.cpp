#include <iostream>
#include <ctime>
#include <list>
#include <string>
bool findexpr(const std::string & str, const std::string & rule)
{
	if (str == rule)
		return true;
	if (str.size() < rule.size())
		return false;
	  auto findchar =[&](char ch)->bool { for (auto & i:rule)
			if (i == ch)
				return true;
		return false;
	};
	if ((!findchar('?')) && (!findchar('*')))
		return false;
	if (findchar('?') && (!findchar('*')))
	{
		if (str.size() != rule.size())
			return false;
		for (int i = 0; i != str.size(); ++i)
			if ((rule[i] != '?') && (str[i] != rule[i]))
				return false;
		return true;
	}
	std::list < std::string > rules;
	std::string tmp;
	for (int i = 0; i != rule.size(); ++i)
		if (rule[i] == '*')
		{
			if (tmp != "")
			{
				rules.push_back(tmp);
				tmp = "";
			}
		}
		else
			tmp += rule[i];
	if (tmp != "")
		rules.push_back(tmp);
	bool fail = false;
	int count = 0;
  for (auto & expr:rules)
	{
		for (int k = 0; k < str.size(); ++k)
			if (str[k] == expr[0])
			{
				for (int n = 0; k < str.size() && n < expr.size(); ++n, ++k)
					if ((expr[n] != '?') && (str[k] != expr[n]))
					{
						fail = true;
						break;
					}
				if (!fail)
					++count;
				fail = false;
			}
		if (count == 0)
			return false;
	}
	return true;
}

int main()
{
	clock_t begin, end;
	begin = clock();
	findexpr("abcdefghijklmnopq", "*cdef?h?*");
	end = clock();
	std::cout << end - begin;
}