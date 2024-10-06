//---------------------------------------------------------------------------------------------------------------------
// MagicTile.cpp
//
// Subclass of tile
// Includes tiles with magic function: 
//
// Author: 11812041, 11942956, 12130995
//---------------------------------------------------------------------------------------------------------------------
//

#include "MagicTile.hpp"

//---------------------------------------------------------------------------------------------------------------------
MagicTile::MagicTile(bool walkable, bool jumpable, char id)
  : Tile{walkable, jumpable, true, id} {};


//---------------------------------------------------------------------------------------------------------------------
//representation of inactive tile
  const std::array<std::string, 3> MagicTile::passage_string_rep =
  {
    "       ","       ","       "
  };


//---------------------------------------------------------------------------------------------------------------------
std::array<std::string,3> const MagicTile::getTileString()
{
  //check for active -> if inactive instead show passageway and make walkable/jumpable
  if (active_ == false)
  {
    std::array<std::string, 3> representation = passage_string_rep;
    Tile::tileCharModifier(representation, getCharacterType());
    return representation;
  }
  else
  {
    //not sure if tileCharModifier is needed here as well or if it instantly becomes used for tiles that are walkable
    //going to assume it is instant
    return getMagicString();
  }
}
