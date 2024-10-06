//---------------------------------------------------------------------------------------------------------------------
// Tile.cpp
//
// Base class for tiles
// 
//
// Author: 11942956, 11812041, 12130995
//---------------------------------------------------------------------------------------------------------------------
//

#include "Tile.hpp"


///---------------------------------------------------------------------------------------------------------------------
Tile::Tile( bool walkable, bool jumpable, bool active, char id) : 
            tile_id_{id}, character_{nullptr}, walkable_{walkable}, 
            jumpable_{jumpable}, active_{active}, occupied_{false} {};


//---------------------------------------------------------------------------------------------------------------------
Tile::~Tile()
{
  character_.reset();
}

///---------------------------------------------------------------------------------------------------------------------
CharacterType Tile::getCharacterType() const
{
  if (character_ != nullptr)
    return character_->getCharacterType();
  else
    return CharacterType::NoCharacter;
} 

///---------------------------------------------------------------------------------------------------------------------
void Tile::swapCharacter(Tile* other_tile)
{
  //should work
  this->character_.swap(other_tile->character_);
}

///---------------------------------------------------------------------------------------------------------------------
void Tile::setCharacter(std::unique_ptr<Character> character)
{
  character_ = std::move(character);
}


//enum TileType {W, P, X, M, G, C, H, V, L, T, F, S};

///---------------------------------------------------------------------------------------------------------------------
///representation for a tile in an unrevealed room
std::array<std::string, 3> Tile::unknown_representation_{"UUUUUUU", "UUUUUUU", "UUUUUUU"};

///---------------------------------------------------------------------------------------------------------------------
void Tile::tileCharModifier(std::array<std::string, 3> &representation, CharacterType character_type)
{
  switch (character_type)
  {
  case CharacterType::NoCharacter:
    return;
    break;
  case CharacterType::FighterCharacter:
    //remember tile is 7 char wide, middle position is 4th letter -> index 3
    representation[1].at(3) = 'F';
    break;
  case CharacterType::ThiefCharacter:
    representation[1].at(3) = 'T';
    break;
  case CharacterType::SeerCharacter:
    representation[1].at(3) = 'S';
    break;
  }
}

///---------------------------------------------------------------------------------------------------------------------
std::array<std::string, 3> const Tile::getTileString(const bool revealed) 
{
  if (revealed == false)
  {
    return unknown_representation_;
  }
  else 
  {
    return this->getTileString();
  }
}
///---------------------------------------------------------------------------------------------------------------------
bool Tile::isWalkable() const
{
  return walkable_;
}

///---------------------------------------------------------------------------------------------------------------------
void Tile::setOccupied(bool occupied)
{
  occupied_ = occupied;
};

///---------------------------------------------------------------------------------------------------------------------
bool Tile::isOccupied() const
{
  return occupied_;
}

///---------------------------------------------------------------------------------------------------------------------
void Tile::setInactive() 
{
  active_ = false;
}

///---------------------------------------------------------------------------------------------------------------------
char Tile::getTileId() const
{
  return tile_id_;
}

///---------------------------------------------------------------------------------------------------------------------
bool Tile::getActive() const
{
  return active_;
}  