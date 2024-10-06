//---------------------------------------------------------------------------------------------------------------------
// HourGlass.cpp
//
// Subclass of tile
// implements string representation and handles construction of tile behavior
//
// Author: 11942956, 11812041, 12130995
//---------------------------------------------------------------------------------------------------------------------
//
#include "HourGlass.hpp"

HourGlass::HourGlass()
  : MagicTile(true, true, 'G') {};

  std::array<std::string, 3> HourGlass::string_rep = 
  {
    " \\   / ","  | |  "," /   \\ "
  };

std::array<std::string, 3> HourGlass::getMagicString()
{
  std::array<std::string, 3> representation = string_rep;
  //not needed here
  //Tile::tileCharModifier(representation, character_->getCharacterType());
  //walkable, jumpable, and changeable via use of bonus field
  return string_rep;
}