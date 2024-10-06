//---------------------------------------------------------------------------------------------------------------------
// Game.cpp
//
// Implementation of singleton class game
// Not sure if we need a different main.cpp as well to run in, probably yes
//
// Author: 11812041, 11942956, 12130995
//---------------------------------------------------------------------------------------------------------------------
//
#include "Game.hpp"



//---------------------------------------------------------------------------------------------------------------------
Game::~Game()
{
  for (auto room : rooms_)
  {
    delete room;
  }
}


//---------------------------------------------------------------------------------------------------------------------
Game& Game::instance()
{
  //I have a rough idea now
  static Game instance_;
  return instance_;
}


//---------------------------------------------------------------------------------------------------------------------
///Sets up game and verifies
void Game::setInstance(std::string input)
{
  if (game_set_)
  {
    //cannot set a game twice
    return;
  } 
  //need to read the option input/config_string

  verification(input);

  std::string parsed;

  parsed = input.substr(0, room_num_[0]);
  if (room_num_[1] > 1)
  {
    //not sure on the +1 here -> SHOULD BE WORKING
    for (int i = 1; i < room_num_[1]; i++)
    {
      parsed += input.substr(i*(room_num_[0]+1), room_num_[0]);
    }
  }

  //check for duplicates
  for (char room_id : parsed)
  {
    if (parsed.find(room_id)!= parsed.find_last_of(room_id))
    {
      //std::cout << "multiple same rooms" << std::endl;
      throw InvalidConfig("Error: Invalid configuration!\n");
    }
  }

  for (char room_id : parsed)
  {
    std::string room_config = Oop::RoomInfo::getInstance().getRoomInfoString(room_id);
    if (room_config.empty())
    {
      //std::cout << "no room_config" << std::endl;
      throw InvalidConfig("Error: Invalid configuration!\n");
    }
    rooms_.push_back(new Room(room_id, room_config));
    rooms_.back()->setMonster(rooms_.back()->checkMonster());
    //set characters
    if (room_id == 'S')
    {
      std::array<std::array<Tile*, 5>, 5> map_temp = rooms_.back()->getRoomMap();
      //rooms_.size()-1 because array index (starts 0) vs length (starts 1 for non empty)
      //in the array: row-length, column-length
      std::unique_ptr<Character> Fighter_ptr (new Character(CharacterType::FighterCharacter));
      map_temp[1][1]->setCharacter(std::move(Fighter_ptr));
      map_temp[1][1]->setOccupied(true);
      character_pos_[FighterCharacter] = {CharacterType::FighterCharacter, rooms_.size()-1, 6};

      std::unique_ptr<Character> Thief_ptr (new Character(CharacterType::ThiefCharacter));
      map_temp[3][1]->setCharacter(std::move(Thief_ptr));
      map_temp[3][1]->setOccupied(true);
      character_pos_[ThiefCharacter] = {CharacterType::ThiefCharacter, rooms_.size()-1, 8};

      std::unique_ptr<Character> Seer_ptr (new Character(CharacterType::SeerCharacter));
      map_temp[1][3]->setCharacter(std::move(Seer_ptr));
      map_temp[1][3]->setOccupied(true);
      character_pos_[SeerCharacter] = {CharacterType::SeerCharacter, rooms_.size()-1, 16};

      rooms_.back()->setRevealed();
    }
  }


  game_set_ = true;
}

//---------------------------------------------------------------------------------------------------------------------
///Verification method for config
///1: only capital letters and spaces
///2: no other command-line inputs -> not sure what that means currently
///3: At least one row 
///4: 0 < row size < 6 [1, 2, 3, 4, 5]
///5: every row has to have the same amount of columns
///6: every room may apppear once only -> do when setting up the room vector
///7: There has to be a start ('S') room in the configuration
bool Game::verification(std::string config_str)
{
  //std::cout << config_str << std::endl;

  // ad 3
  if (config_str.empty())
  {
    //std::cout << "string is empty" << std::endl;
    throw InvalidConfig("Error: Invalid configuration!\n");
  }
    
  // ad 4
  if (config_str[0] == '0')
  {
    //std::cout << "first pos 0" << std::endl;
    throw InvalidConfig("Error: Invalid configuration!\n");
  }

  // ad 1
  for (char character_config : config_str)
  {
    if (((character_config > 64) && (character_config < 91)) || (character_config == 32))
    {
      continue;
    }
    else
    {
      //std::cout << "wrong characters in config" << std::endl;
      throw InvalidConfig("Error: Invalid configuration!\n");
    }
  }

  // ad 7
  if (config_str.find('S') == config_str.npos)
  {
    //std::cout << "no S found" << std::endl;
    throw InvalidConfig("Error: Invalid configuration!\n");
  }

  int spacing;
  //npos is (should be) -1 for size_t object
  spacing = config_str.find(' ') + 1;

  //this should actually be valid, since there doesn't have to be a space for a single row
  //instead checking if the first letter is space in the beginning and then waving this case through
  if (spacing == 0)
  {
    //ad 4
    if (config_str.length() > 5)
    {
      //std::cout << "too long" << std::endl;
      throw InvalidConfig("Error: Invalid configuration!\n");
    }

    room_num_[0] = config_str.length();
    //std::cout << "only one row" << room_num_[0] << std::endl;
    room_num_[1] = 1;
    return true;
  }

  //I bet this has a one off error
  // ad 4
  if (((config_str.length()+1) / spacing) > 5)
  {
    //std::cout << "too many rows" << std::endl;
    throw InvalidConfig("Error: Invalid configuration!\n");
  }

  // ad 5
  // check for one off error in regards of range and spacing
  int pos_comparison = (config_str.length()-spacing);
  for (int pos = spacing - 1; (pos < pos_comparison); pos += spacing)
  {
    if (config_str[pos] != ' ')
    {
      //std::cout << "not consistent rows" << std::endl;
      throw InvalidConfig("Error: Invalid configuration!\n");
    }
  }

  //set number of columns
  room_num_[0] = spacing - 1;
  //set number of rows
  room_num_[1] = (config_str.length()+1)/(spacing);
  return true;
}

void Game::roomFloorPrinter(int rooms_per_row) const
{
    std::cout<<"╬═══════════════════════════════════╬";
  for (int i = 0;i < rooms_per_row - 1; i++)
  {
    std::cout<< "═══════════════════════════════════╬";
  }
  std::cout<<std::endl;
}

void Game::boardPrinter() const
{
  int rooms_per_row = room_num_[0];
  int number_of_rows;
  if (room_num_[1] != 1)
  {
    number_of_rows = room_num_[1];
  }
  else
  {
    number_of_rows = room_num_[1];
  }



  roomFloorPrinter(rooms_per_row);

  for (int row = 0; row < number_of_rows; row++)
  {
    for (int room_row = 0; room_row < 5; room_row++)
    {
      for (int line = 0; line < 3; line++)
      {
        for (int column = 0; column < rooms_per_row; column++)
        {
          std::cout << "║";
          for (int room_col = 0; room_col < 5; room_col++)
          {
            Room* room = rooms_.at(column + (row * rooms_per_row));
            std::array<std::array<Tile*, 5>, 5> room_map = room->getRoomMap();
            //current workaround, probably as permanent as it gets
            if (room->getRevealed() == true)
            {
              std::cout << room_map.at(room_col).at(room_row)->getTileString().at(line);
            }
            else
            {
              std::cout << room_map.at(room_col).at(room_row)->getTileString(false).at(line);
            }
          }
        }
        std::cout<< "║" << std::endl;
      }
    }
    roomFloorPrinter(rooms_per_row);
  }
}

//---------------------------------------------------------------------------------------------------------------------
///
int Game::getColumns() const
{
  return room_num_[0];
}

//---------------------------------------------------------------------------------------------------------------------
///
int Game::getRows() const
{
  return room_num_[1];
}

//---------------------------------------------------------------------------------------------------------------------
///
std::vector<Room*> Game::getRooms(){
  return rooms_;
}

//---------------------------------------------------------------------------------------------------------------------
///
std::map<CharacterType, std::array<ulong, 3>> Game::getCharacterPos() const
{
  return character_pos_;
}

//---------------------------------------------------------------------------------------------------------------------
/// sets the games knowledge of the current position of the character
void Game::setCharacterPos(CharacterType type, ulong flat_pos, ulong room){
  character_pos_[type].at(1) = flat_pos;
  character_pos_[type].at(2) = room;  
}

//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
int Game::unlock()
{
  //doors don't have the id D but H or V... sadly
  return specialAction(CharacterType::ThiefCharacter, "HV");
}

//---------------------------------------------------------------------------------------------------------------------
int Game::fight()
{
  bool success;
  success =  specialAction(CharacterType::FighterCharacter, "M");
  if (!success)
  {
    int char_pos = character_pos_[CharacterType::FighterCharacter][1];
    bool monster_active;
    //check if there are other monsters in the room and update value accordingly
    monster_active = rooms_[char_pos]->checkMonster();
    rooms_[char_pos]->setMonster(monster_active);
  }
  return success;
}
//---------------------------------------------------------------------------------------------------------------------
int Game::specialAction(CharacterType character_typ, std::string tile_ids)
{
  //find current position
  std::array<ulong, 3UL> char_pos = getCharacterPos()[character_typ];

  int current_row = char_pos[2] / 5;
  int current_column = char_pos[2] % 5;

  bool success = false;
  int current_room_index = char_pos[1];
  int current_room_row_column[3] = {current_room_index, current_row, current_column};
  //---------------------------------------------
  //left
  int target_direction[2] = {-1, 0};
  adjacentTile(current_room_row_column, target_direction, success, tile_ids);
  //---------------------------------------------
  //right
  target_direction[0] = +1;
  adjacentTile(current_room_row_column, target_direction, success, tile_ids);
  //---------------------------------------------
  //up
  target_direction[0] = 0;
  target_direction[1] = -1;
  adjacentTile(current_room_row_column, target_direction, success, tile_ids);
  //---------------------------------------------
  //down
  target_direction[1] = +1;
  adjacentTile(current_room_row_column, target_direction, success, tile_ids);

  if (success == true)
  {
    return 0;
  }
  else
  {
    return 1;
  }
}

//---------------------------------------------------------------------------------------------------------------------
void Game::adjacentTile(int current_room_row_column[3], int target_direction[2], 
                        bool& success, std::string target_tile_ids)
{
  int target_room = current_room_row_column[0];
  bool bool_continue = true;
  std::vector<Room*> room_vector = getRooms();
  
  int target_row = 0;
  int target_column = 0;

  int room_row_len = getColumns();

  if (target_direction[1] == 0)
  {
    //horizontal
    target_column = current_room_row_column[2] + target_direction[0];
    target_row = current_room_row_column[1];

    if ((target_column < 0) || (target_column > 4))
    {
      //first is if there is a room to the left
      //second is if there is a room to the right
      bool_continue = !(((target_room % room_row_len) == 0) || ((target_room +1) % room_row_len == 0));

      if (bool_continue)
      {
        target_room += target_direction[0];
        target_column = (target_column + 5) % 5;
      }
    }
  }
  else if (target_direction[0] == 0)
  {
    //vertical
    int room_column_len = getRows();
    target_row = current_room_row_column[1] + target_direction[1];
    target_column = current_room_row_column[2];

    if ((target_row == -1) || (target_row == 5))
    {
      target_room = target_room + target_direction[1]*room_row_len;
      //first is if there is a room upwards
      //second is if there is a room downwards
      bool_continue = !((target_room < 0) || ((target_room / room_row_len) > room_column_len));
    }
  }
  else
  {
    std::cout << "problem with adjacentTile" << std::endl;
    bool_continue = false;
  }

  //check if tile is right id
  if ((bool_continue == true) && turn_tile(room_vector[target_room], target_tile_ids, target_row, target_column))
  {
    success = true;
  }
}




//---------------------------------------------------------------------------------------------------------------------
//handles the scry action
//expecting direction input to be parsed correctly
int Game::scry(char room_id_relative, DirectionPrimitive direction_relative)
{
  //first check if on active crystall ball tile
  std::array<ulong, 3UL> char_pos = getCharacterPos()[CharacterType::SeerCharacter];
  std::vector<Room *> room_vector = getRooms();
  Room* current_room = room_vector[char_pos[1]];
  //using the newly overloaded getRoomMap to only return the tile of interest
  Tile* current_tile = current_room->getRoomMap(char_pos[2]);

  //not on active crystal ball
  if ((current_tile->getTileId() != 'C') || !current_tile->getActive())
  {
    //std::cout << "I can't scry without my magic crystal ball!" << std::endl;
    return 1;
  }

  //find the room from room_id
  ulong index_room_id = 0;
  for (auto room : room_vector)
  {
    if (room->getRoomID() == room_id_relative)
    {
      if (room->getRevealed() == false)
      {
        //needs to be revealed
        return 2;
      }
      break;
    }
    index_room_id++;
  }

  //check if room id actually was found
  if (index_room_id + 1 > room_vector.size())
  {
    //return 2 equals:
    //std::cout << "I don't understand which room I should scry!" << std::endl;
    return 2;
  }

  //calculates position of room to scry in room_vector
  index_room_id = scry_position(room_vector, direction_relative, index_room_id);

  if (index_room_id < 0)
  {
    ulong comp = -2;
    if (index_room_id == comp)
    {
      return 4;
    }
    return 3;
  }

  if (room_vector[index_room_id]->getRevealed() == false)
  {
    room_vector[index_room_id]->setRevealed();
  }
  else
  {
    //already know that room
    return 3;
  }

  //turn tile into passage
  current_tile->setInactive();
  return 0;
}

int Game::scry_position(std::vector<Room *> room_vector, DirectionPrimitive direction_relative, ulong index_room_id)
{
  int relative_index_change;
  switch (direction_relative)
  {
    case DirectionPrimitive::UP_P:
      relative_index_change = getRows();
      index_room_id -= relative_index_change;
      if (index_room_id < 0)
      {
        return -2;
      }
      break;

    case DirectionPrimitive::DOWN_P:
      relative_index_change = getRows();
      index_room_id += relative_index_change;
      if (index_room_id + 1 > room_vector.size())
      {
        return -2;
      }
      break;

    case DirectionPrimitive::LEFT_P:
      relative_index_change = 1;
      index_room_id -= relative_index_change;
      if (index_room_id  < 0)
      {
        return -2;
      }
      break;

    case DirectionPrimitive::RIGHT_P:
      relative_index_change = 1;
      index_room_id += relative_index_change;
      if (index_room_id + 1 > room_vector.size())
      {
        return -2;//---------------------------------------------------------------------------------------------------------------------
      }

      break;

    default:
      return -2;
      break;
  }
  return index_room_id;
}

//---------------------------------------------------------------------------------------------------------------------
//to accept string tile_id for multiple ids
//checks if a tile has the id of tile_ids and sets it inactive
//returns true if successfull
bool Game::turn_tile(Room* target_room, std::string tile_ids, int target_row, int target_column)
{
  Tile* target_tile = target_room->getRoomMap()[target_column][target_row];
  for (char single_id : tile_ids)
  {
    if ((target_tile->getTileId() == single_id) && (target_tile->getActive()))
    {
      target_tile->setInactive();
      target_tile->setWalkable();
      target_tile->setJumpable();
      return true;
    }
  }
  return false;
}

//---------------------------------------------------------------------------------------------------------------------
//unlock all hidden doors
void Game::hiddenDoorActivation()
{
  for (Room* room_current : rooms_)
  {
    //iterate through the tilemap array
    //order doesn't matter in this case
    for (int tile_column = 0; tile_column < 5; tile_column++)
    {
      for (int tile_row = 0; tile_row < 5; tile_row++)
      {
        //turn all hidden doors
        turn_tile(room_current, "X", tile_row, tile_column);
      }
    }
  }
  //set buttons inactive
  for (auto set : character_pos_)
  {
    rooms_[set.second[1]]->getRoomMap(set.second[2])->setInactive();
  }
}

std::vector<std::vector<int>>  Game::bfsArrayGenerator()
{
  std::vector<std::vector<int>> bfs_array;
  int rooms_per_row = room_num_[0];
  int number_of_rows;
  if (room_num_[1] != 1)
  {
    number_of_rows = room_num_[1];
  }
  else
  {
    number_of_rows = room_num_[1];
  }




  for (int row = 0; row < number_of_rows; row++)
  {
    for (int room_row = 0; room_row < 5; room_row++)
    {
      std::vector<int> temp;
      for (int column = 0; column < rooms_per_row; column++)
      {
        for (int room_col = 0; room_col < 5; room_col++)
        {
          Room* room = rooms_.at(column + (row * rooms_per_row));
          std::array<std::array<Tile*, 5>, 5> room_map = room->getRoomMap();
          if(room_map.at(room_col).at(room_row)->isOccupied())
          {
            temp.push_back(0);
          }
          else if(!room_map.at(room_col).at(room_row)->isWalkable())
          {
            temp.push_back(5);
          }
          else
          {
            temp.push_back(1);
          }
        }
      }
      bfs_array.push_back(temp);      
    }
  }

  // for(auto room: bfs_array)
  // {
  //   for(auto r: room)
  //   {
  //     std::cout<<r;
  //   }
  //   std::cout<<std::endl;
  // }

  return bfs_array;
}