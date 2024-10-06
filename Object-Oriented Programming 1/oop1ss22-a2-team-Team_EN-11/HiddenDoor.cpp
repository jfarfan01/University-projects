//---------------------------------------------------------------------------------------------------------------------
// HiddenDoor.cpp
//
// Subclass of tile
// implements string representation and handles construction of tile behavior
//
// Author: 11942956, 11812041, 12130995
//---------------------------------------------------------------------------------------------------------------------
//
#include "HiddenDoor.hpp"

HiddenDoor::HiddenDoor()
  : MagicTile(false, false, 'X') {};
 
std::array<std::string, 3> HiddenDoor::string_rep = 
{
  "███████","███████","███████"
};


std::array<std::string, 3> HiddenDoor::getMagicString()
{
  //not walkable til it shows passage
  return string_rep;
}