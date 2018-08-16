  // Shell.cpp Covarite Shell主文件
  // 版本:Covariate Shell 1.0
  // 李登淳编写+测试+发布+语言翻译
  // (C) 版权所有 2000-2015 李登淳
  // Technology&Creation Studio制作
  // (C) 版权所有 2010-2015 T&C Studio
  // QQ:1624026919 @攻城狮小李
  // E-Mail:lidengchun@163.com
  // 注释什么的最烦人了ㄟ(￣▽￣ㄟ)
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<stdlib.h>
using namespace std;
// 初始化
// 在G++参数中加入-Dandroid开启Android模式
// 非Android系统请勿开启Android模式
#ifndef android
static string shelldir = "/root/.tcstudio/";
static string home = "/root/";
#else
static string shelldir = "/mnt/sdcard/.tcstudio/";
static string home = "/mnt/sdcard/";
#endif
static bool iecho = true;
static bool su = false;
static bool dev_mod = false;
static string user = "User";
static string initial_home = home;
static string icrypt;
// 以上数据请勿更改，会导致程序产生错误
// 开启开发者模式请在settings.txt中配置
// 语言(English或中文)
static string lang = "中文";
// 此语句请勿更改
static string initial_lang = lang;
// 测试版信息显示设置，true开启，false关闭
static bool beta = false;
// 文本刷新率设置，单位微秒
static size_t irefresh = 11111;
// 显示的Build版本设置
static string buildver = "20150330 3000";
// 显示的Shell版本设置
static string shellver = "1.0";
// 其他源文件
#include"language_support&help.cpp"
#include"shell.h"
// 独立函数体
inline void outsettings() {
  ofstream out(shelldir + "settings.txt");
  cout << error(22);
  string name;
  getline(cin, name);
  if (!findspace(name)) {
    user = name;
  } else {
    user = "User";
  }
  variable::write("%user%", user);
  out << "user " + user;
  cout << error(23);
  string psw;
  getline(cin, psw);
  if (!findspace(psw) && !psw.empty()) {
    ofstream sout(shelldir + ".encrypt/usercrypt.key");
    icrypt = psw;
    sout << psw;
    out << "\nencrypt";
    su = false;
  } else {
    out << "\ndecrypt";
    icrypt = "";
    su = true;
  }
  cout << error(24);
  string dir;
  getline(cin, dir);
  if (!findspace(dir)) {
    if (dir[dir.size() - 1] != '/') {
      dir += "/";
    }
    if (dir[0] != '/') {
      dir = "/" + dir;
    }
    home = dir;
  } else {
    home = initial_home;
  }
  if (!opendir(home.c_str()))
    mkdir(home.c_str(), 0777);
  variable::write("%home%", home);
  out << "\nhome " + home << "\nlang " + lang;
  clean();
}
inline void settings() {
  try {
    ifstream in(shelldir + "settings.txt");
    if (in) {
      string n;
      while (getline(in, n)) {
        if (findspace(n) || n == "") {
          continue;
        } else {
          vector < string > nm = split(n);
          if (nm[0] == "user") {
            if (nm.size() == 1)
              throw domain_error("Error: \"user\" have no parameters.");
            n = nm[1];
            for (int i = 2; i != nm.size(); ++i) {
              n = n + " " + nm[i];
            }
            user = n;
            variable::write("%user%", user);
            continue;
          }
          if (nm[0] == "encrypt") {
            string temp_crypt;
            ifstream incrypt(shelldir + ".encrypt/usercrypt.key");
            getline(incrypt, temp_crypt);
            icrypt = temp_crypt;
            su = false;
            continue;
          }
          if (nm[0] == "decrypt") {
            su = true;
            continue;
          }
          if (nm[0] == "home") {
            if (nm.size() == 1)
              throw domain_error("Error: \"home\" have no parameters.");
            if ((nm[1])[nm[1].size() - 1] != '/') {
              home = nm[1] + "/";
              variable::write("%home%", home);
            } else {
              home = nm[1];
              variable::write("%home%", home);
            }
            continue;
          }
          if (nm[0] == "lang") {
            if (nm.size() == 1)
              throw domain_error("Error: \"lang\" have no parameters.");
            lang = nm[1];
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
  catch(domain_error e) {
    cout << e.what() << endl << "Please check \"settings.txt\"." << endl <<
      "Press any key to exit...";
    cpause();
    exit(0);
  }
}
inline string m_if(vector < string > v) {
  if (v.size() == 1)
    return "os.shell.m_if.error";
  bool m_not;
  bool m_true;
  int cmd_place;
  if (v[1] == "not")
    m_not = true;
  else
    m_not = false;
  int k = 0;
  if (m_not)
    k = 1;
  if (k + 1 == v.size())
    return "os.shell.m_if.error";
  if (findchar(v[1 + k], '=') != 0) {
    if (v[1 + k].substr(0, findchar(v[1 + k], '=') - 1) ==
        v[1 + k].substr(findchar(v[1 + k], '='),
                        v[1 + k].size() - findchar(v[1 + k], '=')))
      m_true = true;
    else
      m_true = false;
    cmd_place = 1 + k;
  }
  if (k + 2 == v.size())
    return "os.shell.m_if.error";
  if (v[1 + k] == "exist") {
    string value = v[2 + k];
    string directory;
    if (findchar(value, '/') == 0) {
      directory = home + value;
    } else {
      directory = value;
    }
    if (findfile(directory))
      m_true = true;
    else
      m_true = false;
    cmd_place = 2 + k;
  }
  if (m_not && !m_true)
    goto run;
  if (!m_not && m_true)
    goto run;
  return "os.shell.m_if.false";
run:
  if (cmd_place + 1 == v.size())
    return "os.shell.m_if.error";
  string tmp = v[cmd_place + 1];
  for (int i = cmd_place + 2; i != v.size(); ++i)
    tmp = tmp + " " + v[i];
  return tmp;
}
inline void outecho() {
  if (icrypt.empty())
    su = true;
  if (iecho) {
    if (su)
      cout << "#:" + home + ">";
    else
      cout << "$:" + home + ">";
  } else
    cout << endl;
}
inline void getshell(vector < string > *ch, string dir) {
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
    cerr << error(1) << endl;
  }
}
inline int run(string cmd) {
  // 初始化
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
  if (v.size() == 1
      && cmd.substr(findchar(cmd, '.'),
                    cmd.size() - findchar(cmd, '.')) == "tsh")
    return run("run " + cmd);
  if (v[0] == "if" || v[0] == "IF") {
    string temp = m_if(v);
    if (temp != "os.shell.m_if.false" && temp != "os.shell.m_if.error")
      return run(temp);
    if (temp == "os.shell.m_if.error")
      cout << error() << endl;
    return 0;
  }
  // 目录操作
  if (su) {
    if (v[0] == "del" || v[0] == "DEL") {
      if (v.size() == 1) {
        cerr << error() << endl;
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
      if (!opendir(directory.c_str())) {
        if (findchar(value, '/') == 0) {
          directory = home + value;
        } else {
          directory = value;
        }
        if (!findfile(directory)) {
          cerr << error(2) << directory << endl;
          return 0;
        }
      } else if (directory == home)
        settings();
      if (remove(directory.c_str()))
        cerr << error(4) + "\"" + directory + "\"" + error(6) << endl;
      return 0;
    }
    if (v[0] == "md" || v[0] == "MD") {
      if (v.size() == 1) {
        cerr << error() << endl;
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
        cerr << error(3) + "\"" + v[1] + "\"" + error(6) << endl;;
        return 0;
      }
      return 0;
    }
  } else if (v[0] == "del" || v[0] == "DEL" || v[0] == "md" || v[0] == "MD") {
    cout << error(7) << endl;
    int r = run("ctrl");
    if (r == 0)
      run(cmd);
    su = false;
    return r;
  }
  if (v[0] == "cd" || v[0] == "CD") {
    if (v.size() == 1) {
      cerr << error() << endl;
      return 0;
    }
    if (v[1] != "." && v[1] != "..") {
      string directory;
      string tmp_dir = v[1];
      for (int i = 2; i != v.size(); ++i)
        tmp_dir = tmp_dir + " " + v[i];
      if (findchar(tmp_dir, '/') == 0) {
        directory = home + tmp_dir + "/";
      } else {
        if (tmp_dir[tmp_dir.size() - 1] != '/') {
          directory = tmp_dir + "/";
        } else {
          directory = tmp_dir;
        }
      }
      if (!opendir(directory.c_str())) {
        cerr << error(8) << endl;
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
  if (v[0] == "list" || v[0] == "LIST") {
    if (v.size() > 1) {
      string directory;
      if (v[1] != "." && v[1] != "..") {
        string tmp_dir = v[1];
        for (int i = 2; i != v.size(); ++i)
          tmp_dir = tmp_dir + " " + v[i];
        if (findchar(tmp_dir, '/') == 0) {
          directory = home + tmp_dir + "/";
        } else {
          if (tmp_dir[tmp_dir.size() - 1] != '/') {
            directory = tmp_dir + "/";
          } else {
            directory = tmp_dir;
          }
        }
        if (!opendir(directory.c_str())) {
          cerr << error(8) << endl;
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
  // echo功能
  if (v[0] == "echo" || v[0] == "ECHO") {
    if (v.size() > 1) {
      if (v[1] == "on") {
        iecho = true;
        cerr << error(9) << endl;
        return 0;
      }
      if (v[1] == "off") {
        iecho = false;
        cerr << error(10) << endl;
        return 0;
      }
      string str;
      str = v[1];
      for (int i = 2; i != v.size(); ++i)
        str += " " + v[i];
      cout << str << endl;
      return 0;
    }
    if (iecho) {
      cerr << error(9) << endl;
      return 0;
    } else {
      cerr << error(10) << endl;
      return 0;
    }
    return 0;
  }
  if (v[0] == "*echo" || v[0] == "*ECHO") {
    if (v.size() > 1) {
      if (v[1] == "on") {
        iecho = true;
        return 0;
      }
      if (v[1] == "off") {
        iecho = false;
        return 0;
      }
      string str = v[1];
      for (int i = 2; i != v.size(); ++i)
        str += " " + v[i];
      cout << str << endl;
      return 0;
    }
  }
  if (v[0] == "echo." || v[0] == "ECHO.") {
    cout << endl;
    return 0;
  }
  // 中心功能
  if (!su) {
    if (v[0] == "ctrl" || v[0] == "CTRL") {
      string password;
      if (v.size() < 2) {
        cout << error(25);
        getline(cin, password);
      } else
        password = v[1];
      if (password == icrypt)
        su = true;
      else if (password.empty() || findchar(password, ' ') != 0)
        return 4;
      else {
        cout << error(11) << endl;
        run("ctrl");
      }
      return 0;
    }
  } else if (v[0] == "ctrl" || v[0] == "CTRL") {
    if (icrypt.empty()) {
      ofstream out(shelldir + "settings.txt");
      string psw;
      if (v.size() < 2) {
        cout << error(23);
        getline(cin, psw);
      } else
        psw = v[1];
      out << "user " + user;
      if (!findspace(psw) && !psw.empty()) {
        ofstream sout(shelldir + ".encrypt/usercrypt.key");
        icrypt = psw;
        sout << psw;
        out << "\nencrypt";
        su = false;
      } else {
        out << "\ndecrypt";
        icrypt = "";
        su = true;
      }
      out << "\nhome " + initial_home;
      return 0;
    }
    if (v.size() > 1) {
      icrypt = "";
      run(cmd);
      return 0;
    }
    su = false;
    return 0;
  }
  if (v[0] == "help" || v[0] == "HELP") {
    if (v.size() > 1)
      cout << help(v[1]) << endl;
    else
      cout << help("all") << endl;
    return 0;
  }
  if (v[0] == "set" || v[0] == "SET") {
    if (v.size() == 1) {
      cerr << error() << endl;
      return 0;
    }
    if (findchar(v[1], '-') == 1) {
      if (v[1] == "-input") {
        string temp0 = v[2];
        for (int l = 3; l != v.size(); ++l)
          temp0 += " " + v[l];
        if (findchar(v[2], '-') != 0) {
          string temp = temp0.substr(findchar(v[2], '-'),
                                     temp0.size() - findchar(temp0, '-'));
          int i = 0;
          while (i != temp.size()) {
            if (temp[i] == '-')
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
      cout << error(12) << v[1] << error(14) << endl;
    return 0;
  }
  if (v[0] == "run" || v[0] == "RUN") {
    if (v.size() == 1) {
      cerr << error() << endl;
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
      cout << "\'" + v[1] + "\'" + error(15) << endl;
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
    outecho();
    for (int i = 0; i != cmd.size(); ++i) {
      usleep(irefresh);
      string temp = cmd[i];
      getvalue(temp);
      vector < string > c = split(temp);
      if (c[0] == "if" || c[0] == "IF") {
        string tmp = m_if(c);
        if (tmp != "os.shell.m_if.false" && tmp != "os.shell.m_if.error")
          temp = tmp;
        if (tmp == "os.shell.m_if.error")
          cout << error() << endl;
      }
      c = split(temp);
      if (c[0] == "goto" || c[0] == "GOTO") {
        if (c.size() == 1) {
          cerr << error(16) << endl;
          continue;
        }
        if (variable::read(c[1], i)) {
          continue;
        } else {
          cerr << error(16) << endl;
          continue;
        }
      }
      if (findchar(c[0], ':') != 0)
        continue;
      if (iecho)
        cout << cmd[i] << endl;
      int r = run(temp);
      if (r != 0) {
        return r;
      }
      if (findchar(c[0], '*') == 0 && i != cmd.size() - 1) {
        outecho();
      }
    }
    return 2;
  }
  if (v[0] == "type" || v[0] == "TYPE") {
    if (v.size() == 1) {
      cerr << error() << endl;
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
  // 命令行操作
  if (v[0] == "lang" || v[0] == "LANG") {
    if (v.size() == 1) {
      cerr << error(26) + lang << endl;
      return 0;
    }
    string tmp = lang;
    lang = v[1];
    bool outech;
    if (v.size() > 2 && v[2] == "-null")
      outech = false;
    else
      outech = true;
    if (error() == "os.shell.lang.error") {
      lang = tmp;
      cout << error(27) << endl;
    } else {
      ifstream l_in(shelldir + "settings.txt");
      if (l_in) {
        string tmp;
        vector < string > swap;
        while (getline(l_in, tmp)) {
          vector < string > m_tmp = split(tmp);
          if (m_tmp[0] != "lang")
            swap.push_back(tmp);
        }
        swap.push_back("lang " + lang);
        ofstream l_out(shelldir + "settings.txt");
        if (l_out) {
          for (int n = 0; n != swap.size(); ++n)
            l_out << swap[n] << "\n";
          if (outech) {
            clean();
            run("lang");
          }
          return 0;
        }
      }
      lang = initial_lang;
    }
    return 0;
  }
  if (v[0] == "ver" || v[0] == "VER") {
    cout << "\nCovariate Shell [" + error(17) + shellver + "]";
    if (v.size() > 1 && v[1] == "-build") {
      cout << endl << "Build " + buildver;
    }
    cout << endl << endl;
    return 0;
  }
  if (v[0] == "shell" || v[0] == "SHELL") {
    if (v.size() > 1) {
      if (v[1] == "-bash") {
        system("sh");
      }
      return 0;
    }
    iecho = true;
    return 1;
  }
  if (v[0] == "pause" || v[0] == "PAUSE") {
    if (v.size() > 1) {
      if (v[1] == "-nul" || v[1] == "-NUL") {
        cpause();
        return 0;
      }
    }
    cout << error(18);
    cpause();
    cout << endl;
    return 0;
  }
  if (v[0] == "clean" || v[0] == "CLEAN") {
    clean();
    return 0;
  }
  if (v[0] == "time" || v[0] == "TIME") {
    cout << error(19);
    time();
    cout << endl;
    return 0;
  }
  if (v[0] == "date" || v[0] == "DATE") {
    cout << error(20);
    date();
    cout << endl;
    return 0;
  }
  if (v[0] == "exit" || v[0] == "EXIT") {
    return 3;
  }
  if (v[0] == "reset" || v[0] == "RESET") {
    if (su) {
      if (findfile(shelldir + "settings.txt")) {
        string del_temp = shelldir + "settings.txt";
        remove(del_temp.c_str());
      }
      if (findfile(shelldir + ".encrypt/usercrypt.key")) {
        string del_temp = shelldir + ".encrypt/usercrypt.key";
        remove(del_temp.c_str());
      }
      clean();
      return 1;
    } else {
      cout << error(7) << endl;
      int r = run("ctrl");
      if (r == 0) {
        int re = run(cmd);
        su = false;
        return re;
      }
      return r;
    }
  }
  // 错误处理
  if (v[0] == "rem" || v[0] == "REM" || v[0] == "goto" || v[0] == "GOTO")
    return 0;
  cerr << "\'" + v[0] + "\'" + error(15) << endl;
  return 2;
}
inline void runshell() {
  settings();
  if (error() == "os.shell.lang.error") {
    cout << "The type of unknown language \"" + lang +
      "\"\nPlease check \"settings.txt\".";
    exit(0);
  }
  if (dev_mod) {
    string c = "run " + shelldir + "run.tsh";
    int r = run(c);
    if (r == 3) {
      exit(0);
    }
    if (r == 2) {
      iecho = true;
    }
    if (r == 1) {
      dev_mod = false;
      iecho = true;
      runshell();
    }
    dev_mod = false;
  }
  cout << "Covariate Shell [" + error(17) + shellver + "]" << endl;
  if (beta) {
    cout << "Build " + buildver + error(21) << endl;
  }
  cout << "(C) Copyright 2010-2015 S.W-T&C Studio" << endl;
  while (true) {
    outecho();
    usleep(irefresh);
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
      iecho = true;
    if (r == 1) {
      iecho = true;
      runshell();
    }
  }
}
int main(int argc, char **argv) {
  clean();
  variable::create("%user%", "User");
  variable::create("%home%", initial_home);
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
  if (!opendir(shelldir.c_str()))
    mkdir(shelldir.c_str(), 0777);
  string ttem = shelldir + ".encrypt/";
  if (!opendir(ttem.c_str()))
    mkdir(ttem.c_str(), 0777);
  iecho = true;
  runshell();
  return 0;
}