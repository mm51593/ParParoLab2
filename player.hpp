#include <string>
#include "input_method.hpp"

#ifndef PLAYER_IN
#define PLAYER_IN

class Player
{
	private:
		char symbol;
        std::string name;
        InputMethod *IM;

	public:
		Player(std::string name, char symbol, InputMethod *IM);
	
		char get_symbol();

        std::string get_name();

        int get_input();
};

#endif