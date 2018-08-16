#ifndef GUARD_Variable_h
#define GUARD_Variable_h
  // Variable.h By 李登淳
  // (C) Copyright 2000-2015 李登淳
#include<map>
#include<string>
  // Using namespace var
namespace variable {
  // Reload variable
  static std::map < std::string, int >vary_int;
  /*
  static std::map < std::string, double >vary_double;
  static std::map < std::string, bool > vary_bool;
  static std::map < std::string, char >vary_char;*/
  static std::map < std::string, std::string > vary_str;
  // Create variable
  void create(std::string vary_name, int vary_value) {
    vary_int[vary_name] = vary_value;
    return;
  }/*
  void create(std::string vary_name, double vary_value) {
    vary_double[vary_name] = vary_value;
    return;
  }
  void create(std::string vary_name, bool vary_value) {
    vary_bool[vary_name] = vary_value;
    return;
  }
  void create(std::string vary_name, char vary_value) {
    vary_char[vary_name] = vary_value;
    return;
  }*/
  void create(std::string vary_name, std::string vary_value) {
    vary_str[vary_name] = vary_value;
    return;
  }
  // Read variable
  bool read(std::string vary_name, int &return_value) {
    if (vary_int.find(vary_name) == vary_int.end())
      return false;
    return_value = vary_int.find(vary_name)->second;
    return true;
  }/*
  bool read(std::string vary_name, double &return_value) {
    if (vary_double.find(vary_name) == vary_double.end())
      return false;
    return_value = vary_double.find(vary_name)->second;
    return true;
  }
  bool read(std::string vary_name, bool & return_value) {
    if (vary_bool.find(vary_name) == vary_bool.end())
      return false;
    return_value = vary_bool.find(vary_name)->second;
    return true;
  }
  bool read(std::string vary_name, char &return_value) {
    if (vary_char.find(vary_name) == vary_char.end())
      return false;
    return_value = vary_char.find(vary_name)->second;
    return true;
  }*/
  bool read(std::string vary_name, std::string & return_value) {
    if (vary_str.find(vary_name) == vary_str.end())
      return false;
    return_value = vary_str.find(vary_name)->second;
    return true;
  }
  // Write variable
  bool write(std::string vary_name, int vary_value) {
    if (vary_int.find(vary_name) == vary_int.end())
      return false;
    vary_int[vary_name] = vary_value;
    return true;
  }/*
  bool write(std::string vary_name, double vary_value) {
    if (vary_double.find(vary_name) == vary_double.end())
      return false;
    vary_double[vary_name] = vary_value;
    return true;
  }
  bool write(std::string vary_name, bool vary_value) {
    if (vary_bool.find(vary_name) == vary_bool.end())
      return false;
    vary_bool[vary_name] = vary_value;
    return true;
  }
  bool write(std::string vary_name, char vary_value) {
    if (vary_char.find(vary_name) == vary_char.end())
      return false;
    vary_char[vary_name] = vary_value;
    return true;
  }*/
  bool write(std::string vary_name, std::string vary_value) {
    if (vary_str.find(vary_name) == vary_str.end())
      return false;
    vary_str[vary_name] = vary_value;
    return true;
  }
}
#endif