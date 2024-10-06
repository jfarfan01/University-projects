//---------------------------------------------------------------------------------------------------------------------
// Loot.cpp
//
// Subclass of tile
// implements string representation and handles construction of tile behavior
//
// Author: 11942956, 11812041, 12130995
//---------------------------------------------------------------------------------------------------------------------
//
#include "Loot.hpp"

Loot::Loot()
  : MagicTile(true, true, 'L') {};
 
 std::array<std::string, 3> Loot::string_rep = 
{
  "$$$$$$$","$     $","$$$$$$$"
};

std::array<std::string, 3> Loot::getMagicString()
{
  std::array<std::string, 3> representation = string_rep;
  Tile::tileCharModifier(representation, getCharacterType());
  return representation;
}