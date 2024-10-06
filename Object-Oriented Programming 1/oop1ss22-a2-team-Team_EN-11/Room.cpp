//---------------------------------------------------------------------------------------------------------------------
// Room.cpp
//
// Base class for rooms
// 
//
// Author: 11942956, 11812041, 12130995
//---------------------------------------------------------------------------------------------------------------------
//

#include "Room.hpp"

//---------------------------------------------------------------------------------------------------------------------
Room::Room(char room_id, std::string room_info_str)
  : room_id_{room_id}, room_map_(), revealed_{false}, monster_{false}
{
  //needs to create the room map from alphanumeric characters to tile pointers

  char tile_char;
  if (room_info_str.empty())
  {
    //I don't think we need an exception here, but probably useful for troubleshooting right now
    //std::cout << "Room is empty" <<std::endl;
    throw InvalidConfig("Error: Invalid configuration!\n");
  }

  for (int i = 0; i < 5; i++)
  {
    for (int ii = 0; ii < 5; ii++)
    {
      tile_char = room_info_str.at(i+ii*5);
      room_map_[i][ii] = assignTile(tile_char, i, ii);
      if (tile_char == 'M')
      {
        setMonster(true);
      }
    }
  }
  
}

//---------------------------------------------------------------------------------------------------------------------
Room::~Room()
{
  for (auto room_array : room_map_)
  {
    for(auto room: room_array)
      delete room;
  }
}



//---------------------------------------------------------------------------------------------------------------------
Tile* Room::assignTile(char tile_char, int i, int ii)
{
  //wanted to do a switch case with TileType here but it probably is less efficient than just a lot of if statements
  if (tile_char == 'W')
  {
    if ((i < 1) && (ii < 1))
      {
        return new Wall(getRoomID());
      }
    return new Wall();
  }
  else if (tile_char == 'P')
    return new Passage();
  else if (tile_char == 'X')
    return new HiddenDoor();
  else if (tile_char == 'M')
    return new Monster();
  else if (tile_char == 'G')
    return new HourGlass();
  else if (tile_char == 'C')
    return new Crystal();
  else if (tile_char == 'H')
    return new HorDoor();
  else if (tile_char == 'V')
    return new VerDoor();
  else if (tile_char == 'L')
    return new Loot();
  else if (tile_char == 'T')
    return new ButtonThief();
  else if (tile_char == 'F')
    return new ButtonFighter();
  else if (tile_char == 'S')
    return new ButtonSeer();
  else
  {
    std::cout << "Tile assignment problem" <<std::endl;
    //maybe throw an error instead
    return nullptr;
  }
}

//---------------------------------------------------------------------------------------------------------------------
std::array<std::array<Tile*,5>,5> Room::getRoomMap() 
{
  return room_map_;
}

//---------------------------------------------------------------------------------------------------------------------
Tile* Room::getRoomMap(int index_tile_flat)
{
  int row;
  int column;
  row = index_tile_flat / 5;
  column = index_tile_flat % 5;
  //number of rows, number of columns not index in row column
  return room_map_[column][row];
}

//---------------------------------------------------------------------------------------------------------------------
bool Room::checkMonster()
{
  auto tiles_map = getRoomMap();
  for (auto tiles_it_first : tiles_map)
  {
    for (Tile* tiles_it_second : tiles_it_first)
    {
      if (tiles_it_second->getTileId() == 'M' && tiles_it_second->getActive() == true)
      {
        return true;
      }
    }
  }
  return false;
}