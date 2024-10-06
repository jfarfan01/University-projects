//---------------------------------------------------------------------------------------------------------------------
// HorDoor.cpp
//
// Subclass of tile
// implements string representation and handles construction of tile behavior
//
// Author: 11942956, 11812041, 12130995
//---------------------------------------------------------------------------------------------------------------------
//
#include "HorDoor.hpp"

HorDoor::HorDoor()
  : MagicTile(false, false, 'H') {};
 
 std::array<std::string, 3> HorDoor::string_rep = 
{
  "       ","███████","       "
};

std::array<std::string, 3> HorDoor::getMagicString()
{
  //not walkable til it shows passage
  return string_rep;
}