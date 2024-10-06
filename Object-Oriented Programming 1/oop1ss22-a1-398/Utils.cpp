//----------------------------------------------------------------------------------------------------------------------
// Assignment 1 - OOP1 SS2022
//
//
//
// Author: Tutors
//----------------------------------------------------------------------------------------------------------------------
//
#include "Utils.hpp"
#include "User.hpp"

//----------------------------------------------------------------------------------------------------------------------
bool Utils::decimalStringToInt(const std::string& str, char& number)
{
  size_t position = 0;

  try
  {
    number = std::stoi(str, &position, 10);
  }
  catch (std::exception&)
  {
    return false;
  }

  return position == str.length();
}

//----------------------------------------------------------------------------------------------------------------------
bool Utils::convertHexTo64BitNumber(const std::string& str, uint64_t& number)
{
  if (str.length() != KEY_LENGTH)
    return false;

  for (auto& c : str)
  {
    if (!isalnum(c))
      return false;
  }

  size_t position = 0;
  try
  {
    number = std::stoull(str, &position, 16);
  }
  catch (std::exception&)
  {
    return false;
  }

  if (position != 16)
    return false;
  
  return true;
}

//----------------------------------------------------------------------------------------------------------------------
Key Utils::deriveKey(const std::string& name1, const std::string& name2)
{
  // some weird function to derive a key from the usernames
  // you should never use this function in an application :^)
  // unfortunately, we are limited in terms of libraries we can use
  // otherwise we could have provided something better here :/

  size_t n1_c = 0, n2_c = 0;
  Key k = 0;

  for (size_t i = 0; i < 59; i++)
  {
    uint64_t next = 0;

    if (i % 2)
    {
      if (n2_c == name2.size())
      {
        n2_c = 0;
      }
      next = toupper(name2.at(n2_c++));
    }
    else
    {
      if (n1_c == name1.size())
      {
        n1_c = 0;
      }
      next = toupper(name1.at(n1_c++));
    }

    k ^= (next << i);
  }

  for (size_t i = 0; i < 8; i++)
  {
    if (i % 2)
    {
      k = (~(k & (0xffull << (i * 8))) & (0xffull << (i * 8))) | (k & ~(0xffull << (i * 8)));
    }
  }

  return k;
}

//----------------------------------------------------------------------------------------------------------------------
bool Utils::sortContactsAlphabetically(User* a, User* b)
{
  return a->getName() < b->getName();
}
