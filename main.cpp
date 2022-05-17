#include <iostream>
#include <vector>
#include "player.hpp"
#include "game.hpp"
#include "player_input.hpp"
#include "ai_input.hpp"
#include <mpi.h>

#define BOARD_WIDTH 7
#define WINNING_STREAK 4
#define SEARCH_DEPTH 6
#define SEARCH_CUTOFF 2

#define debug
#ifdef debug

void slave_loop();

int main(void)
{
    MPI_Init(nullptr, nullptr);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (world_rank == 0)
    {
        Player player1("Player1", 'x', new PlayerInput());
        Player player2("Player2", 'o', new AIInput(SEARCH_DEPTH, SEARCH_CUTOFF));
        //Player player2("Player2", 'o', new PlayerInput());

        std::vector<Player> *players = new std::vector<Player>();
        players->push_back(player1);
        players->push_back(player2);

        Game game(players, BOARD_WIDTH, WINNING_STREAK);

        /*game.play_move();
        game.play_move();*/
        do 
        {
            std::cout << std::endl;
            game.print_board();
            std::cout << std::endl << "Input column: ";
        } while(!game.play_move());


        // Close other nodes
        MessageSetup finishing_setup;
        finishing_setup.player_count = 0;
        for (int i = 1; i < world_size; i++)
        {
            MPI_Send(&finishing_setup, sizeof(MessageSetup), MPI_BYTE, i, 0, MPI_COMM_WORLD);
        }
    }
    else
    {
        slave_loop();
    }

    MPI_Finalize();
    return 0;
}

#endif
#ifndef debug
#include <mpi.h>

int main(void)
{
    // setup
    MPI_Init(nullptr, nullptr);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (world_rank == 0)
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
    }
    else
    {
        
    }


    MPI_Finalize();
    return 0;
}
#endif