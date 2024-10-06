//---------------------------------------------------------------------------------------------------------------------
// GameException.hpp
//
// Definition of singleton class game
// 
//
// Author: 11942956, 11812041, 12130995
//---------------------------------------------------------------------------------------------------------------------
//

#include <iostream>
#include "GameException.hpp"

 InvalidConfig::InvalidConfig(const char* message) : message_{message}  {
      std::cout << message;
 }