#include <vector>
#include <iostream>
#include "game.hpp"
#include "player.hpp"
#include "board.hpp"

void Game::switch_player()
{
	++player_iter;
	if (player_iter == players->end())
	{
		player_iter = players->begin();
	}
	return;
}

Game::Game(std::vector<Player> *players, int board_width, int winning_streak) : 
	players(players), board_width(board_width), winning_streak(winning_streak)
{
	for (auto it = players->begin(); it != players->end(); it++)
	{
		it->set_game(this);
	}

	board = new Board(board_width);
	player_iter = players->begin();
}

Game::~Game()
{
	delete board;
}

int Game::get_board_width()
{
	return board_width;
}

bool Game::play_move(int column)
{
	int drop_location[2];
	char player_symbol = get_current_player()->get_symbol();
	board->drop_token(player_symbol, column, drop_location);

	unsigned last_move_streak = board->get_longest_streak(drop_location[0], drop_location[1]);
	
	bool is_over = check_victory(last_move_streak);

	if (!is_over)
	{
		switch_player();
	}

	return is_over;
}

bool Game::check_victory(unsigned streak)
{
	return streak >= winning_streak;
}

Player *Game::get_current_player()
{
	return &(*player_iter);
}

void Game::print_board()
{
	board->print_board();
}

void Game::copy(Game *other)
{
	other->winning_streak = this->winning_streak;
	other->players = this->players;
	other->player_iter = this->player_iter;

	this->board->copy(other->board);
}