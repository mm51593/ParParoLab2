#include <vector>
#include <iterator>
#include "player.hpp"
#include "board.hpp"

#ifndef GAME_IN
#define GAME_IN

class Game
{
	private:
		Board *board;
        int winning_streak;
        int last_move_streak;
		std::vector<Player> *players;
        std::vector<Player>::iterator player_iter;

		Player switch_player();

	public:
		Game(std::vector<Player> *players, int board_width, int winning_streak);

		void play_move(int column);

        bool check_victory();

        Player get_current_player();

        void print_board();
};

#endif