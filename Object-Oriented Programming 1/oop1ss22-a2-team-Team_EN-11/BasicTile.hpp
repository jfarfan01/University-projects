//---------------------------------------------------------------------------------------------------------------------
// BasicTile.hpp
//
// Subclass of tile
// Includes tiles without magic function: wall or pathway
//
// Author: 11942956, 11812041, 12130995
//---------------------------------------------------------------------------------------------------------------------
//
#include "Tile.hpp"

#ifndef BASICTILE_HPP
#define BASICTILE_HPP



class BasicTile : public Tile
{
  private:


  public:
    //---------------------------------------------------------------------------------------------------------------------
    /// returns ASCII representation of tile
    //virtual std::vector<std::string> const getTileString() = 0;

    //---------------------------------------------------------------------------------------------------------------------
    /// Constructor
    BasicTile(bool walkable, bool jumpable, char id);

    ~BasicTile() = default;

};

//very much not elegant but no choice at this point
#include "Wall.hpp"
#include "Passage.hpp"

#endif