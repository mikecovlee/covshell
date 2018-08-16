#ifndef GUARD_TinyShell_h
#define GUARD_TinyShell_h
  // TinyShell 头文件 1.2.2c
  // 版权所有 2015 © T&C Studio
  // 李登淳编写
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<map>
#include<algorithm>
#include<time.h>
#include<stdio.h>
#include<termios.h>
#include<dirent.h>
#include<sys/stat.h>
int findchar(const std::string chars, char ch) {
  for (int i = 0; i != chars.size(); ++i) {
    if (chars[i] == ch) {
      return i + 1;
    }
  }
  return 0;
}
bool findfile(std::string dir) {
  std::ifstream in(dir);
  if (in) {
    return true;
  } else {
    return false;
  }
}
void type(const std::string dir) {
  std::ifstream in(dir.c_str());
  std::string temp;
  if (!findfile(dir))
    std::cerr << "系统找不到指定文件。" << std::endl;
  else {
    while (!in.eof()) {
      getline(in, temp);
      std::cout << temp << std::endl;
    }
  }
}
bool findspace(const std::string & chars) {
  typedef std::string::size_type string_size;
  for (string_size i; i != chars.size(); ++i) {
    if (!std::isspace(chars[i])) {
      return false;
    }
  }
  return true;
}
namespace variable {
  static std::map < std::string, int >vary_int;
  static std::map < std::string, std::string > vary;
  void create(std::string vary_name, std::string vary_value) {
    vary[vary_name] = vary_value;
    return;
  } void create(std::string vary_name, int vary_value) {
    vary_int[vary_name] = vary_value;
    return;
  }
  bool read(std::string vary_name, std::string & return_value) {
    if (vary.find(vary_name) == vary.end())
      return false;
    return_value = vary.find(vary_name)->second;
    return true;
  }
  bool read(std::string vary_name, int &return_value) {
    if (vary_int.find(vary_name) == vary_int.end())
      return false;
    return_value = vary_int.find(vary_name)->second;
    return true;
  }
  bool write(std::string vary_name, std::string vary_value) {
    if (vary.find(vary_name) == vary.end())
      return false;
    vary[vary_name] = vary_value;
    return true;
  }
  bool write(std::string vary_name, int vary_value) {
    if (vary_int.find(vary_name) == vary_int.end())
      return false;
    vary_int[vary_name] = vary_value;
    return true;
  }
}
void getvalue(std::string & str) {
  std::string tmp;
  int first = 0;
  int last = 0;
  int i = 0;
for (const char &ch:str)
    if (ch == '%')
      i++;
  i /= 2;
  while (i--) {
    first = str.find('%', last);
    last = str.find('%', first + 1) + 1;
    if (variable::read(str.substr(first, last - first), tmp))
      str.replace(first, last - first, tmp);
  }
}
void date() {
  static const std::string months[] =
    { "1月", "2月", "3月", "4月", "5月", "6月", "7月", "8月",
    "9月", "10月", "11月", "12月"
  };
  static const std::string weekdays[] =
    { "星期日", "星期一", "星期二", "星期三", "星期四",
    "星期五",
    "星期六"
  };
  time_t t;
  time(&t);
  tm *tn = localtime(&t);
  std::cout << tn->tm_year +
    1900 << "年" << months[tn->tm_mon] << tn->
    tm_mday << "日 " << weekdays[tn->tm_wday];
}
void time() {
  time_t t;
  time(&t);
  tm *tn = localtime(&t);
  std::cout << ((tn->tm_hour < 10) ? "0" : "") << tn->tm_hour << ':' <<
    ((tn->tm_min < 10) ? "0" : "") << tn->tm_min << ':' << ((tn->tm_sec <
                                                             10) ? "0" : "") <<
    tn->tm_sec;
}
std::vector < std::string > split(const std::string & s) {
  std::vector < std::string > input;
  typedef std::string::size_type string_size;
  string_size i = 0;
  while (i != s.size()) {
    while (i != s.size() && std::isspace(s[i]))
      ++i;
    string_size j = i;
    while (j != s.size() && !std::isspace(s[j]))
      ++j;
    if (i != j) {
      input.push_back(s.substr(i, j - i));
      i = j;
    }
  }
  return input;
}
void dir(const std::string & d) {
  dirent **dstr;
  if (!opendir(d.c_str())) {
    std::cerr << "系统找不到指定目录。";
    return;
  }
  int n = scandir(d.c_str(), &dstr, 0, alphasort);
  std::vector < std::string > files;
  for (int i = 0; i < n; i++) {
    files.push_back(dstr[i]->d_name);
    free(dstr[i]);
  }
  std::string::size_type maxlen = 0;
  for (int i = 0; i != files.size(); ++i)
    maxlen = std::max(maxlen, files[i].size());
  int dirs = 0;
  int ffiles = 0;
  std::cout << " 目录 " + d << std::endl;
  for (int i = 0; i != files.size(); ++i) {
    std::string t = d + files[i];
    std::string space(maxlen + 7 - files[i].size(), ' ');
    std::string space0(maxlen + 2 - files[i].size(), ' ');
    if (opendir(t.c_str())) {
      std::cout << files[i] + space + "<DIR>" << std::endl;
      ++dirs;
      continue;
    } else if (findfile(t)) {
      int point = files[i].size();
      while (point != 0) {
        if (files[i][point] == '.')
          break;
        --point;
      }
      std::string ttem =
        files[i].substr(point + 1, files[i].size() - point - 1);
      std::cout << files[i] + space0 + ttem << std::endl;
      ++ffiles;
      continue;
    }
    std::cout << files[i] + space0 + "----" << std::endl;;
  }
  std::string space1(maxlen + 2, ' ');
  std::cout << std::endl;
  std::cout << space1 << ffiles << " 个文件" << std::endl;
  std::cout << space1 << dirs << " 个目录" << std::endl;
}
void clean() {
  printf("\x1B[2J\x1B[0;0f");
}
void cpause() {
  termios newt;
  tcgetattr(0, &newt);
  newt.c_lflag &= ~ICANON;
  newt.c_lflag &= ~ECHO;
  tcsetattr(0, TCSANOW, &newt);
  std::cin.get();
  newt.c_lflag |= ICANON;
  newt.c_lflag |= ECHO;
  tcsetattr(0, TCSANOW, &newt);
}
void rmdir(const std::string & d) {
  dirent **dstr;
  if (!opendir(d.c_str())) {
    std::cerr << "系统找不到指定路径。";
    return;
  }
  int n = scandir(d.c_str(), &dstr, 0, alphasort);
  std::string file;
  while (n-- != 2) {
    file += d + "/" + dstr[n]->d_name;
    if (remove(file.c_str()))
      rmdir(file);
    file.erase(file.begin(), file.end());
    free(dstr[n]);
  }
  free(dstr);
  if (remove(d.c_str()))
    std::cerr << "未知错误";;
}
#endif