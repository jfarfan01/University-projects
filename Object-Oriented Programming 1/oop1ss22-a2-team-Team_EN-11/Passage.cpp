//---------------------------------------------------------------------------------------------------------------------
// Passage.cpp
//
// Subclass of tile
// implements string representation and handles construction of tile behavior
//
// Author: 11942956, 11812041, 12130995
//---------------------------------------------------------------------------------------------------------------------
//

#include "Passage.hpp"

//---------------------------------------------------------------------------------------------------------------------
std::array<std::string, 3> const Passage::string_rep_ = 
{
  "       ", "       ", "       "
};

//---------------------------------------------------------------------------------------------------------------------
Passage::Passage()
  : BasicTile(true, true, 'P') {};

//---------------------------------------------------------------------------------------------------------------------
std::array<std::string, 3>  const Passage::getTileString()
{
  std::array<std::string, 3> representation = string_rep_;

  Tile::tileCharModifier(representation, getCharacterType());

  return representation;
}