#include <vector>
#include <iostream>
#include "game.hpp"
#include "player.hpp"
#include "board.hpp"

void Game::switch_player()
{
	++current_player_index;
	if (current_player_index >= players->size())
	{
		current_player_index = 0;
	}
	return;
}

Game::Game(std::vector<Player> *players, int board_width, int winning_streak) : 
	players(players), winning_streak(winning_streak), current_player_index(0)
{
	for (auto it = players->begin(); it != players->end(); it++)
	{
		it->set_game(this);
	}

	board = new Board(board_width);
}

Game::~Game()
{
	delete board;
}

int Game::get_winning_streak()
{
	return winning_streak;
}

Board *Game::get_board()
{
	return board;
}

bool Game::play_move()
{
	int column = get_current_player()->get_input();
	char player_symbol = get_current_player()->get_symbol();

	int drop_location[2];
	
	board->drop_token(player_symbol, column, drop_location);

	moves.push_back(column);

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
	return &players->at(current_player_index);
}

void Game::print_board()
{
	board->print_board();
}

void Game::copy(Game *other)
{
	other->winning_streak = this->winning_streak;
	other->players = this->players;
	other->current_player_index = this->current_player_index;

	for (auto iter = this->moves.begin(); iter != this->moves.end(); iter++)
	{
		other->moves.push_back(*iter);
	}

	this->board->copy(other->board);
}

void Game::set_players(std::vector<Player> *new_players)
{
	this->players = new_players;
}

std::vector<Player> *Game::get_players()
{
	return players;
}

std::vector<int> *Game::get_moves()
{
	return &moves;
}