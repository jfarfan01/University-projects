//---------------------------------------------------------------------------------------------------------------------
// MovesMaster.cpp
//
// Base class for movement cards andhandling movement
//
//
// Author: 11942956, 11812041, 12130995
//---------------------------------------------------------------------------------------------------------------------
//

#include "MovesMaster.hpp"

#include <iostream>
MovesMaster::MovesMaster()
{
  setFlips(0);
  constructList();
}

//---------------------------------------------------------------------------------------------------------------------
// Setter for Flips
void MovesMaster::setFlips(int new_amount)
{
  MovesMaster::flips_ = new_amount;
}

//---------------------------------------------------------------------------------------------------------------------
// Getter for Flips
int MovesMaster::getFlips()
{
  return flips_;
}

//---------------------------------------------------------------------------------------------------------------------
// Iterator for Flips
void MovesMaster::iterateFlips()
{
  int old_flips = MovesMaster::getFlips();
  old_flips++;
  MovesMaster::setFlips(old_flips);
}
//---------------------------------------------------------------------------------------------------------------------
// This will adjust the list as required based on the flips
void MovesMaster::flipMoveList(std::vector<std::string> & moves_)
{  
  std::string element = moves_.front();
  moves_.push_back(element);
  moves_.erase(moves_.begin());
}

//---------------------------------------------------------------------------------------------------------------------
///
void MovesMaster::iterateMap(int move_to_be_placed, std::map<int, std::string>& BaseList)
{
  BaseList.erase(move_to_be_placed);

  std::map<int, std::string>::iterator iterator = BaseList.begin();
  std::vector<std::pair<int, std::string>> elements;
  while (iterator != BaseList.end())
  {
    int index = iterator->first;
    std::string content = iterator->second;
    if (index != move_to_be_placed)
    {
      if (index >= move_to_be_placed)
      {
        index--;
      }
      elements.push_back(std::make_pair(index, content));
    }
    ++iterator;
  }
  int elements_size = elements.size();
  BaseList.clear();
  for (int i = 0; i < elements_size; i++)
  {
    BaseList.emplace(elements.at(i));
  }
}

//---------------------------------------------------------------------------------------------------------------------
///
void MovesMaster::constructList()
{
  std::map<int, std::string> BaseList;
  BaseList.emplace(1, "up");
  BaseList.emplace(2, "right");
  BaseList.emplace(3, "down");
  BaseList.emplace(4, "left");

  for (size_t i = 0; i <= 3; i++)

  {
    int number_cards_left = 4 - i;

    if (number_cards_left != 1)
    {
      int move_to_be_placed = Oop::Random::getInstance().getRandomCard(number_cards_left);
      auto iterator = BaseList.find(move_to_be_placed);
      if (iterator == BaseList.end())
      {
        std::cout << "Iterator is having issues, more than one card left" << std::endl;
      }
      else
      {
        moves_.push_back(iterator->second);
      }
      iterateMap(move_to_be_placed, BaseList);
    }
    else
    {
      auto iterator = BaseList.find(1);
      if (iterator == BaseList.end())
      {
        std::cout << "Iterator is having issues and one card is left" << std::endl;
      }
      else
      {
        MovesMaster::moves_.push_back(iterator->second);
      }
    }
  }
}
//---------------------------------------------------------------------------------------------------------------------
bool MovesMaster::isMovable(CharacterType character_type, Tile* tile, int steps)
{
  if (!tile->isWalkable())
  {
    characterPrinter(character_type);
    std::cout << ": \"My way is blocked!\"" << std::endl;
    return false;
  }
  if (tile->isOccupied())
  {
    //steps suppresses the error message so it doesn't happen twice
    if (steps < 2)
    {
      characterPrinter(character_type);
      std::cout << ": \"There is not enough space on that tile!\"" << std::endl;
    }
    return false;
  }
  return true;
}
//---------------------------------------------------------------------------------------------------------------------
Room* MovesMaster::getNeighbouringRoom(int room_index, Direction direction, Game& game)
{
  int cols = game.getColumns();
  int rows = game.getRows();
  // std::cout << room_index << std::endl;
  int room_col = (room_index % cols) + (direction % 5);
  int room_row = static_cast<int>(room_index / cols) + static_cast<int>(direction / 5);
  // std::cout << "neighbour at " << room_col << ":" << room_row << std::endl;
  //  std::cout << room_col << ":" << room_row << std::endl;
  if (room_col < 0 || room_col >= cols || room_row < 0 || room_row >= rows)
  {
    // std::cout << "mhh?" << std::endl;
    return nullptr;
  }

  return game.getRooms().at(room_col + (room_row * cols));
}
//---------------------------------------------------------------------------------------------------------------------
bool MovesMaster::handleNewRoom(int x, int y, std::pair<CharacterType, std::pair<Direction, int>> command,
                                int room_index, Game& game, std::array<std::array<Tile*, 5>, 5> tiles,
                                Room* neighbour_room, int old_x, int old_y, MovesMaster& moves, int steps,
                                int flat_coord, int flat_tile)
{

  int new_x = (x + (command.second.first % 5)) % 5;
  int new_y = ((y + static_cast<int>(command.second.first / 5))) % 5;
  if (new_x < 0)
    new_x += 5;
  if (new_y < 0)
    new_y += 5;

  int room_col = (room_index % game.getColumns()) + (command.second.first % 5);
  int room_row = static_cast<int>(room_index / game.getColumns()) + static_cast<int>(command.second.first / 5);

  int new_flat_room = room_col + (room_row * game.getColumns());
  int new_flat_tile = static_cast<ulong>(new_x + (new_y * 5));

  std::array<std::array<Tile*, 5>, 5> new_tiles = neighbour_room->getRoomMap();
  if (!(isMovable(command.first, new_tiles[new_x][new_y], 0)))
    return false;
  new_tiles[new_x][new_y]->swapCharacter(tiles[old_x][old_y]);
  tiles[old_x][old_y]->setOccupied(false);
  new_tiles[new_x][new_y]->setOccupied(true);
  game.setCharacterPos(command.first, new_flat_room, new_flat_tile);
  if (steps - 1 == 0)
    return true;
  auto new_command = std::make_pair(command.first, std::make_pair(command.second.first, steps - 1));
  if (handleCommand(new_command, game, moves))
    return true;
  new_tiles[new_x][new_y]->swapCharacter(tiles[old_x][old_y]);
  tiles[old_x][old_y]->setOccupied(true);
  new_tiles[new_x][new_y]->setOccupied(false);

  game.setCharacterPos(command.first, flat_coord, flat_tile);
  return false;
}

//---------------------------------------------------------------------------------------------------------------------
// THIS IS WHERE MOVEMENT IS FULLY HANDLED
//---------------------------------------------------------------------------------------------------------------------
bool MovesMaster::handleCommand(std::pair<CharacterType, std::pair<Direction, int>> command, Game& game,
                                MovesMaster& moves)
{
  std::vector<Room*> rooms = game.getRooms();
  auto char_pos = game.getCharacterPos()[command.first];
  Direction dir = command.second.first;

  int flat_coord = char_pos.at(1);
  int flat_tile = char_pos.at(2);

  char room_id = rooms[char_pos.at(1)]->getRoomID();
  int room_index = 0;
  for (auto& room : rooms)
  {
    if (room->getRoomID() == room_id)
    {
      int x = flat_tile % 5;
      int y = flat_tile / 5;
      int old_x = x;
      int old_y = y;

      int steps = command.second.second;

      std::array<std::array<Tile*, 5>, 5> tiles = room->getRoomMap();
      int new_x = x + command.second.second * (command.second.first % 5);
      int new_y = y + command.second.second * static_cast<int>(command.second.first / 5);

      // check for room change
      bool room_change = (new_x < 0 || new_y < 0 || new_x >= 5 || new_y >= 5);

      // check if path in current room is possible
      if (!pathing(command.first, tiles, steps, x, y, dir))
      {
        return false;
      }

      Room* neighbour_room = getNeighbouringRoom(room_index, command.second.first, game);
      if (room_change && !neighbour_room->getRevealed())
      {
        characterPrinter(command.first);
        std::cout << ": \"My way is blocked!\"" << std::endl;
        return false;
      }
      if (((x == 0 && dir == LEFT) || (x == 4 && dir == RIGHT) || (y == 0 && dir == UP) || (y == 4 && dir == DOWN)) &&
          neighbour_room != nullptr)
        neighbour_room->setRevealed();

      if (neighbour_room != nullptr && room_change && neighbour_room->getMonster())
      {
        if (command.first != CharacterType::FighterCharacter)
        {
          characterPrinter(command.first);
          std::cout << ": \"That room is too scary for me!\"" << std::endl;
          return false;
        }
      }

      if (room_change && neighbour_room == nullptr)
        return false;

      ulong new_flat_room = flat_coord;
      ulong new_flat_tile = static_cast<ulong>(x + (y * 5));

      if (room_change)
        return handleNewRoom(x, y, command, room_index, game, tiles, neighbour_room, old_x, old_y, moves, steps,
                             flat_coord, flat_tile);

      tiles[old_x][old_y]->swapCharacter(tiles[x][y]);
      tiles[x][y]->setOccupied(true);
      tiles[old_x][old_y]->setOccupied(false);
      game.setCharacterPos(command.first, new_flat_room, new_flat_tile);

      return true;
    }
    room_index++;
  }
  return false;
}
// update pos (only to first field in new room
//-> recursive function call with reduced steps)
//-> on fail restore old position

//---------------------------------------------------------------------------------------------------------------------
///
void MovesMaster::characterPrinter(CharacterType character)
{
  if (character == CharacterType::FighterCharacter)
  {
    std::cout << "Fighter";
  }
  else if (character == CharacterType::ThiefCharacter)
  {
    std::cout << "Thief";
  }

  else if (character == CharacterType::SeerCharacter)
  {
    std::cout << "Seer";
  }
}

std::vector<std::string>& getMoves();


//---------------------------------------------------------------------------------------------------------------------
///checks for availability of movement
bool MovesMaster::pathing(CharacterType char_type, 
                          std::array<std::array<Tile *, 5UL>, 5UL> tile_map, int& steps, int& x, int& y, Direction dir)
{
  // check if path in current room is possible
  do
  {
    if (steps > 0)
    {
      if (((x == 0 && dir == LEFT) || (x == 4 && dir == RIGHT) || (y == 0 && dir == UP) || (y == 4 && dir == DOWN)))
        break;
      x += (dir % 5);
      y += static_cast<int>(dir / 5);
      if (!(isMovable(char_type, tile_map[x][y], steps)))
      {
        //test if you can jump over the tile
        if (steps < 2)
        {
          return false;
        }
        if(!(!(steps > 1) || !tile_map[x][y]->isJumpable()))
        {
          characterPrinter(char_type);
          std::cout << ": \"There is not enough space on that tile!\"" << std::endl;
          return false;
        }
        //check tile behind
        
      }
      steps--;
    }
  } while (!(x < 0 || y < 0 || x > 4 || y > 4) && steps > 0);

  return true;
}


//---------------------------------------------------------------------------------------------------------------------
bool MovesMaster::hourGlassFlipReducer(Game& game, CharacterType character_type)
{
  auto  char_pos = game.getCharacterPos()[character_type];
  Room* room = game.getRooms()[char_pos[1]];
  Tile* tile = room->getRoomMap(char_pos[2]);

  if (tile->getTileId() != 'G')
  {
    return false;
  }

  int old_flips = flips_;
  old_flips -= 5;

  if(old_flips < 0)
  {
    flips_ = 0;
  }

  else
  {
    flips_ = old_flips;
  }

  tile->setInactive();
  return true;
}
