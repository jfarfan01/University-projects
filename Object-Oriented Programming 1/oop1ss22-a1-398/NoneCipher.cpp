#include "NoneCipher.hpp"
#include <string>

std::string NoneCipher::getCipherTypeString() const 
{
  std::string string = "NONE";
  return string;
}


std::string NoneCipher::encrypt(const std::string& plain_text)
{
  std::string string = preparePlainText(plain_text);
  return string;
}

std::string NoneCipher::decrypt(const std::string& cipher_text)
{
  //std::string string = preparePlainText(cipher_text);
  return cipher_text;
}
