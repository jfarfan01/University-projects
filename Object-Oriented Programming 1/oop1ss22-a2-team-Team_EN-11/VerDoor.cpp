//---------------------------------------------------------------------------------------------------------------------
// VerDoor.cpp
//
// Subclass of tile
// implements string representation and handles construction of tile behavior
//
// Author: 11942956, 11812041, 12130995
//---------------------------------------------------------------------------------------------------------------------
//
#include "VerDoor.hpp"

VerDoor::VerDoor()
  : MagicTile(false, false, 'V') {};
 
 std::array<std::string, 3> VerDoor::string_rep = 
{
  "   █   ","   █   ","   █   "
};

std::array<std::string, 3> VerDoor::getMagicString()
{
  //not walkable til it shows passage
  return string_rep;
}