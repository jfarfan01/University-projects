//---------------------------------------------------------------------------------------------------------------------
// ButtonFighter.cpp
//
// Subclass of tile
// implements string representation and handles construction of tile behavior
//
// Author: 11942956, 11812041, 12130995
//---------------------------------------------------------------------------------------------------------------------
//
#include "ButtonFighter.hpp"

ButtonFighter::ButtonFighter()
  : MagicTile(true, true, 'F') {};

  std::array<std::string, 3> ButtonFighter::string_rep= 
    {
      "FFFFFFF","F     F","FFFFFFF"
    };

std::array<std::string, 3> ButtonFighter::getMagicString()
{
  //can stand on it without activating
  std::array<std::string, 3> representation = string_rep;
  Tile::tileCharModifier(representation, getCharacterType());
  

  return representation;
}
