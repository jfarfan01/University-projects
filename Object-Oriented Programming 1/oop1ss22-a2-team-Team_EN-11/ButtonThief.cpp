//---------------------------------------------------------------------------------------------------------------------
// ButtonThief.cpp
//
// Subclass of tile
// implements string representation and handles construction of tile behavior
//
// Author: 11942956, 11812041, 12130995
//---------------------------------------------------------------------------------------------------------------------
//
#include "ButtonThief.hpp"

ButtonThief::ButtonThief()
  : MagicTile(true, true, 'T') {};

std::array<std::string, 3> ButtonThief::string_rep= 
  {
    "TTTTTTT","T     T","TTTTTTT"
  };


std::array<std::string, 3> ButtonThief::getMagicString()
{
  //can stand on it without activating
  std::array<std::string, 3> representation = string_rep;
  Tile::tileCharModifier(representation, getCharacterType());

  return representation;
}