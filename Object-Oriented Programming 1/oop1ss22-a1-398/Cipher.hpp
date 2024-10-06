#ifndef CIPHER_HPP
#define CIPHER_HPP
#include <string>

#include <cstdint>
using std::string;

enum class CipherType { INVALID, NONE_CIPHER, ASCII_CIPHER, CAESAR_CIPHER };

typedef uint64_t Key;

class Cipher
{
  //public:
  protected:
  Key key_;
 
  public:   
    Cipher(const Cipher&) = delete;
    Cipher(Key key);
    Key getKey(){return key_;};
    string virtual getCipherTypeString() const = 0;
    string  preparePlainText(const string& plain_text) const;
    string virtual encrypt(const string& plain_text) = 0;
    string virtual decrypt(const string& cipher_text) = 0;
    static  CipherType getCipherType(const std::string& type);
   
    virtual ~Cipher() = default;
};


#endif
