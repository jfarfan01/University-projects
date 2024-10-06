#ifndef CAESAR_HPP
#define CAESAR_HPP
#include <iostream>
using std::endl;
using std::cout;
#include "Cipher.hpp"


class CaesarCipher : public Cipher
{
  public:
  CaesarCipher(const CaesarCipher&) = delete;
  CaesarCipher(Key key) :Cipher (key){}

  std::string getCipherTypeString() const override;
  std::string encrypt(const std::string& plain_text) override;
  std::string decrypt(const std::string& cipher_text) override;

};



#endif