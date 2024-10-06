#ifndef ASCII_HPP
#define ASCII_HPP
#include "Cipher.hpp"
#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <map>
#include <vector>
#include <sstream>
#include <cstddef>
using namespace std;

class AsciiCipher : public Cipher
{
  public:
  AsciiCipher(const AsciiCipher&) = delete;
  AsciiCipher(Key key) :Cipher (key){}

  std::string getCipherTypeString() const override;
  std::string encrypt(const std::string& plain_text) override;
  std::string decrypt(const std::string& cipher_text) override;

};


#endif