//----------------------------------------------------------------------------------------------------------------------
// Assignment 1 - OOP1 SS2022
//
//
//
// Author: Tutors
//----------------------------------------------------------------------------------------------------------------------
//
#ifndef IO_HPP
#define IO_HPP

#include "User.hpp"
#include "Cipher.hpp"

#include <string>

enum class MenuOption {INVALID, REGISTER, LOGIN, TEST, ADD, ENCRYPT, DECRYPT, SHOW, QUIT};



class IO
{
  private:
    IO() = default;
    IO(const IO&) = delete;
    ~IO() = default;

  public:
    inline static const size_t MAX_LOGIN_TRIES = 3;

    // these functions check for the basic requirements like valid chars
    static MenuOption inputOption(const std::string& options);
    static std::string inputUsername();
    static std::string inputPassword(size_t& fail_counter);
    static std::string inputFilename();
    static CipherType inputCipher();
    static std::string inputMessage();

    static void printContacts(User* user);
    static void printEncryptedMessage(const std::string& message);
    static void printDecryptedMessage(const std::string& recipient, const std::string& sender, 
      const std::string& plain_text);

    static const std::string OUT_OF_MEMORY;
    static const std::string ERR_INVALID_CALL;
    static const std::string ERR_CONFIG_NOT_READABLE;
    static const std::string ERR_INVALID_CONFIG_FILE;
    static const std::string ERR_USERNAME_ALREADY_EXISTS;
    static const std::string ERR_USERNAME_UNKNOWN;
    static const std::string ERR_INVALID_PASSWORD1;
    static const std::string ERR_INVALID_PASSWORD2;
    static const std::string ERR_ADD_CONTACT_NOT_POSSIBLE;
    static const std::string ERR_MESSAGE_NOT_SENT;
    static const std::string ERR_MESSAGE_NOT_READ;
    static const std::string ERR_CONFIG_NOT_UPDATED;

    static const std::string WELCOME_MESSAGE;
    static const std::string ENTER_USERNAME;
    static const std::string ENTER_PASSWORD;
    static const std::string MENU_CHOOSE;
    static const std::string MENU_WHO_ADD_CONTACT;
    static const std::string MENU_CHOOSE_CIPHER;
    static const std::string MENU_CHOOSE_RECIPIENT;
    static const std::string MENU_CHOOSE_FILENAME;
    static const std::string MENU_ENTER_MESSAGE;
    static const std::string MENU_CHOOSE_MESSAGE;
    static const std::string BYE;

    static const std::string TEST_ENTER_CIPHER;
    static const std::string TEST_ENTER_MESSAGE;

};

#endif //IO_HPP
