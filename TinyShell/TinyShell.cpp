  // TinyShell.cpp TinyShell主文件
  // 版本:1.2.2c
  // 命令解析器版本:TCOS Shell 1.2
  // 李登淳编写+测试+发布
  // (C) 版权所有 2000-2015 李登淳
  // Technology&Creation Studio制作
  // (C) 版权所有 2010-2015 T&C Studio
  // QQ:1624026919 @攻城狮小李
  // E-Mail:lidengchun@163.com
  // 注释什么的最烦人了ㄟ(￣▽￣ㄟ)
  // 最后修改日期:2015.2.2
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include "TinyShell.h"
using namespace std;
static bool echo = true;
static bool dev_mod = false;
static string shellver = "1.2.2c";
static string home = "/mnt/sdcard/";
static string user = "用户";
void outsettings() {
  cout << "请输入您的姓名，按回车键跳过:";
  string name;
  getline(cin, name);
  if (!findspace(name)) {
    ofstream out("/sdcard/TinyShell/settings.txt");
    user = name;
    variable::write("%user%", user);
    out << "user " + name;
  } else {
    user = "用户";
    variable::write("%user%", user);
  }
  cout << "请输入您的主目录，按回车键跳过:";
  string dir;
  getline(cin, dir);
  if (!findspace(dir)) {
    ofstream out("/sdcard/TinyShell/settings.txt");
    if (dir[dir.size() - 1] != '/') {
      dir += "/";
    }
    home = dir;
    variable::write("%home%", home);
    out << "\nhome " + dir;
  } else {
    home = "/mnt/sdcard/";
    variable::write("%home%", home);
  }
  clean();
}
void settings() {
  ifstream in("/sdcard/TinyShell/settings.txt");
  if (in) {
    string n;
    while (getline(in, n)) {
      if (findspace(n) || n == "") {
        continue;
      } else {
        vector < string > nm = split(n);
        if (nm[0] == "user") {
          n = nm[1];
          for (int i = 2; i != nm.size(); ++i) {
            n = n + " " + nm[i];
          }
          if (n == "unknow") {
            user = "用户";
            variable::write("%user%", user);
          } else {
            user = n;
            variable::write("%user%", user);
          }
          continue;
        }
        if (nm[0] == "home") {
          if ((nm[1])[nm[1].size() - 1] != '/') {
            home = nm[1] + "/";
            variable::write("%home%", home);
          } else {
            home = nm[1];
            variable::write("%home%", home);
          }
          continue;
        }
        if (nm[0] == "dev_mod") {
          dev_mod = true;
          continue;
        }
      }
      outsettings();
    }
  } else {
    outsettings();
    return;
  }
}
void getshell(vector < string > *ch, string dir) {
  ifstream input(dir);
  if (input) {
    string buff;
    while (getline(input, buff)) {
      if (findspace(buff)) {
        continue;
      }
      ch->push_back(buff);
    }
  } else {
    cerr << "系统未找到指定目录或文件。";
  }
}
int run(string & cmd) {
  getvalue(cmd);
  vector < string > v = split(cmd);
  if (findchar(v[0], '&') != 0) {
    string c;
    c = cmd.substr(0, findchar(cmd, '&') - 1);
    int r = run(c);
    if (r != 0)
      return r;
    c = cmd.substr(findchar(cmd, '&'), cmd.size() - findchar(cmd, '&'));
    return run(c);
  }
  if (v[0] == "set" || v[0] == "SET") {
    if (v.size() == 1) {
      cerr << "命令语法不正确。";
      return 0;
    }
    if (findchar(v[1], '/') == 1) {
      if (v[1] == "/p") {
        string temp0 = v[2];
        for (int l = 3; l != v.size(); ++l)
          temp0 += " " + v[l];
        if (findchar(v[2], '\"') != 0) {
          string temp = temp0.substr(findchar(v[2], '\"'),
                                     temp0.size() - findchar(temp0, '\"'));
          int i = 0;
          while (i != temp.size()) {
            if (temp[i] == '\"')
              temp.erase(i);
            else
              ++i;
          }
          cout << temp;
        }
        string input;
        getline(cin, input);
        variable::create("%" + temp0.substr(0, findchar(temp0, '=') - 1) + "%",
                         input);
        return 0;
      }
    }
    if (findchar(v[1], '=') != 0) {
      string swap = v[1].substr(findchar(v[1], '='), v[1].size());
      for (int i = 2; i != v.size(); ++i)
        swap += " " + v[i];
      variable::read("%" + swap + "%", swap);
      variable::create("%" + v[1].substr(0, findchar(v[1], '=') - 1) + "%",
                       swap);
      return 0;
    }
    string tem;
    if (variable::read("%" + v[1] + "%", tem))
      cout << v[1] << "=" << tem << endl;
    else
      cout << "环境变量 " << v[1] << " 没有定义" << endl;
    return 0;
  }
  if (v[0] == "del" || v[0] == "DEL") {
    if (v.size() == 1) {
      cerr << "命令语法不正确。";
      return 0;
    }
    string value = v[1];
    for (int i = 2; i != v.size(); ++i) {
      value = value + " " + v[i];
    }
    string directory;
    if (findchar(value, '/') == 0) {
      directory = home + value;
    } else {
      directory = value;
    }
    if (!findfile(directory)) {
      cerr << "找不到 " << directory << endl;;
      return 0;
    }
    if (remove(directory.c_str()))
      cerr << "删除 \"" + directory + "\" 失败！";
    return 0;
  }
  if (v[0] == "md" || v[0] == "mkdir" || v[0] == "MD" || v[0] == "MKDIR") {
    if (v.size() == 1) {
      cerr << "命令语法不正确。";
      return 0;
    }
    string value = v[1];
    for (int i = 2; i != v.size(); ++i) {
      value = value + " " + v[i];
    }
    string directory;
    if (findchar(value, '/') == 0) {
      directory = home + value + "/";
    } else {
      if (value[value.size() - 1] != '/') {
        directory = value + "/";
      } else {
        directory = value;
      }
    }
    if (mkdir(directory.c_str(), 0777)) {
      cerr << "未能创建目录 \"" << v[1] << "\"！";
      return 0;
    }
    return 0;
  }
  if (v[0] == "type" || v[0] == "TYPE") {
    if (v.size() == 1) {
      cerr << "命令语法不正确。";
      return 0;
    }
    string value = v[1];
    for (int i = 2; i != v.size(); ++i) {
      value = value + " " + v[i];
    }
    string directory;
    if (findchar(value, '/') == 0) {
      directory = home + value;
    } else {
      directory = value;
    }
    type(directory);
    return 0;
  }
  if (v[0] == "rd" || v[0] == "rmdir" || v[0] == "RD" || v[0] == "RMDIR") {
    if (v.size() == 1) {
      cerr << "命令语法不正确。";
      return 0;
    }
    string value = v[1];
    for (int i = 2; i != v.size(); ++i) {
      value = value + " " + v[i];
    }
    string directory;
    if (findchar(value, '/') == 0) {
      directory = home + value + "/";
    } else {
      if (value[value.size() - 1] != '/') {
        directory = value + "/";
      } else {
        directory = value;
      }
    }
    rmdir(directory);
    if (directory == home)
      settings();
    return 0;
  }
  if (v[0] == "cls" || v[0] == "CLS") {
    clean();
    return 0;
  }
  if (v[0] == "call" || v[0] == "CALL") {
    if (v.size() == 1) {
      cerr << "命令语法不正确。";
      return 0;
    }
    string value = v[1];
    for (int i = 2; i != v.size(); ++i) {
      value = value + " " + v[i];
    }
    string directory;
    if (findchar(value, '/') == 0) {
      directory = home + value;
    } else {
      directory = value;
    }
    if (!findfile(directory)) {
      cerr << "\'" << v[1] <<
        "\'不是内部或外部命令，也不是可运行的程序或批处理文件。"
        << endl;
      return 0;
    }
    vector < string > cmd;
    getshell(&cmd, directory);
    for (int i = 0; i != cmd.size(); ++i) {
      string temp = cmd[i];
      vector < string > c = split(temp);
      if (findchar(c[0], ':') == 1) {
        string k = c[0].substr(1, c[0].size());
        variable::create(k, i);
      }
    }
    for (int i = 0; i != cmd.size(); ++i) {
      string temp = cmd[i];
      getvalue(temp);
      vector < string > c = split(temp);
      if (c[0] == "goto" || c[0] == "GOTO") {
        if (c.size() == 1) {
          cerr << "未知标签！";
          continue;
        }
        if (variable::read(c[1], i)) {
          continue;
        } else {
          cerr << "未知标签！";
          continue;
        }
      }
      if (findchar(c[0], ':') != 0)
        continue;
      int r = run(temp);
      if (r == 3) {
        return 3;
      }
      if (findchar(c[0], '@') == 0) {
        if (echo)
          cout << "\n" + home + ">";
        else
          cout << endl;
      }
    }
    return 2;
  }
  if (v[0] == "rem" || v[0] == "REM" || v[0] == "goto" || v[0] == "GOTO") {
    return 0;
  }
  if (v[0] == "cd" || v[0] == "CD" || v[0] == "chdir" || v[0] == "CHDIR") {
    if (v.size() == 1) {
      cerr << "命令语法不正确。";
      return 0;
    }
    if (v[1] != "." && v[1] != "..") {
      string directory;
      if (findchar(v[1], '/') == 0) {
        directory = home + v[1] + "/";
      } else {
        if ((v[1])[v[1].size() - 1] != '/') {
          directory = v[1] + "/";
        } else {
          directory = v[1];
        }
      }
      if (!opendir(directory.c_str())) {
        cerr << "系统找不到指定的路径。";
        return 0;
      }
      home = directory;
    } else if (v[1] == "..") {
      if (home.size() != 1)
        home.erase(home.size() - 1);
      for (int k = home.size() - 1; home[k] != '/'; --k) {
        home.erase(k);
      }
    }
    return 0;
  }
  if (v[0] == "ver" || v[0] == "VER") {
    cout << "\nTinyShell [版本 " + shellver + "]\n";
    return 0;
  }
  if (v[0] == "cmd" || v[0] == "CMD") {
    echo = true;
    return 1;
  }
  if (v[0] == "echo" || v[0] == "ECHO") {
    if (v.size() > 1) {
      if (v[1] == "on") {
        echo = true;
        cerr << "ECHO处于打开状态";
        return 0;
      }
      if (v[1] == "off") {
        echo = false;
        cerr << "ECHO处于关闭状态";
        return 0;
      }
      string str;
      str = v[1];
      for (int i = 2; i != v.size(); ++i)
        str += " " + v[i];
      cout << str;
      return 0;
    }
    if (echo) {
      cerr << "ECHO处于打开状态";
      return 0;
    } else {
      cerr << "ECHO处于关闭状态";
      return 0;
    }
    return 0;
  }
  if (v[0] == "@echo" || v[0] == "@ECHO") {
    if (v.size() > 1) {
      if (v[1] == "on") {
        echo = true;
        return 0;
      }
      if (v[1] == "off") {
        echo = false;
        return 0;
      }
      string str = v[1];
      for (int i = 2; i != v.size(); ++i)
        str += " " + v[i];
      cout << str;
      return 0;
    }
  }
  if (v[0] == "echo." || v[0] == "ECHO.") {
    cout << endl;
    return 0;
  }
  if (v[0] == "time" || v[0] == "TIME") {
    cout << "当前时间：";
    time();
    cout << endl;
    return 0;
  }
  if (v[0] == "date" || v[0] == "DATE") {
    cout << "当前日期：";
    date();
    cout << endl;
    return 0;
  }
  if (v[0] == "dir" || v[0] == "DIR") {
    if (v.size() > 1) {
      string directory;
      if (v[1] != "." && v[1] != "..") {
        if (findchar(v[1], '/') == 0) {
          directory = home + v[1] + "/";
        } else {
          if ((v[1])[v[1].size() - 1] != '/') {
            directory = v[1] + "/";
          } else {
            directory = v[1];
          }
        }
        if (!opendir(directory.c_str())) {
          cerr << "系统找不到指定的路径。";
          return 0;
        }
      } else if (v[1] == "..") {
        directory = home;
        if (directory.size() != 1)
          directory.erase(directory.size() - 1);
        for (int k = directory.size() - 1; directory[k] != '/'; --k) {
          directory.erase(k);
        }
      }
      dir(directory);
      return 0;
    }
    dir(home);
    return 0;
  }
  if (v[0] == "pause" || v[0] == "PAUSE") {
    if (v.size() > 1) {
      if (v[1] == ">nul" || v[1] == ">NUL") {
        cpause();
        return 0;
      }
    }
    cout << "按任意键继续…";
    cpause();
    return 0;
  }
  if (v[0] == "exit" || v[0] == "EXIT") {
    return 3;
  }
  if (v[0] == "reset" || v[0] == "RESET") {
    ifstream del_user("/sdcard/TinyShell/settings.txt");
    if (del_user) {
      remove("/sdcard/TinyShell/settings.txt");
    }
    clean();
    cout << "重置成功。\n";
    return 1;
  }
  cerr << "\'" + v[0] +
    "\'不是内部或外部命令，也不是可运行的程序或批处理文件。"
    << endl;
  return 2;
}
void runshell() {
  settings();
  if (dev_mod) {
    string c = "call /sdcard/TinyShell/run.bat";
    int r = run(c);
    if (r == 3) {
      exit(0);
    }
    if (r == 2) {
      echo = true;
    }
    if (r == 1) {
      dev_mod = false;
      echo = true;
      runshell();
    }
    dev_mod = false;
  }
  clean();
  cout << "TinyShell [版本" + shellver +
    "]\n(C) 版权所有 2010-2015 S.W-T&C Studio" << endl;
  while (true) {
    if (echo)
      cout << "\n" + home + ">";
    else
      cout << endl;
    string c;
    getline(cin, c);
    if (findspace(c) || c == "") {
      continue;
    }
    int r = run(c);
    if (r == 3) {
      exit(0);
    }
    if (r == 2)
      echo = true;
    if (r == 1) {
      echo = true;
      runshell();
    }
  }
}
int main(int argc, char **argv) {
  clean();
  variable::create("%user%", "用户");
  variable::create("%home%", "/mnt/sdcard/");
  if (argc > 1) {
    string c = argv[1];
    if (argc > 2) {
      for (int i = 2; i != argc; ++i) {
        c = c + " " + argv[i];
      }
    }
    int r = run(c);
    if (r == 3) {
      exit(0);
    }
    if (r == 1) {
      runshell();
    }
  }
  if (!opendir("/sdcard/TinyShell/"))
    mkdir("/sdcard/TinyShell/", 0777);
  echo = true;
  runshell();
  return 0;
}