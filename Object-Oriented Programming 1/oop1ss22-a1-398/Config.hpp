#ifndef CONFIG_HPP
#define CONFIG_HPP
// #pragma once
#include <string>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#include "User.hpp"
#include <cstdint>
#include <iomanip>
using std::string;
using std::endl;

class Config
{
  std::fstream file_;
  string filename_;
  bool modified_;
  std::vector <User*> users_;
  public:
  Config(const Config&) = delete;

  //  TODO constructor (just fstream variable)

  Config(const std::string& filename)
  : file_{fstream(filename,ios::app)},filename_{filename}{}
  void setConfigModified()
  {modified_= true;return;}
  bool validateUnPw(const std::string& string) const;
  bool verifyCipher(const string& string) const;
  bool isOpen() const{return true;}
  bool parseFile() ;
  bool containsUser(const std::string& name) const;
  User* getUser(const std::string& name) const ;
  User* registerUser(const string& name, const string& password);
  User* loginUser(const std::string& name, const std::string& password) const;  
  bool updateConfigFile();
  ~Config()
  {
    for(auto& u: users_)
    {
      delete u;
    }
  }
};




#endif