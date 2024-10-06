//---------------------------------------------------------------------------------------------------------------------
// Wall.cpp
//
// Subclass of tile
// implements string representation and handles construction of tile behavior
//
// Author: 11942956, 11812041, 12130995
//---------------------------------------------------------------------------------------------------------------------
//
#include "Wall.hpp"

std::array<std::string, 3> const Wall::string_rep_ = 
{
  "███████", "███████", "███████"
};

//---------------------------------------------------------------------------------------------------------------------
Wall::Wall()
  : BasicTile(false, false, 'W'), room_id_wall_{' '} {};

//---------------------------------------------------------------------------------------------------------------------
Wall::Wall(char room_id)
  : BasicTile(false, false, 'W'), room_id_wall_{room_id} {};



//---------------------------------------------------------------------------------------------------------------------
std::array<std::string,3> const Wall::getTileString()
{
  std::array<std::string, 3> representation = string_rep_;
  //wall cannot be stepped on

  //this circumvents the problem with using .at which does not erase the extra storage space needed by the █
  representation[1] = "███";
  if (room_id_wall_ != ' ')
  {
    //representation[1].at(3) = room_id_wall_;
    representation[1] += room_id_wall_;
  }
  else
  {
    representation[1] += "█";
  }
  representation[1] += "███";
  return representation;
}