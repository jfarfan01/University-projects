//---------------------------------------------------------------------------------------------------------------------
// Crystal.cpp
//
// Subclass of tile
// implements string representation and handles construction of tile behavior
//
// Author: 11942956, 11812041, 12130995
//---------------------------------------------------------------------------------------------------------------------
//
#include "Crystal.hpp"

Crystal::Crystal()
  : MagicTile(true, true, 'C') {};

  std::array<std::string, 3> Crystal::string_rep= 
  {
    "  / \\  "," |   | ","  \\ /  "
  };

std::array<std::string, 3> Crystal::getMagicString()
{
  std::array<std::string, 3> representation = string_rep;
  Tile::tileCharModifier(representation, getCharacterType());
  //walkable, jumpable, and changeable via use of bonus field
  return representation;
}
