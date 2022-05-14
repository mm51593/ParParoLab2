#include "player.hpp"
#include <string>

Player::Player(std::string name, char symbol, InputMethod *IM) : name(name), symbol(symbol), IM(IM)
{
}

char Player::get_symbol()
{
	return symbol;
}

std::string Player::get_name()
{
	return name;
}

int Player::get_input()
{
	return IM->return_input();
}