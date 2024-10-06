//----------------------------------------------------------------------------------------------------------------------
// Assignment 1 - OOP1 SS2022
//
//
//
// Author: Tutors
//----------------------------------------------------------------------------------------------------------------------
//
#ifndef UTILS_HPP
#define UTILS_HPP

#include "Cipher.hpp"
#include <string>

class User;

class Utils
{
  private:
    static const size_t KEY_LENGTH = 16;

    Utils() = delete;
    Utils(const Utils&) = delete;
    ~Utils() = default;

  public:
    static bool decimalStringToInt(const std::string& str, char& number);
    static bool convertHexTo64BitNumber(const std::string& str, uint64_t& number);
    static Key deriveKey(const std::string& name1, const std::string& name2);

    static bool sortContactsAlphabetically(User* a, User* b);
};

#endif //UTILS_HPP
