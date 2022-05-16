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
		std::vector<Player> *players;
		int current_player_index;

		void switch_player();
		bool check_victory(unsigned streak);

	public:
		Game(std::vector<Player> *players, int board_width, int winning_streak);

		~Game();

		Board *get_board();

		bool play_move();

        Player *get_current_player();

        void print_board();

		void copy(Game *other);

		void set_players(std::vector<Player> *new_players);

		std::vector<Player> *get_players();
};

#endif