#include <string>
#include "Cipher.hpp"
using std::string;

  
    string Cipher::preparePlainText(const string& plain_text) const
    {
      string prepared_str="";
      //prepared_str.resize(plain_text.size());

      for(char i :plain_text)
      {
        if (i!= ' ')
        {
        prepared_str.push_back(toupper(i)); 
        }
      } 
      return prepared_str;
    };

  Cipher::Cipher(Key key)
  : key_{key}
  {};


CipherType Cipher::getCipherType(const std::string& type)
  {
    if (type =="NONE")
    {
      return CipherType::NONE_CIPHER;
    }
    if (type =="ASCII")
    {
      return CipherType::ASCII_CIPHER;
    }
    if (type =="CAESAR")
    {
      return CipherType::CAESAR_CIPHER;
    }
    return CipherType::INVALID;
  }

