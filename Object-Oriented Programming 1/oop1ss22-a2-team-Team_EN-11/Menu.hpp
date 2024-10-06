//---------------------------------------------------------------------------------------------------------------------
// Menu.hpp
//
// Implements menu interaction and calling of methods for actions in game
// 
//
// Author: 11812041, 11942956, 12130995
//---------------------------------------------------------------------------------------------------------------------
//

#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <cstring>
#include <utility>
#include "MovesMaster.hpp"
#include "Character.hpp"
#include "Game.hpp"
#include "MenuException.hpp"

// "BLANK" represents both no input and only whitespaces.
enum PlayerCommand { HELP = 0, QUIT = 1, MAP = 2, FLIP = 3, MOVE = 4, UNLOCK = 5, FIGHT = 6, 
SCRY = 7, BLANK = 8 , UNKNOWN = 9};



class Menu
{
private:
	//---------------------------------------------------------------------------------------------------------------------
	/// handles calling fight and error message
	bool fightHandler(Game& game);
	//---------------------------------------------------------------------------------------------------------------------
	/// handles calling unlock and error message
	bool unlockHandler(Game& game);
	//---------------------------------------------------------------------------------------------------------------------
	/// Parses primitive direction for scry
	DirectionPrimitive scryParser();
	//---------------------------------------------------------------------------------------------------------------------
	/// handles calling the parser etc for scry and then calls scry
	/// checks if all arguments needed are given
	bool scryHandler(Game& game);

	std::vector<std::string> words_in_command;
public:
	//---------------------------------------------------------------------------------------------------------------------
	/// default constructor
	Menu() = default;
	//---------------------------------------------------------------------------------------------------------------------
	/// parses main command input into commands for the if statements
	PlayerCommand commandReader();
	//---------------------------------------------------------------------------------------------------------------------
	/// removes whitespaces etc
	std::string inputParser(std::string player_input);
	//---------------------------------------------------------------------------------------------------------------------
	/// 
	std::pair<CharacterType, std::pair<Direction, int>> moveParser();

	//---------------------------------------------------------------------------------------------------------------------
	/// returns parsed room id (for scry)
	char roomIDParser();
	//---------------------------------------------------------------------------------------------------------------------
	/// prints the name of the character (full)
	void characterPrinter(CharacterType character_type);
	//---------------------------------------------------------------------------------------------------------------------
	/// 
	PlayerCommand mapTask(MovesMaster &master, Game& game);

	//---------------------------------------------------------------------------------------------------------------------
	///checks input for whitespaces or simple enters and only prints the > prompt if that is the case
	///if there is actual input it strips whitespaces away and returns the string
	std::string preParseInput();

	//---------------------------------------------------------------------------------------------------------------------
	///checks input for whitespaces or simple enters and only prints the > prompt if that is the case
	///if there is actual input it strips whitespaces away and returns the string
	bool checkCharTilePos(Game& game, CharacterType type, char tile_id);

	//---------------------------------------------------------------------------------------------------------------------
	/// prints helptext
	void helpTextPrinter();

	//---------------------------------------------------------------------------------------------------------------------
	/// check if all buttons are pressed
	/// if pressed correctly then open hidden doors
	void hiddenDoorHandler(Game& game);

	//---------------------------------------------------------------------------------------------------------------------
	/// checks if the win conditions are met
	bool winChecker(Game& game, MovesMaster& master);

};