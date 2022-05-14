#include <vector>
#include <iostream>
#include "game.hpp"

Player Game::switch_player()
{
	++player_iter;
	if (player_iter == players->end())
	{
		player_iter = players->begin();
	}
	return *player_iter;
}

Game::Game(std::vector<Player> *players, int board_width, int winning_streak) : 
	players(players), winning_streak(winning_streak), last_move_streak(0)
{
	board = new Board(board_width);
	player_iter = players->begin();
}

void Game::play_move(int column)
{
	int drop_location[2];
	char player_symbol = switch_player().get_symbol();
	board->drop_token(player_symbol, column, drop_location);

	last_move_streak = board->get_longest_streak(drop_location[0], drop_location[1]);
	
	return;
}

bool Game::check_victory()
{
	return last_move_streak >= winning_streak;
}

Player Game::get_current_player()
{
	return *player_iter;
}

void Game::print_board()
{
	board->print_board();
}