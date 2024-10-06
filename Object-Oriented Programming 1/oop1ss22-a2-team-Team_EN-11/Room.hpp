//---------------------------------------------------------------------------------------------------------------------
// Room.hpp
//
// Base class for rooms
// 
//
// Author: 11942956, 11812041, 12130995
//---------------------------------------------------------------------------------------------------------------------
//
#ifndef ROOM_HPP
#define ROOM_HPP

#include <string>
#include <iostream>
#include <memory>
#include "Tile.hpp"
#include "BasicTile.hpp"
#include "MagicTile.hpp"
#include "GameException.hpp"



class Room
{
private:
  char room_id_;
  std::array<std::array<Tile*,5>,5> room_map_;
  bool revealed_;
  bool monster_;


public:
  //---------------------------------------------------------------------------------------------------------------------
  /// constructor
  Room(char room_id, std::string room_info_str);

  //---------------------------------------------------------------------------------------------------------------------
  /// destructor
  ~Room();

  //---------------------------------------------------------------------------------------------------------------------
  /// Copy constructor
  Room(const Room &none) = delete;

  //---------------------------------------------------------------------------------------------------------------------
  /// Copy Assignment constructor
  Room& operator=(Room &none) = delete;

  //---------------------------------------------------------------------------------------------------------------------
  /// Tile assignment
  Tile* assignTile(char tile_char, int i, int ii);


  //---------------------------------------------------------------------------------------------------------------------
  /// Getter for room_id_
  /// Not sure yet if we need one
  char getRoomID() const {return room_id_;}

  //---------------------------------------------------------------------------------------------------------------------
  /// Getter for room_map_
  /// returns tile pointer array of size 5x5
  std::array<std::array<Tile*,5>,5> getRoomMap();

  //---------------------------------------------------------------------------------------------------------------------
  /// overloaded Getter for room_map_
  /// when given an integer it returns just the Tile* for the tile appointed by the integer in a flattened array
  Tile* getRoomMap(int index_tile_flat);

  //---------------------------------------------------------------------------------------------------------------------
  /// Getter for revealed_ status
  bool getRevealed() const {return revealed_;}

  //---------------------------------------------------------------------------------------------------------------------
  /// Setter for revealed_
  /// sets revealed_ true
  void setRevealed() {revealed_ = true;}

  //---------------------------------------------------------------------------------------------------------------------
  /// Getter for if a monster is active in a room
  bool getMonster() const {return monster_;}

  //---------------------------------------------------------------------------------------------------------------------
  /// Setter for if a monster is active in a room
  void setMonster(bool active) {monster_ = active;}

  //---------------------------------------------------------------------------------------------------------------------
  /// checks room for active monster and returns truth value accordingly
  bool checkMonster();
};







#endif