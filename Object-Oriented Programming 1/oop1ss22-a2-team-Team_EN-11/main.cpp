//---------------------------------------------------------------------------------------------------------------------
// Main.cpp
//
// This is where the game is played
// 
//
// Author: 11942956
//---------------------------------------------------------------------------------------------------------------------
//
#include "Game.hpp"
#include "MovesMaster.hpp"
#include "Menu.hpp"
#include <iomanip>

//---------------------------------------------------------------------------------------------------------------------
int MenuLoop(Game& game, MovesMaster& moves)
{
  std::string user_input;
  int turn_counter = 0;
  int quit_value = 1;
  bool quit = false;
  bool map = true;

  std::string card_flip_message{"Card Flip Counter:"};

  Menu* menu = new Menu();
  PlayerCommand command = PlayerCommand::UNKNOWN;
  while (!quit)
  {
    if (command != BLANK)
    {
      if(command != MAP || map)
      {
        std::cout << card_flip_message;
        std::cout.width(4);
        std::cout << std::setfill(' ') <<  moves.getFlips() << std::endl;
        std::cout.width(0);
      }
      if (map)
        game.boardPrinter();
      if (command != MAP || map)
        std::cout << "Possible move: " << moves.moves_[turn_counter] << std::endl;
    }
    if (menu->winChecker(game, moves) == true)
    {
      delete menu;
      return 0;
    }

    std::cout << " > ";

    std::string user_input;

    try
    {
      user_input = menu->preParseInput();
    }
    catch (const InputEOF& e)
    {
      quit_value = 0;
      break;
    }

    // moves.handleInput(user_input, game, moves);
    std::string parsed_input = menu->inputParser(user_input);
    command = menu->mapTask(moves, game);
    if (command == QUIT)
    {
      quit = true;
      quit_value = 0;
    }
    if (command == MAP)
      map = map ^ true;
    // std::cout << parsed_input;
  }
  delete menu;
  return quit_value;
}

//---------------------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
  std::string input;
  for (int i = 1; i < argc; i++) 
  {
    input = input + argv[i];
    if (argc > 2 && i != argc-1)
    {
      input = input + " ";
    }
  }
  //std::cout << input;

  Game& game = Game::instance(); 
   
  try
  {
    game.setInstance(input);
  }
  catch(const InvalidConfig &e)
  {
    std::cerr << e.what() << '\n';
    return 2;
  }
  catch(const std::bad_alloc &e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }

  //return the opening string
  std::cout << "Welcome to the magical OOP1 Maze!!!" << std::endl;

  MovesMaster move;
  int quit_value = MenuLoop(game, move);
  return quit_value;
};
