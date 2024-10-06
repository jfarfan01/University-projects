//---------------------------------------------------------------------------------------------------------------------
// Monster.cpp
//
// Subclass of tile
// implements string representation and handles construction of tile behavior
//
// Author: 11942956, 11812041, 12130995
//---------------------------------------------------------------------------------------------------------------------
//
#include "Monster.hpp"

Monster::Monster()
  : MagicTile(false, false, 'M') {};

//std::string const Monster::string_rep_ = "       \n   M   \n       ";
std::array<std::string, 3>  Monster::string_rep = 
{
  "       ","   M   ","       "
};

std::array<std::string, 3> Monster::getMagicString()
{
  //not walkable until beaten by the fighter
  return string_rep;
}