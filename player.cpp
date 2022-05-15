#include "player.hpp"
#include "input_method.hpp"
#include <string>

Player::Player(std::string name, char symbol, InputMethod *IM) : name(name), symbol(symbol), IM(IM)
{
	IM->set_player(this);
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

void Player::set_game(Game *game)
{
	this->game = game;
	this->IM->set_game(game);
}