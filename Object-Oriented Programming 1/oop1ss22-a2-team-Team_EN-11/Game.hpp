//---------------------------------------------------------------------------------------------------------------------
// Game.hpp
//
// Definition of singleton class game
// 
//
// Author: 11942956, 11812041, 12130995
//---------------------------------------------------------------------------------------------------------------------
//
#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <vector>
#include <sstream>
#include <memory>
#include <map>


#include "RoomInfo.hpp"
#include "Room.hpp"
#include "GameException.hpp"
#include "Character.hpp"

enum DirectionPrimitive
{
  UP_P = -1,
  DOWN_P = +1,
  LEFT_P = -2,
  RIGHT_P = +2,
  NONE_P = 0
};

class Game
{
private:
  //---------------------------------------------------------------------------------------------------------------------
  ///vector of rooms defined in constructor
  std::vector<Room*> rooms_;

  //---------------------------------------------------------------------------------------------------------------------
  ///number of rooms in a row/column
  ///mainly needed for verifying config and also for printing the ASCII map
  int room_num_[2];

  //---------------------------------------------------------------------------------------------------------------------
  ///positions of characters [character][room(flattened)][tile(flattened)]
  std::map<CharacterType, std::array<ulong, 3>> character_pos_;

  //---------------------------------------------------------------------------------------------------------------------
  ///bool to avoid having a game set more than once
  bool game_set_;

  //---------------------------------------------------------------------------------------------------------------------
  ///constructors have to be private for singleton class/
  ///has to create roomplan and initialize character positions
  ///needs to parse and verify the command for building the roomplan (square matrix! N x N)
  ///not sure how many different constructors are needed
  Game() : game_set_{false} {};


  //---------------------------------------------------------------------------------------------------------------------
  ///Verification method for config
  ///1: only capital letters and spaces
  ///2: no other command-line inputs
  ///3: At least one row 
  ///4: 0 < row size < 6 [1, 2, 3, 4, 5]
  ///5: every row has to have the same amount of columns
  ///6: every room may apppear once only
  ///7: There has to be a start ('S') room in the configuration
  bool verification(const std::string config_str);

  

public:
  //---------------------------------------------------------------------------------------------------------------------
  ///destructor
  ///has to clear vector with rooms 
  ~Game();

  //---------------------------------------------------------------------------------------------------------------------
  ///only one instance so no copy operator
  Game(const Game&) = delete;

  //---------------------------------------------------------------------------------------------------------------------
  ///only one instance so no assignment/copy operator
  Game operator=(const Game&) = delete;


  //---------------------------------------------------------------------------------------------------------------------
  ///not sure if we should load this like this for initialization
  ///only returns the same instance every time
  static Game& instance();

  //---------------------------------------------------------------------------------------------------------------------
  ///would be the constructor in a normal class
  ///verifies and sets up game
  void setInstance(std::string input);

  //---------------------------------------------------------------------------------------------------------------------
  /// prints the representation of the floor of a room (thus needs amount of rooms in row)
  void roomFloorPrinter(int rooms_per_row) const;

  //---------------------------------------------------------------------------------------------------------------------
  /// returns the vector with rooms
  std::vector<Room*> getRooms();

  //---------------------------------------------------------------------------------------------------------------------
  ///returns the position of the character
  std::map<CharacterType, std::array<ulong, 3>> getCharacterPos() const;

  //---------------------------------------------------------------------------------------------------------------------
  ///sets the position of the character
  void setCharacterPos(CharacterType type, ulong new_x, ulong new_y);

  //---------------------------------------------------------------------------------------------------------------------
  /// prints the playing board/map
  void boardPrinter() const;
  
  //---------------------------------------------------------------------------------------------------------------------
  ///Returns rooms per row    
  int getColumns() const;
  //returns total number of rows 
  int getRows() const;


  //---------------------------------------------------------------------------------------------------------------------
  ///activates hidden doors (well deactivates from the program standpoint)
  void hiddenDoorActivation();
  
  ///Thief
  //---------------------------------------------------------------------------------------------------------------------
  //unlocks all doors horizontally or vertically adjacent to the thief
  //returns 0 if all good and 1 if failed
  int unlock();


  ///Fighter
  //---------------------------------------------------------------------------------------------------------------------
  //fights all monsters horizontally or vertically adjacent to the thief
  //returns 0 if all good and 1 if failed
  int fight();

  ///Seer
  //---------------------------------------------------------------------------------------------------------------------
  //reveals one room on the map
  //returns 0 if all good and 1 if failed
  int scry(char room_id, DirectionPrimitive direction_rel);

  std::vector<std::vector<int>> bfsArrayGenerator();


private:
  //---------------------------------------------------------------------------------------------------------------------
  //method used in unlock and fight
  int specialAction(CharacterType character_typ, std::string tile_ids);
  //---------------------------------------------------------------------------------------------------------------------
  //helping method for scry
  //returns the vector position of the room to be revealed
  int scry_position(std::vector<Room *>room_vector, DirectionPrimitive direction_rel, ulong index_current_room);


  //---------------------------------------------------------------------------------------------------------------------
  ///for string to check multiple ids (single char deprecated)
  ///checks if a tile has the id of tile_id and sets it inactive
  ///returns true if successfull
  bool turn_tile(Room* target_room, std::string tile_ids, int target_row, int target_column);

  //---------------------------------------------------------------------------------------------------------------------
  //helper for unlock and fight to find the target tile
  void adjacentTile(int current_room_row_column[3], int target_direction[2], bool& success, std::string ids);

};




#endif