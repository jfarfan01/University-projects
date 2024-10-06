//----------------------------------------------------------------------------------------------------------------------
// Assignment 1 - OOP1 SS2022
//
//
//
// Author: Tutors
//----------------------------------------------------------------------------------------------------------------------
//
#include "IO.hpp"
#include "User.hpp"
#include "Utils.hpp"
#include "Config.hpp"
#include "NoneCipher.hpp"
#include "AsciiCipher.hpp"
#include "CaesarCipher.hpp"

#include <iostream>
#include <string>

enum ErrorValue
{
  OK = 0,
  MEMORY_ERROR = 1,
  WRONG_PARAMETER_COUNT = 2,
  COULD_NOT_OPEN_FILE = 3,
  INVALID_CONFIG = 4,
  MAX_PASSWORD_RETRIES = 5
};

User* userRegistration(Config& config);
User* userLogin(Config& config);
void menu(Config& config, User* active_user);
void testCiphers();

//----------------------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
  /**
   * error checks
   */
  if (argc != 2)
  {
    std::cout << IO::ERR_INVALID_CALL << std::endl;
    return ErrorValue::WRONG_PARAMETER_COUNT;
  }
  try
  {
    Config config(argv[1]); // call config constructor with the filename
    if (!config.isOpen())
    {
      std::cout << IO::ERR_CONFIG_NOT_READABLE << std::endl;
      return ErrorValue::COULD_NOT_OPEN_FILE;
    }

    if (!config.parseFile()) // parse the file into the config - read users and contacts
    {
      std::cout << IO::ERR_INVALID_CONFIG_FILE << std::endl;
      return ErrorValue::INVALID_CONFIG;
    }

    std::cout << IO::WELCOME_MESSAGE << std::endl;
    MenuOption option = IO::inputOption("rlt");
    if (option == MenuOption::TEST)
    {
      testCiphers();
      return ErrorValue::OK;
    }

    /**
     * registration / login
     */
    User* active_user = nullptr; // c++ equivalent of NULL
    if (option == MenuOption::REGISTER)
    {
      active_user = userRegistration(config);
    }
    else if (option == MenuOption::LOGIN)
    {
      active_user = userLogin(config);
      if (active_user == nullptr)
        return ErrorValue::MAX_PASSWORD_RETRIES;
    }

    menu(config, active_user);
  }
  catch (std::bad_alloc& e)  // in c++ an exception is thrown if no more memory on heap
  {
    std::cout << IO::OUT_OF_MEMORY << std::endl;
    return ErrorValue::MEMORY_ERROR;
  }

  return ErrorValue::OK;
}

//----------------------------------------------------------------------------------------------------------------------
User* userRegistration(Config& config)
{
  std::cout << IO::ENTER_USERNAME << std::endl;

  std::string username;

  while (true)
  {
    username = IO::inputUsername(); // read username from stdin

    if (!config.containsUser(username)) // checks if user exists - method must be implemented
      break;

    std::cout << IO::ERR_USERNAME_ALREADY_EXISTS << std::endl;
  }

  std::cout << IO::ENTER_PASSWORD << std::endl;
  size_t password_tries = (size_t) -1;
  std::string password = IO::inputPassword(password_tries); // read password from stdin

  // registers user and returns pointer to the newly created user - method must be implemented
  return config.registerUser(username, password);
}

//----------------------------------------------------------------------------------------------------------------------
User* userLogin(Config& config)
{
  User* active_user = nullptr;

  std::cout << IO::ENTER_USERNAME << std::endl;
  std::string username;

  while (true)
  {
    username = IO::inputUsername(); // read username from stdin

    if (config.containsUser(username)) // checks if user exists - method must be implemented
      break;

    std::cout << IO::ERR_USERNAME_UNKNOWN << std::endl;
  }

  std::cout << IO::ENTER_PASSWORD << std::endl;
  size_t password_tries = 0;

  while (password_tries < IO::MAX_LOGIN_TRIES && active_user == nullptr)
  {
    std::string password = IO::inputPassword(password_tries); // read password from stdin
    // tries to login in user with username and password - method must be implemented
    active_user = config.loginUser(username, password);

    if (active_user == nullptr)
    {
      password_tries++;
      std::cout << IO::ERR_INVALID_PASSWORD1 << password_tries << IO::ERR_INVALID_PASSWORD2 << std::endl;
    }
  }

  return active_user;
}

//----------------------------------------------------------------------------------------------------------------------
void menu(Config& config, User* active_user)
{
  /**
   * The options of the menu are:
   * - encrypt
   * - decrypt
   * - quit
   * - show contacts
   * - add contact
   */
  while (true)
  {
    std::cout << IO::MENU_CHOOSE << std::endl;
    MenuOption option = IO::inputOption("aedsq");

    switch (option)
    {
      case MenuOption::ADD:
      {
        std::cout << IO::MENU_WHO_ADD_CONTACT << std::endl;
        std::string contact_name;
        User* contact = nullptr;

        while (true)
        {
          contact_name = IO::inputUsername();
          contact = config.getUser(contact_name);
          if (active_user->getName() != contact_name && !active_user->hasContact(contact_name) &&
              config.containsUser(contact_name))
            break;

          std::cout << IO::ERR_ADD_CONTACT_NOT_POSSIBLE << std::endl;
        }

        std::cout << IO::MENU_CHOOSE_CIPHER << std::endl;
        CipherType cipher = IO::inputCipher();

        Key key = Utils::deriveKey(active_user->getName(), contact_name);
        active_user->addContact(contact, cipher, key);
        contact->addContact(active_user, cipher, key);

        config.setConfigModified();
        break;
      }
      case MenuOption::ENCRYPT:
      {
        std::cout << IO::MENU_CHOOSE_RECIPIENT << std::endl;
        std::string contact_name = IO::inputUsername();

        std::cout << IO::MENU_CHOOSE_FILENAME << std::endl;
        std::string filename = IO::inputFilename();

        std::cout << IO::MENU_ENTER_MESSAGE << std::endl;
        std::string message = IO::inputMessage();

        if (!active_user->sendMessage(contact_name, filename, message))
        {
          std::cout << IO::ERR_MESSAGE_NOT_SENT << std::endl;
        }
        break;
      }
      case MenuOption::DECRYPT:
      {
        std::cout << IO::MENU_CHOOSE_MESSAGE << std::endl;
        std::string filename = IO::inputFilename();

        if (!active_user->readMessage(filename))
        {
          std::cout << IO::ERR_MESSAGE_NOT_READ << std::endl;
        }
        break;
      }
      case MenuOption::SHOW:
      {
        IO::printContacts(active_user);
        break;
      }
      case MenuOption::QUIT:
      {
        std::cout << IO::BYE << std::endl;
        if (!config.updateConfigFile())
        {
          std::cout << IO::ERR_CONFIG_NOT_UPDATED << std::endl;
        }
        return;
      }
      default:
        return;
    }
  }
}

void testCiphers()
{
  Key key = 0x516a1f4f83854e9c;

  std::cout << IO::TEST_ENTER_CIPHER << std::endl;
  CipherType cipher = IO::inputCipher();

  std::cout << IO::TEST_ENTER_MESSAGE << std::endl;
  std::string message = IO::inputMessage();

  Cipher* c = nullptr;
  switch (cipher)
  {
    case CipherType::ASCII_CIPHER:
      c = new AsciiCipher(key);
      break;
    case CipherType::CAESAR_CIPHER:
      c = new CaesarCipher(key);
      break;
    case CipherType::NONE_CIPHER:
      c = new NoneCipher(key);
    default:
      break;
  }

  std::string ciphertext = c->encrypt(message);
  IO::printEncryptedMessage(ciphertext);

  std::string plaintext = c->decrypt(ciphertext);
  IO::printDecryptedMessage("SYSTEM", "SYSTEM", plaintext);

  delete c;
}
