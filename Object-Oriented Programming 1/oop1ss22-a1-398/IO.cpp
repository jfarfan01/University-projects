//----------------------------------------------------------------------------------------------------------------------
// Assignment 1 - OOP1 SS2022
//
//
//
// Author: Tutors
//----------------------------------------------------------------------------------------------------------------------
//
#include "IO.hpp"

#include <iostream>
#include <iomanip>

//----------------------------------------------------------------------------------------------------------------------
// static constants
const std::string IO::OUT_OF_MEMORY = "Error: not enough memory!";
const std::string IO::ERR_INVALID_CALL = "Usage: ./a1 <configfile>";
const std::string IO::ERR_CONFIG_NOT_READABLE = "Error: could not open config file!";
const std::string IO::ERR_INVALID_CONFIG_FILE = "Error: invalid config file!";
const std::string IO::ERR_USERNAME_ALREADY_EXISTS = "Error: username already in use!";
const std::string IO::ERR_USERNAME_UNKNOWN = "Error: unknown user name!";
const std::string IO::ERR_INVALID_PASSWORD1 = "Error: invalid password (";
const std::string IO::ERR_INVALID_PASSWORD2 = " of 3 tries)!";
const std::string IO::ERR_ADD_CONTACT_NOT_POSSIBLE = "Error: you cannot add this user as a contact!";
const std::string IO::ERR_MESSAGE_NOT_SENT = "Error: message could not be sent!";
const std::string IO::ERR_MESSAGE_NOT_READ = "Error: message could not be read!";
const std::string IO::ERR_CONFIG_NOT_UPDATED = "Error: could not update config file!";

const std::string IO::WELCOME_MESSAGE = "Welcome to the OOP1 Messaging Service!\n"
                                        "Would you like to [r]egister, [l]ogin or [t]est?";
const std::string IO::ENTER_USERNAME = "Please enter user name!";
const std::string IO::ENTER_PASSWORD = "Please enter your password!";
const std::string IO::MENU_CHOOSE = "\n\nChoose your option: [a]dd contact, [e]ncrypt, [d]ecrypt, [s]how or [q]uit";
const std::string IO::MENU_WHO_ADD_CONTACT = "Who would you like to add as a contact?";
const std::string IO::MENU_CHOOSE_CIPHER = "What cipher would you like to use?";
const std::string IO::MENU_CHOOSE_RECIPIENT = "Who would you like to write to?";
const std::string IO::MENU_CHOOSE_FILENAME = "What should be the (file)name of your message?";
const std::string IO::MENU_ENTER_MESSAGE = "Enter your message now:";
const std::string IO::MENU_CHOOSE_MESSAGE = "Which message would you like to read?";
const std::string IO::BYE = "Bye!";

const std::string IO::TEST_ENTER_CIPHER = "What cipher would you like to use?";
const std::string IO::TEST_ENTER_MESSAGE = "Enter your message now:";

//----------------------------------------------------------------------------------------------------------------------
MenuOption IO::inputOption(const std::string& options)
{
  while (true)
  {
    std::string input;
    std::cout << "  > ";
    std::getline(std::cin, input);

    if (input.length() == 1 && options.find(input.at(0)) != std::string::npos)
    {
      switch(input.at(0))
      {
        case 'l':
          return MenuOption::LOGIN;
        case 'r':
          return MenuOption::REGISTER;
        case 't':
          return MenuOption::TEST;
        case 'a':
          return MenuOption::ADD;
        case 'e':
          return MenuOption::ENCRYPT;
        case 'd':
          return MenuOption::DECRYPT;
        case 's':
          return MenuOption::SHOW;
        case 'q':
          return MenuOption::QUIT;
      }
    }

    std::cout << "Error: invalid option!" << std::endl;
  }

  return MenuOption::INVALID;
}

//----------------------------------------------------------------------------------------------------------------------
std::string IO::inputUsername()
{
  std::string input;

  while (true)
  {
    std::cout << "  > ";
    std::getline(std::cin, input);

    bool invalid = input.length() == 0;

    for (auto& c : input)
    {
      if (!isalnum(c))
      {
        invalid = true;
        break;
      }
    }

    if (invalid)
    {
      std::cout << "Error: invalid username!" << std::endl;
      continue;
    }

    return input;
  }

  return "";
}

//----------------------------------------------------------------------------------------------------------------------
std::string IO::inputPassword(size_t& fail_counter)
{
  std::string input;

  while (fail_counter < MAX_LOGIN_TRIES || fail_counter == (size_t) -1)
  {
    std::cout << "  > ";
    std::getline(std::cin, input);

    bool invalid = input.length() == 0;

    for (auto& c : input)
    {
      if (!isalnum(c))
      {
        invalid = true;
        break;
      }
    }

    if (invalid)
    {
      if (fail_counter != (size_t) -1)
      {
        fail_counter++;
        std::cout << "Error: invalid password (" << fail_counter << " of " <<
          MAX_LOGIN_TRIES << " tries)!" << std::endl;
      }
      else
      {
        std::cout << "Error: invalid password!" << std::endl;
      }

      continue;
    }

    return input;
  }

  std::cout << "Error: failed to insert correct password three times!" << std::endl;
  return "";
}

//----------------------------------------------------------------------------------------------------------------------
std::string IO::inputFilename()
{
  std::string input;

  while (true)
  {
    std::cout << "  > ";
    std::getline(std::cin, input);

    if (input.length() == 0)
    {
      std::cout << "Error: invalid (file)name!" << std::endl;
      continue;
    }

    return input;
  }

  return "";
}

//----------------------------------------------------------------------------------------------------------------------
CipherType IO::inputCipher()
{
  CipherType type = CipherType::INVALID;
  std::string input;

  while (type == CipherType::INVALID)
  {
    std::cout << "  > ";
    std::getline(std::cin, input);

    for (auto& c : input)
      c = std::toupper(c);

    type = Cipher::getCipherType(input);

    if (type == CipherType::INVALID)
      std::cout << "Error: invalid cipher!" << std::endl;
  }

  return type;
}

//----------------------------------------------------------------------------------------------------------------------
std::string IO::inputMessage()
{
  std::string input;

  while (true)
  {
    std::cout << "  > ";
    std::getline(std::cin, input);

    bool invalid = input.length() == 0;

    for (auto& c : input)
    {
      if (!isalpha(c) && !isspace(c))
      {
        invalid = true;
        break;
      }
    }

    if (invalid)
    {
      std::cout << "Error: invalid message!" << std::endl;
      continue;
    }

    return input;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void IO::printContacts(User* user)
{
  std::cout << "Contacts of user: " << user->getName() << std::endl;
  std::cout << "-----------------------------------" << std::endl;
  for(auto const& [contact, cipher] : user->getContacts())
  {
    std::cout << std::setw(15) << cipher->getCipherTypeString() << " | " << contact->getName() << std::endl;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void IO::printEncryptedMessage(const std::string& message)
{
  std::cout << "\nSending ..." << std::endl;
  std::cout << message;
}

//----------------------------------------------------------------------------------------------------------------------
void IO::printDecryptedMessage(const std::string& recipient, const std::string& sender, const std::string& plain_text)
{
  std::cout << "\nReading ...\nRecipient: " << recipient << "\nSender: " << 
    sender << "\n" << plain_text << std::endl;;
}
