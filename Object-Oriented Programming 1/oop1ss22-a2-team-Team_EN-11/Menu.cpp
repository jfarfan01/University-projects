//---------------------------------------------------------------------------------------------------------------------
// Menu.cpp
//
// Implements menu interaction and calling of methods for actions in game
// 
//
// Author: 11812041, 11942956, 12130995
//---------------------------------------------------------------------------------------------------------------------
//
#include "Menu.hpp"
//---------------------------------------------------------------------------------------------------------------------
std::string Menu::inputParser(std::string player_input)
{
  words_in_command.clear();
  std::istringstream istring_stream(player_input);
  for (std::string word; istring_stream >> word;)
  {
    std::string case_insensitive;
    for (auto character : word)
    {
      if (character != ' ')
      {
        case_insensitive.push_back(toupper(character));
      }
    }
    words_in_command.push_back(case_insensitive);
  }
  return player_input;
}
//---------------------------------------------------------------------------------------------------------------------
PlayerCommand Menu::commandReader()
{
  std::string command = words_in_command[0];
  // take into account that std::string is not supported on switch case
  if (command == "HELP")
  {
    return PlayerCommand::HELP;
  }

  else if (command == "QUIT")
  {
    return PlayerCommand::QUIT;
  }

  else if (command == "MAP")
  {
    return PlayerCommand::MAP;
  }

  else if (command == "FLIP")
  {
    return PlayerCommand::FLIP;
  }

  else if (command == "MOVE")
  {
    return PlayerCommand::MOVE;
  }

  else if (command == "UNLOCK")
  {
    return PlayerCommand::UNLOCK;
  }

  else if (command == "FIGHT")
  {
    return PlayerCommand::FIGHT;
  }

  else if (command == "SCRY")
  {
    return PlayerCommand::SCRY;
  }

  else if (command == "")
  {
    return PlayerCommand::BLANK;
  }

  return PlayerCommand::UNKNOWN;
}
//---------------------------------------------------------------------------------------------------------------------
// return values for wrong input:
// CharacterType = NoCharacter
// Direction = WRONG
// int distance = -1
std::pair<CharacterType, std::pair<Direction, int>> Menu::moveParser()
{
  if (words_in_command.size() < 3 || words_in_command.size() > 4)
  {
    std::cout << "Please enter a correct NUMBER OF PARAMETERS to not confuse your treasure hunters!" << std::endl;
    std::pair<Direction, int> move_pair = std::make_pair(Direction::WRONG, 5);
    std::pair<CharacterType, std::pair<Direction, int>> wrong_pair;
    wrong_pair = std::make_pair(CharacterType::NoCharacter, move_pair);
    return wrong_pair;
  }
  std::string character_string = words_in_command[1];
  std::string direction_string = words_in_command[2];
  int distance = 1;
  int distance_flag = 0;
  if (words_in_command.size() > 3)
  {
    for (auto character : words_in_command[3])
    {
      if (character < '0' || character > '9')
      {
        distance_flag = 1;
      }
      distance = character - 48;
    }
  }

  CharacterType character_type;
  if (character_string == "F")
  {
    character_type = CharacterType::FighterCharacter;
  }
  else if (character_string == "T")
  {
    character_type = CharacterType::ThiefCharacter;
  }
  else if (character_string == "S")
  {
    character_type = CharacterType::SeerCharacter;
  }
  else
  {
    std::cout << "Who do you want to move?!?" << std::endl;
    character_type = CharacterType::NoCharacter;
  }
  if (distance_flag)
  {
    characterPrinter(character_type);
    distance = -1;
    std::cout << ": \"I don't understand how far I should go!\"" << std::endl;
  }

  Direction direction;
  if (direction_string == "UP")
  {
    direction = UP;
  }
  else if (direction_string == "DOWN")
  {
    direction = DOWN;
  }
  else if (direction_string == "LEFT")
  {
    direction = LEFT;
  }
  else if (direction_string == "RIGHT")
  {
    direction = RIGHT;
  }
  else
  {
    characterPrinter(character_type);
    std::cout << ": \"I don't understand where I should go!\"" << std::endl;
    direction = WRONG;
  }

  std::pair<Direction, int> move_pair = std::make_pair(direction, distance);
  std::pair<CharacterType, std::pair<Direction, int>> complete_pair = std::make_pair(character_type, move_pair);
  return complete_pair;
}
//---------------------------------------------------------------------------------------------------------------------
DirectionPrimitive Menu::scryParser()
{
  // room_id = words_in_command [1];
  std::string direction_string = words_in_command[2];
  DirectionPrimitive direction;

  if (direction_string == "UP")
  {
    direction = DirectionPrimitive::UP_P;
  }

  else if (direction_string == "DOWN")
  {
    direction = DirectionPrimitive::DOWN_P;
  }

  else if (direction_string == "LEFT")
  {
    direction = DirectionPrimitive::LEFT_P;
  }

  else if (direction_string == "RIGHT")
  {
    direction = DirectionPrimitive::RIGHT_P;
  }
  else
  {
    direction = DirectionPrimitive::NONE_P;
  }

  return direction;
}
//---------------------------------------------------------------------------------------------------------------------
char Menu::roomIDParser()
{
  char room_id = words_in_command[1][0];
  return room_id;
}
//---------------------------------------------------------------------------------------------------------------------
void Menu::characterPrinter(CharacterType character_type)
{
  if (character_type == CharacterType::FighterCharacter)
  {
    std::cout << "Fighter";
  }

  else if (character_type == CharacterType::ThiefCharacter)
  {
    std::cout << "Thief";
  }

  else if (character_type == CharacterType::SeerCharacter)
  {
    std::cout << "Seer";
  }
  return;
}
//---------------------------------------------------------------------------------------------------------------------
PlayerCommand Menu::mapTask(MovesMaster& master, Game& game)
{
  PlayerCommand command = commandReader();
  if (command == HELP)
  {
    helpTextPrinter();
    return PlayerCommand::BLANK;
  }
  else if (command == QUIT)
  {
    return command;
  }
  else if (command == MAP)
  {
    return command;
  }
  else if (command == FLIP)
  {
    master.iterateFlips();
    master.flipMoveList(master.moves_);
    return command;
  }

  else if (command == MOVE)
  {
    auto move_command = moveParser();
    Direction direction;
    if (master.moves_[0] == "up")
    {
      direction = UP;
    }
    else if (master.moves_[0] == "right")
    {
      direction = RIGHT;
    }
    else if (master.moves_[0] == "down")
    {
      direction = DOWN;
    }
    else if (master.moves_[0] == "left")
    {
      direction = LEFT;
    }


    if (!(move_command.first == CharacterType::NoCharacter || move_command.second.first == WRONG ||
          move_command.second.second == -1))
    {
      if (move_command.second.first != direction)
      {
        characterPrinter(move_command.first);
        std::cout << ": \"I can't go that way right now!\"" << std::endl;
        return PlayerCommand::BLANK;
      }
      if (master.handleCommand(move_command, game, master))
      {
        //check for hourglass
        master.hourGlassFlipReducer(game, move_command.first);
        //check for hidden door activation
        hiddenDoorHandler(game);
        return command;
      }
    }
    return PlayerCommand::BLANK; // BLANK -> no map print else map print
  }

  else if (command == UNLOCK)
  {
    if (unlockHandler(game))
    {
      return command;
    }
    else
    {
      return PlayerCommand::BLANK;
    }
  }
  else if (command == FIGHT)
  {
    if (fightHandler(game))
    {
      return command;
    }
    else 
    {
      return PlayerCommand::BLANK;
    }
  }

  else if (command == SCRY)
  {
    if( scryHandler(game) )
    {
    return command;
    }
    else
    {
      return PlayerCommand::BLANK;
    }
  }

  else if (command == BLANK)
  {
    return command;
  }
  else
  {
    std::cout << "Please enter a KNOWN COMMAND to not confuse your treasure hunters!"<< std::endl;
    return PlayerCommand::UNKNOWN;
  }
}

//---------------------------------------------------------------------------------------------------------------------
std::string Menu::preParseInput()
{
  std::string user_input;
  // only print > if enter or whitespaces
  while (true)
  {
    // discard whitespaces 
    // DEPRECATED
    while (std::cin.peek() == ' ')
    {
      std::cin.get();
    }
    // get line after discarding and check cases

    std::getline(std::cin, user_input);

    if (user_input == "")
    {
      if (std::cin.eof())
      {
        throw InputEOF();
      }
      std::cout << " > ";
      continue;
    }
    else
    {
      // continue with input and return line
      break;
    }
  }

  return user_input;
}
//---------------------------------------------------------------------------------------------------------------------
bool Menu::scryHandler(Game& game)
{
  int number_commands = words_in_command.size();
  if (number_commands < 3)
  {
    std::cout << "Seer: " << '"' << "I don't understand which room I should scry!" << '"' << std::endl;
    // return so rest doesn't happen
    return false;
  }

  DirectionPrimitive direction = scryParser();
  char target_room = roomIDParser();

  int scry_out;
  scry_out = game.scry(target_room, direction);
  switch (scry_out)
  {
    case 1:
      std::cout << "Seer: " << '"' << "I can't scry without my magic crystal ball!" << '"' << std::endl;
      return false;
      break;
    case 2:
      std::cout << "Seer: " << '"' << "I don't understand which room I should scry!" << '"' << std::endl;
      return false;
      break;
    case 3:
      std::cout << "Seer: " << '"' << "We already know that room..." << '"' << std::endl;
      return false;
      break;
    case 4:
      std::cout << "Seer: " << '"' << "There is no room I can reveal at this position!" << '"' << std::endl;
      return false;
      break;
  }
  return true;
}

//---------------------------------------------------------------------------------------------------------------------
///checks input for whitespaces or simple enters and only prints the > prompt if that is the case
///if there is actual input it strips whitespaces away and returns the string
bool Menu::checkCharTilePos(Game& game, CharacterType type, char tile_id)
{
  std::array<ulong, 3UL> char_pos = game.getCharacterPos()[type];

  Room* char_room = game.getRooms()[char_pos[1]];
  Tile* char_tile = char_room->getRoomMap(char_pos[2]);
  //check if tile and tile_id fit
  if (char_tile->getTileId() == tile_id)
  {
    return true;
  }
  return false;
}

//---------------------------------------------------------------------------------------------------------------------
void Menu::helpTextPrinter()
{
  std::cout<< "Commands:\n"
  <<" - help\n"
  <<"    Prints this help text.\n"
  <<"\n"
  <<" - quit\n"
  <<"    Terminates the game.\n"
  <<"\n"
  <<" - map\n"
  <<"    Activates or deactivates the map.\n"
  <<"\n"
  <<" - flip\n"
  <<"    Changes the possible move direction.\n"
  <<"\n"
  <<" - move <CHARACTER> <DIRECTION> <DISTANCE>\n"
  <<"    Moves a character.\n"
  <<"    <CHARACTER>: the character to move, T/F/S\n"
  <<"    <DIRECTION>: the direction to move, up/down/left/right\n"
  <<"    <DISTANCE>: optional, the distance to move\n"
  <<"\n"
  <<" - unlock\n"
  <<"    Unlocks a nearby door.\n"
  <<"\n"
  <<" - fight\n"
  <<"    Fights a nearby monster.\n"
  <<"\n"
  <<" - scry <KNOWN_ROOM> <DIRECTION>\n"
  <<"    Reveals an unknown room.\n"
  <<"    <KNOWN_ROOM>: where to scry from\n"
  <<"    <DIRECTION>: which direction to scry\n"
  <<"\n";
  return;

}


//---------------------------------------------------------------------------------------------------------------------
/// check if all buttons are pressed
/// if pressed correctly then open hidden doors
void Menu::hiddenDoorHandler(Game& game)
{
  //check all characters
  if (!checkCharTilePos(game, CharacterType::FighterCharacter, 'F'))
  {
    return;
  }
  if (!checkCharTilePos(game, CharacterType::SeerCharacter, 'S'))
  {
    return;
  }
  if (!checkCharTilePos(game, CharacterType::ThiefCharacter, 'T'))
  {
    return;
  }
  //activate doors
  game.hiddenDoorActivation();
  auto rooms = game.getRooms();
}

//---------------------------------------------------------------------------------------------------------------------
bool Menu::winChecker(Game& game, MovesMaster& master)
{
  if(checkCharTilePos(game, CharacterType::FighterCharacter,'L') == true && 
  checkCharTilePos(game, CharacterType::SeerCharacter,'L') == true &&
  checkCharTilePos(game, CharacterType::ThiefCharacter,'L') == true)
  {
    std::cout<< "You win, congratulations! It took you " << master.getFlips()
    << " card flips to find the treasure.\n";
    return true;
  }
  return false;
}

//---------------------------------------------------------------------------------------------------------------------
/// handles fighting and error message
bool Menu::fightHandler(Game& game)
{
  if (game.fight())
  {
    std::cout << "Fighter: " << '"' << "Nothing to fight here!" << '"' << std::endl;
    return false;
  }
  return true;
}
//---------------------------------------------------------------------------------------------------------------------
/// handles unlocking and error message
bool Menu::unlockHandler(Game& game)
{
  if (game.unlock())
  {
    std::cout << "Thief: " << '"' << "Nothing to unlock here!" << '"' << std::endl;
    return false;
  }
  return true;
}