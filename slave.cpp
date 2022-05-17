#include <mpi.h>
#include "message_structs.hpp"
#include "game.hpp"
#include "player.hpp"
#include "internal_input.hpp"
#include "ai_input.hpp"
#include <vector>
#include <string>

void slave_loop()
{
    //std::cout << "entered loop" << std::endl;
    while (true)
    {
        // receive setup
        MessageSetup setup;
        MPI_Recv(&setup, sizeof(setup), MPI_BYTE, 
            MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, 
            MPI_STATUS_IGNORE);

        //std::cout << "received setup" << std::endl;

        // introduce breakpoint
        if (setup.player_count == 0)
        {
            break;
        }

        // receive moves
        int moves[setup.move_count];
        MPI_Recv(moves, setup.move_count, MPI_INT, 
            MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD,
            MPI_STATUS_IGNORE);

        //std::cout << "received moves" << std::endl;

        // index of current node
        int index = moves[setup.move_count - 1];

        // build game
            // create dummy players
        std::vector<Player> players;
        InternalInput II;
        for (int i = 0; i < setup.player_count; i++)
        {
            players.push_back(Player(std::to_string(i), i, &II));
        }

            // create game
        Game game(&players, setup.board_width, setup.winning_streak);

            // play moves
        for (int i = 0; i < setup.move_count; i++)
        {
            II.set_next_input(moves[i]);
            game.play_move();
        }

        // change players to AI
            // create AIs
        std::vector<Player> new_players;
        std::vector<AIInput> AIs;
        for (auto iter = players.begin(); iter != players.end(); iter++)
        {
            AIs.push_back(AIInput(setup.search_depth, setup.cutoff_depth));
            new_players.push_back(Player(iter->get_name(), iter->get_symbol(), &AIs.back()));
            new_players.back().set_game(&game);
        }
            // attach AIs to the game
        game.set_players(&new_players);

        // compute moves
        float result = AIs[setup.this_player].compute_move_2(setup.search_depth - setup.cutoff_depth);

        // flip result for other players
        if (setup.this_player != setup.move_count % setup.player_count)
        {
            result *= -1;
        }

        //std::cout << "result: " << result << std::endl;

        Response resp = {
            setup.write_destination,
            index,
            result
        };

        MPI_Send(&resp, sizeof(resp), MPI_BYTE, 0, 0, MPI_COMM_WORLD);
    }
}