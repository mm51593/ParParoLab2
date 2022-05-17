#include "player.hpp"
#include "input_method.hpp"
#include "game.hpp"
#include <string>
#include <iostream>
#include <vector>

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
	int temp = IM->return_input();
	return temp;
}

void Player::set_game(Game *game)
{
	this->game = game;
	this->IM->set_game(game);

	std::vector<Player> *players_in_game = game->get_players();

	for (int i = 0; i < players_in_game->size(); i++)
	{
		if (players_in_game->at(i).get_name() == name)
		{
			this->IM->set_rank_in_game(i);
			break;
		}
	}
}