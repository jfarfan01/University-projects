//---------------------------------------------------------------------------------------------------------------------
// Tile.hpp
//
// Base class for tiles
// 
//
// Author: 11942956, 11812041, 12130995
//---------------------------------------------------------------------------------------------------------------------
//


#ifndef TILE_HPP
#define TILE_HPP

#include <vector>
#include <array>
#include <string>
#include "Character.hpp"
#include <iostream>
#include <memory>


class Tile
{
private:
  char tile_id_;
//check if some of these should be private
protected:
  std::unique_ptr<Character> character_; //character is the player figure currently on the tile (at most one character per tile allowed)
  bool walkable_; //can a character end the turn on this tile (beware of edgecases, might need another implementation)
  bool jumpable_; //can a character pass over the tile without it being the endpoint
  bool active_; //can be interacted with or not, after magicUse or similar (open door) -> false
  bool occupied_; //when attempting to walk, will tell you if a character is currently there or not
  static std::array<std::string, 3> unknown_representation_; //representation if tile is unknown

  //---------------------------------------------------------------------------------------------------------------------
  /// Changes the tile representation to one with a character in the middle
  /// might be moved to Tile.hpp instead
  void tileCharModifier(std::array<std::string,3> &representation, CharacterType character_type);

public:

  //---------------------------------------------------------------------------------------------------------------------
  /// Constructor
  /// Tiles are always constructed with a nullptr for a character, do the assignment after construction
  Tile(bool walkable, bool jumpable, bool active, char id);

  //---------------------------------------------------------------------------------------------------------------------
  /// Deconstructor
  /// implement as character_.reset(), hope that suffices
  virtual ~Tile();

  //---------------------------------------------------------------------------------------------------------------------
  /// Copy Constructor
  Tile(const Tile &none) = delete;
  //---------------------------------------------------------------------------------------------------------------------
  /// Copy Assingment constructor
  Tile& operator=(Tile &none) = delete;

  //---------------------------------------------------------------------------------------------------------------------
  /// returns ASCII representation of tile
  /// automatically prints the character char when needed
  virtual std::array<std::string,3> const getTileString() = 0;

  //---------------------------------------------------------------------------------------------------------------------
  /// overload of getTileString with bool for revealed
  /// returns ASCII representation of tile
  std::array<std::string, 3> const getTileString(bool revealed);

  //---------------------------------------------------------------------------------------------------------------------
  /// returns the character type if character isn't a nullptr, else it returns the default 0
  CharacterType getCharacterType() const;

  //---------------------------------------------------------------------------------------------------------------------
  /// swaps unique character pointers (likely to be at least one nullptr) between tiles
  void swapCharacter(Tile* other_tile);

  //---------------------------------------------------------------------------------------------------------------------
  /// sets the character_ pointer
  /// couldn't quickly get template to work, only 2 extra overloads though
  void setCharacter(std::unique_ptr<Character> character);
  
  //---------------------------------------------------------------------------------------------------------------------
  /// getter for whether something walkable or not
  bool isWalkable() const;

  //---------------------------------------------------------------------------------------------------------------------
  /// Setter for whether something walkable or not
  void setWalkable() {walkable_ = true;}

  //---------------------------------------------------------------------------------------------------------------------
  /// getter for whether something is jumpable or not
  bool isJumpable() const {return jumpable_;}

  //---------------------------------------------------------------------------------------------------------------------
  /// Setter for to make something jumpable
  void setJumpable() {jumpable_ = true;}

  //---------------------------------------------------------------------------------------------------------------------
  /// getter for occupied_
  bool isOccupied() const;
  
  //---------------------------------------------------------------------------------------------------------------------
  /// sets occupied to true/false
  void setOccupied(const bool occupied);

  //---------------------------------------------------------------------------------------------------------------------
  /// might need a setActive(bool) instead for checking the win condition
  /// sets active_ to false
  void setInactive();

  //---------------------------------------------------------------------------------------------------------------------
  /// returns if tile is active
  bool getActive() const;

  //---------------------------------------------------------------------------------------------------------------------
  /// returns tile id
  char getTileId() const;


};







#endif