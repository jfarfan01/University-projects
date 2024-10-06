#ifndef NONE_CIPHER_HPP
#define NONE_CIPHER_HPP
#include "Cipher.hpp"

class NoneCipher : public Cipher
{
  public:
  NoneCipher(const NoneCipher&) = delete;
  NoneCipher(Key key) :Cipher (key){}
  //key_ {key} {}

  std::string getCipherTypeString() const override;
  std::string encrypt(const std::string& plain_text) override;
  std::string decrypt(const std::string& cipher_text) override;
  

  

};

#endif