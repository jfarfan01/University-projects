//---------------------------------------------------------------------------------------------------------------------
// MovesMaster.hpp
//
// Base class for movement cards and  handling movement
//
//
// Author: 11942956, 11812041, 12130995
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef MOVELIST_HPP
#define MOVELIST_HPP
#include "Character.hpp"
#include "Game.hpp"
#include "Random.hpp"
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

enum Direction
{
  UP = -5,
  DOWN = 5,
  LEFT = -1,
  RIGHT = 1,
  WRONG,
  WHO,
  WHERE
};

class MovesMaster
{
  // Flips is private for now
  // This will eventually be used to track the flips from the flip counter
private:
  int flips_;

  //---------------------------------------------------------------------------------------------------------------------
  /// checks if a path is possible
  bool pathing(CharacterType char_type, std::array<std::array<Tile *, 5UL>, 5UL> tile_map, 
                int& steps, int& x, int& y, Direction dir);

public:
  // Setter for Flips
  void setFlips(int new_amount);
  // Getter for Flips
  int getFlips();
  // Iterator for Flips
  void iterateFlips();

  // Hallelujah! Our list of moves, in a random order, sorted as needed
  std::vector<std::string> moves_;

  // This will shift the list as is appropriate re: flips_
  void flipMoveList(std::vector<std::string>& moves_);

  // This is the constructor for the movement list
  MovesMaster();

  // Destructor
  ~MovesMaster() = default;

  // This essentially shifts a map's key values backwards from a number, so as to "remove" said value
  // from the pool and move all following values up.
  void iterateMap(int move_to_be_placed, std::map<int, std::string>& BaseList);
  // This constructs the moves list via the Random function;
  // I could eventually add a number to creation, in the event that we want to make a restricted
  // movelist, or a movelist with a pool of more than one move (say, 5 lefts, but 1 up, etc.)
  void constructList();

  //---------------------------------------------------------------------------------------------------------------------
  ///
  bool handleCommand(std::pair<CharacterType, std::pair<Direction, int>> command, Game& game, MovesMaster& moves);
  //---------------------------------------------------------------------------------------------------------------------
  ///
  std::pair<CharacterType, std::pair<Direction, int>> parseInput(std::string input);
  //---------------------------------------------------------------------------------------------------------------------
  ///
  void handleInput(std::string input, Game& game, MovesMaster& moves);

  //---------------------------------------------------------------------------------------------------------------------
  /// prints character string (name)
  void characterPrinter(CharacterType type);

  //---------------------------------------------------------------------------------------------------------------------
  /// checks if a tile is possible for movement
  /// jump_suppress only suppresses the There is no space there message
  bool isMovable(CharacterType type, Tile* tile, int steps);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  Room* getNeighbouringRoom(int room_index, Direction direction, Game& game);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  bool handleNewRoom(int x, int y, std::pair<CharacterType, std::pair<Direction, int>> command, int room_index,
                     Game& game, std::array<std::array<Tile*, 5>, 5> tiles, Room* neighbour_room, int old_x, int old_y,
                     MovesMaster& moves, int steps, int flat_coord, int flat_tile);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  bool hourGlassFlipReducer(Game& game, CharacterType character_type);

};

#endif