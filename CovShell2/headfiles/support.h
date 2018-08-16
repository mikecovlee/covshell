// Covariate Shell 语言支持与帮助文件
inline string error(int lable = 0) {
  if (lang == "中文") {
    switch (lable) {
    case 0:
      return "命令语法不正确。";
    case 1:
      return "系统未找到指定目录或文件。";
    case 2:
      return "找不到 ";
    case 3:
      return "创建 ";
    case 4:
      return "删除 ";
    case 5:
      return " 成功";
    case 6:
      return " 失败";
    case 7:
      return
        "权限有限，您不能执行此操作。\n您也可以输入管理员密码以继续。";
    case 8:
      return "系统找不到指定的路径。";
    case 9:
      return "ECHO处在打开状态。";
    case 10:
      return "ECHO处在关闭状态。";
    case 11:
      return "密码错误，请重新输入。";
    case 12:
      return "环境变量 ";
    case 13:
      return " 已定义";
    case 14:
      return " 未定义";
    case 15:
      return
        " 不是已知的命令，也不是可运行的程序或批处理文件。";
    case 16:
      return "未知标签。";
    case 17:
      return "版本 ";
    case 18:
      return "按任意键继续…";
    case 19:
      return "当前时间:";
    case 20:
      return "当前日期:";
    case 21:
      return
        "\n您正在使用测试版软件\n此软件可能会对您的系统造成不可预知的破坏\n智锐科创工作室不会为您的任何损失负责。";
    case 22:
      return "请设置您的用户名，按回车键跳过:";
    case 23:
      return "请设置您的密码，按回车键跳过:";
    case 24:
      return "请设置您的主目录，按回车键跳过:";
    case 25:
      return "请输入管理员密码:";
    case 26:
      return "您当前的语言是:";
    case 27:
      return "您选择的语言类型不受支持。";
    case 28:
      return "系统找不到指定文件。";
    case 29:
      return "目录";
    case 30:
      return "文件";
    }
  }
  if (lang == "English") {
    switch (lable) {
    case 0:
      return "Command syntax is incorrect.";
    case 1:
      return "The system did not find the specified directory or file.";
    case 2:
      return "Couldn't find ";
    case 3:
      return "Set up ";
    case 4:
      return "Delete ";
    case 5:
      return " success";
    case 6:
      return " fail";
    case 7:
      return
        "Access is limited, you can't do this.\nYou can also enter the administrator password to continue.";
    case 8:
      return "The system could not find the specified path.";
    case 9:
      return "ECHO is open.";
    case 10:
      return "ECHO is close.";
    case 11:
      return "Wrong password, please enter again.";
    case 12:
      return "The environment variable ";
    case 13:
      return " is defined.";
    case 14:
      return " is undefined.";
    case 15:
      return " not known to command, not to run a program or batch file.";
    case 16:
      return "Unknown label.";
    case 17:
      return "Version ";
    case 18:
      return "Press any key to continue...";
    case 19:
      return "Now time:";
    case 20:
      return "Now date:";
    case 21:
      return
        "\nBeta software\nyou are using the software of your system\nmay cause unpredictable damage.\nT&C Studio will not be liable\nfor any loss for you.";
    case 22:
      return "Please set your user name\nor press enter to skip:";
    case 23:
      return "Please set your password\nor press enter to skip:";
    case 24:
      return "Please set your home directory\nor press the enter key to skip:";
    case 25:
      return "Please enter the administrator password:";
    case 26:
      return "Your current language is:";
    case 27:
      return "The language type you choise is not supported.";
    case 28:
      return "System can not find the specified file.";
    case 29:
      return "directory";
    case 30:
      return "file";
    }
  }
  return "os.shell.lang.error";
}
inline string help(string var) {
  return "os.shell.help.error";
}