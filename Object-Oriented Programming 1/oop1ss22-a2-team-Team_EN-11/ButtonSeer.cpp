//---------------------------------------------------------------------------------------------------------------------
// ButtonSeer.cpp
//
// Subclass of tile
// implements string representation and handles construction of tile behavior
//
// Author: 11942956, 11812041, 12130995
//---------------------------------------------------------------------------------------------------------------------
//
#include "ButtonSeer.hpp"

ButtonSeer::ButtonSeer()
  : MagicTile(true, true, 'S') {};

  std::array<std::string, 3> ButtonSeer::string_rep = 
  {
    "SSSSSSS","S     S","SSSSSSS"
  };


std::array<std::string, 3> ButtonSeer::getMagicString()
{
  //can stand on it without activating
  std::array<std::string, 3> representation = string_rep;
  Tile::tileCharModifier(representation, getCharacterType());

  return representation;
}