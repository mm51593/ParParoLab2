#include "game.hpp"

class Player
{
	private:
		char symbol;

	public:
		Player(char symbol) : symbol(symbol)
		{
		}
	
		char get_symbol()
		{
			return this->symbol;
		}
};