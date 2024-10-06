//---------------------------------------------------------------------------------------------------------------------
// Passage.hpp
//
// Subclass of tile
// implements string representation and handles construction of tile behavior
//
// Author: 11942956, 11812041, 12130995
//---------------------------------------------------------------------------------------------------------------------
//
#include "BasicTile.hpp"

#ifndef PASSAGE_HPP
#define PASSAGE_HPP

class Passage : public BasicTile
{
  private:
    //too late to change it to be all in one tile
    static std::array<std::string,3>const string_rep_;

  public:
    //---------------------------------------------------------------------------------------------------------------------
    /// Constructor
    Passage();

    //---------------------------------------------------------------------------------------------------------------------
    /// returns the representation of the tile
    /// considers the character on the tile while doing so
    std::array<std::string,3> const getTileString() override;
};


#endif