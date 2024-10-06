//---------------------------------------------------------------------------------------------------------------------
// MagicTile.hpp
//
// Subclass of tile
// Includes tiles with magic function: 
//
// Author: 11812041, 11942956, 12130995
//
//TODO: Couldn't we encapsulate the majority of these tiles, seeing as they consist of the same methods? -> too late
//---------------------------------------------------------------------------------------------------------------------
//
#include "Tile.hpp"

#ifndef MAGICTILE_HPP
#define MAGICTILE_HPP



class MagicTile : public Tile
{
  private:
    static const std::array<std::string, 3> passage_string_rep;


  public:

    //---------------------------------------------------------------------------------------------------------------------
    //constructor
    //sets active_ true and passses other attributes on
    MagicTile(bool walkable, bool jumpable, char id);

    //---------------------------------------------------------------------------------------------------------------------
    //destructor
    ~MagicTile() = default;


    //---------------------------------------------------------------------------------------------------------------------
    /// returns ASCII representation of tile with help of getMagicString
    std::array<std::string,3> const getTileString() override;

    //---------------------------------------------------------------------------------------------------------------------
    /// returns ASCII representation of magic tile (active_==true)
    /// uses Tile::tileCharModifier() if needed
    virtual std::array<std::string, 3> getMagicString() = 0;

    //---------------------------------------------------------------------------------------------------------------------
    /// changes tile into walkway tile after magic was used
    /// I think it would make sense to just turn off interactibility and have it look like a passageway instead of actually making it a basic tile
    bool magicUse();
};

//very much not elegant but no choice at this point
#include "ButtonFighter.hpp"
#include "ButtonSeer.hpp"
#include "ButtonThief.hpp"
#include "Crystal.hpp"
#include "HiddenDoor.hpp"
#include "HorDoor.hpp"
#include "VerDoor.hpp"
#include "HourGlass.hpp"
#include "Loot.hpp"
#include "Monster.hpp"

#endif