#ifndef PLAYER_IN
#define PLAYER_IN

#include <string>
/*#include "game.hpp"
#include "input_method.hpp"*/

class Game;
class InputMethod;

class Player
{
	private:
		char symbol;
        std::string name;
        InputMethod *IM;
		Game *game = nullptr;

	public:
		Player(std::string name, char symbol, InputMethod *IM);
	
		char get_symbol();

        std::string get_name();

        int get_input();

		void set_game(Game *game);
};

#endif