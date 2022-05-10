#include <vector>
#include "player.hpp"
#include "board.hpp"

class Game
{
	private:
		Board board;
		std::vector<Player> players;
		int current_player_index;
		int board_width;

		void switch_player()
		{
			current_player_index = (current_player_index + 1) % players.size();
		}

	public:
		Game(vector<Player> players, int board_width) : 
			players(players), current_player_index(0), board_width(board_width)
		{
		}

		void play_move();
}
