//---------------------------------------------------------------------------------------------------------------------
// MenuException.hpp
//
// Exception to end program with std::cin::eof()
// 
//
// Author: 11942956, 11812041, 12130995
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef MENUEXCEPTION_HPP
#define MENUEXCEPTION_HPP

#include <stdexcept>

class InputEOF : public std::exception
{
  const char* message_{nullptr};

  public:
    //---------------------------------------------------------------------------------------------------------------------
    //see free exercise operator_overloading_and_exceptions
    InputEOF(const char* message);
    //---------------------------------------------------------------------------------------------------------------------
    //overload without message as this is used to end program without message
    InputEOF() = default;
    //---------------------------------------------------------------------------------------------------------------------
    InputEOF(const InputEOF&) = default;
    //---------------------------------------------------------------------------------------------------------------------
    InputEOF operator=(const InputEOF&) = delete;
    //---------------------------------------------------------------------------------------------------------------------
    ~InputEOF() = default;

    const char* what() const noexcept override {return message_;}
};

#endif