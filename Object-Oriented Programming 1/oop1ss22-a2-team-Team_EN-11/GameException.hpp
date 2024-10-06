//---------------------------------------------------------------------------------------------------------------------
// GameException.hpp
//
// Definition of singleton class game
// 
//
// Author: 11942956, 11812041, 12130995
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef GAMEEXCEPTION_HPP
#define GAMEEXCEPTION_HPP

#include <stdexcept>

class InvalidConfig : public std::exception
{
  const char* message_;

  public:
    //---------------------------------------------------------------------------------------------------------------------
    //see free exercise operator_overloading_and_exceptions
    InvalidConfig(const char* message);
    //---------------------------------------------------------------------------------------------------------------------
    InvalidConfig(const InvalidConfig&) = default;
    //---------------------------------------------------------------------------------------------------------------------
    InvalidConfig operator=(const InvalidConfig&) = delete;
    //---------------------------------------------------------------------------------------------------------------------
    ~InvalidConfig() = default;

    const char* what() const noexcept override {return message_;}
};

#endif