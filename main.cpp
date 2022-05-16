#include <iostream>
#include <vector>
#include "player.hpp"
#include "game.hpp"
#include "player_input.hpp"
#include "ai_input.hpp"

#define BOARD_WIDTH 7
#define WINNING_STREAK 4
#define SEARCH_DEPTH 4

int main(void)
{
    Player player1("Player1", 'x', new PlayerInput());
    Player player2("Player2", 'o', new AIInput(SEARCH_DEPTH));
    //Player player2("Player2", 'o', new PlayerInput());

    std::vector<Player> *players = new std::vector<Player>();
    players->push_back(player1);
    players->push_back(player2);

    Game game(players, BOARD_WIDTH, WINNING_STREAK);

    do 
    {
        std::cout << std::endl;
	    game.print_board();
        std::cout << std::endl << "Input column: ";
    } while(!game.play_move());

    std::cout << std::endl;
    game.print_board();
    std::cout << game.get_current_player()->get_name() << " wins!" << std::endl;

    return 0;
}
