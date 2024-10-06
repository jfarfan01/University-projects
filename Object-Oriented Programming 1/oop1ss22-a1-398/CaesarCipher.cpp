#include <string>
#include "CaesarCipher.hpp"
using std::string;


  string CaesarCipher::getCipherTypeString()const 
  {
    string Caesar = "CAESAR";
    return Caesar;
  }

  string CaesarCipher::encrypt(const string& plain_text)
  {
    string str = "";

    string to_encrypt = Cipher::preparePlainText(plain_text);
    int counter = 7;
    for(auto s: to_encrypt)
    {
      int encryptedChar;

      uint8_t offset = key_ >> (8 * counter);

      //cout << offset << "is the offset" << endl;

      encryptedChar = s + offset;
      while (encryptedChar > 'Z')
      {
        encryptedChar -= 26;
      }
      counter--;
      str.push_back(encryptedChar);
      if (counter < 0) 
      {
        counter = 7;
      }
    }



    return str;
  }

  string CaesarCipher::decrypt(const string& cipher_text)
  {
    string str = "";

    string to_decrypt = Cipher::preparePlainText(cipher_text);
    int counter = 7;
    for(auto s: to_decrypt)
    {
      int decryptedChar;

      uint8_t offset = key_ >> (8 * counter);

      //cout << offset << "is the offset" << endl;

      decryptedChar = s - offset;
      while (decryptedChar < 'A')
      {
        decryptedChar += 26;
      }
      counter--;
      str.push_back(decryptedChar);
      if (counter < 0) 
      {
        counter = 7;
      }
    }
    return str;
  }
