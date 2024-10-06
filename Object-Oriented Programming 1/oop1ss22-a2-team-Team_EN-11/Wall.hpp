//---------------------------------------------------------------------------------------------------------------------
// Wall.cpp
//
// Subclass of tile
// implements string representation and handles construction of tile behavior
//
// Author: 11942956, 11812041, 12130995
//---------------------------------------------------------------------------------------------------------------------
//
#include "BasicTile.hpp"

#ifndef WALL_HPP
#define WALL_HPP


class Wall : public BasicTile
{
  private:
    //not really sure how to do this
    static std::array<std::string, 3> const string_rep_;
    //room id for printing purposes, only assigned to top left (first) tile in room
    //rooms without have a space ' ' instead
    char room_id_wall_;

  public:
    //---------------------------------------------------------------------------------------------------------------------
    /// Constructor
    Wall();

    //---------------------------------------------------------------------------------------------------------------------
    /// Constructor overload with room id
    Wall(char room_id);


    /// should this be a vector of strings? every tile is 3 high and 7 wide so a vector of 3 lines would make sense
    /// need to look more into the use to know what is actually allowed (or what they mean by the representation 
    /// as a string, like for example as a single string with \n)
    std::array<std::string,3> const getTileString() override;
};

#endif