//---------------------------------------------------------------------------------------------------------------------
// HorDoor.cpp
//
// Subclass of tile
// implements string representation and handles construction of tile behavior
//
// Author: 11942956, 11812041, 12130995
//---------------------------------------------------------------------------------------------------------------------
//
#include "MagicTile.hpp"

#ifndef HORDOOR_HPP
#define HORDOOR_HPP

class HorDoor : public MagicTile
{
  private:
    static std::array<std::string, 3> string_rep;
  public:
    //---------------------------------------------------------------------------------------------------------------------
    /// Constructor
    HorDoor();
    //---------------------------------------------------------------------------------------------------------------------
    /// returns ASCII representation of magic tile (active_==true)
    std::array<std::string, 3> getMagicString();
};

#endif