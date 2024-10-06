#include <string>
#include "AsciiCipher.hpp"
using std::string;


  string AsciiCipher::getCipherTypeString()const 
  {
    string Ascii = "ASCII";
    return Ascii;
  }


  string AsciiCipher::encrypt(const string& plain_text)
  {
    string to_encrypt = Cipher::preparePlainText(plain_text);

    auto mod = key_ % 10;
    string cipher_text;
    unsigned long i = 0;
    int char_as_int;
    string charac ;
    for(i = 0; to_encrypt[i] != '\0';i++)
    {
      char_as_int = to_encrypt[i] -mod;

      charac =std::to_string(char_as_int);
      if (i ==0)
      {
        cipher_text = charac;
      }
      else
      {
        cipher_text.append(charac);
      }

      if (i< to_encrypt.size()-1)
      {   
        cipher_text.append(" ");
      }
    }
    cipher_text.append("\0");
    return cipher_text;  
  }




  

  string AsciiCipher::decrypt(const string& cipher_text)
  {
    int i = 0,j =0;
    string pre_num;
    pre_num.resize(3);


      std::stringstream ss(cipher_text);
      std::vector<int> numbers;

      string temp_string;

      for(int i = 0; ss >> i; ) 
      {
        numbers.push_back(i);
      }
      auto mod = key_ % 10;


      for (int &n : numbers)
      {
        n = n + mod; //mod
        j++;
      }

      for (int n : numbers)
      {
        if (i == 0)
        {     
          temp_string = std::to_string(n);
        }

          else 
          {
            temp_string.append(std::to_string(n));
          }
          i++;
      }



    string s = "";
    array<char, 500> char_arr{};
      for (auto &number : numbers) 
      {
        sprintf(char_arr.data(), "%c", number);
        s = s + char_arr.data();
      }
   
    return s;
  }