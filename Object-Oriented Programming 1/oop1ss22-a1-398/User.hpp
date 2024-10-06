#ifndef USER_HPP
#define USER_HPP
// #pragma once
#include <string>
#include <cstdint>
#include <map>
#include "Cipher.hpp"
#include "Utils.hpp"
#include "NoneCipher.hpp"
#include "AsciiCipher.hpp"
#include "CaesarCipher.hpp"
#include <iostream>
#include <fstream>
#include "IO.hpp"
using std::string;
using std::map;
using std::endl;
using std::cout;


class User
{
  string name_;
  string password_;
  std::map<User*, Cipher*, bool(*)(User*, User*)> contacts_;
  //std::map<User*, Cipher*> contacts_;

  public:

  User(const std::string& name, const std::string& password)
  : name_{name},password_{password}, contacts_ {Utils::sortContactsAlphabetically} {}

  string getName()const {return name_;}
  string getPassword() const {return password_;}
  const auto& getContacts() const {return contacts_;}
  bool verifyPassword(const std::string& password) const;
  bool hasContact(const std::string& name) const;
  void addContact(User* contact, CipherType type, Key key);
  Cipher* getCipherForContact(const std::string& name) const;
  bool sendMessage(string& recipient, string& filename, const string& plain_text) const;
  bool readMessage(const std::string& filename) const;

  ~User()
  {  
    for(auto& [contact, cipher] : contacts_)
    {
      delete cipher;
    }
  }

  };


#endif