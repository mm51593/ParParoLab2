#include <vector>
#include <iterator>

#ifndef GAME_IN
#define GAME_IN

/*#include "player.hpp"
#include "board.hpp"*/

class Player;
class Board;

class Game
{
	private:
		Board *board;
        int winning_streak;
		int board_width;
		std::vector<Player> *players;
        std::vector<Player>::iterator player_iter;

		void switch_player();
		bool check_victory(unsigned streak);

	public:
		Game(std::vector<Player> *players, int board_width, int winning_streak);

		~Game();

		int get_board_width();

		bool play_move(int column);

        Player *get_current_player();

        void print_board();

		void copy(Game *other);
};

#endif