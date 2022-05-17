#include "ai_input.hpp"
#include "game.hpp"
#include "tree_node.hpp"
#include "player.hpp"
#include "board.hpp"
#include "internal_input.hpp"
#include "message_structs.hpp"
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cstring>
#include <mpi.h>
#include <limits>

void send_task(Task *task, int dest)
{
    // send setup
    MPI_Send(&(task->setup), sizeof(MessageSetup), MPI_BYTE, dest, 0, MPI_COMM_WORLD);

    // send moves
    MPI_Send(task->moves, task->setup.move_count, MPI_INT, dest, 0, MPI_COMM_WORLD);
}

float DFS_and_free(TreeNode *node)
{
    float result = 0;
    if (node->children.size() != 0)
    {
        for (auto iter = node->children.begin(); iter != node->children.end(); iter++)
        {
            result += DFS_and_free(*iter);
            delete *iter;

        }
        node->score = result;
    }
    else
    {
        result = node->score;
    }

    return result;
}

AIInput::AIInput(int search_depth, int search_cutoff) : search_depth(search_depth), search_cutoff(search_cutoff)
{
    std::srand(std::time(nullptr));
}

int AIInput::return_input()
{
    int column = compute_move_1();
    std::cout << column << std::endl;
    return column;
}

int AIInput::compute_move_1()
{
    std::vector<Player> temp = std::vector<Player>();
    Game current_game(&temp, game->get_board()->get_width(), 0);
    game->copy(&current_game);

    TreeNode root;
    root.game = &current_game;

    std::vector<Task> tasks;
    descend_tree_1(&root, search_cutoff, &tasks);

    /*
        SEND TASKS
    */
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int tasks_sent = 0;
    int tasks_received = 0;
    Response resp;
    MPI_Status status;
    for (auto iter = tasks.begin(); iter != tasks.end(); iter++)
    {
        if (tasks_sent < world_size - 1)
        {
            send_task(&*iter, tasks_sent + 1);
        }
        else
        {
            // receive message
            MPI_Recv(&resp, sizeof(resp), MPI_BYTE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            tasks_received++;

            // set nodes
            resp.write_destination->score = resp.result;

            // send new task
            int sender = status.MPI_SOURCE;
            send_task(&*iter, sender);
        }

        tasks_sent++;
    }

    // wait for remaining results
    while (tasks_received < tasks_sent)
    {
        // receive message
        MPI_Recv(&resp, sizeof(resp), MPI_BYTE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        tasks_sent--;

        // set nodes
        resp.write_destination->score = resp.result;
    }

    /*
        COLLECT AND DELETE
    */
    
    DFS_and_free(&root);

    /*
        GET BEST MOVE
    */
    std::vector<float> scores;
    for (auto iter = root.children.begin(); iter != root.children.end(); iter++)
    {
        //std::cout << (*iter)->score << " ";
        scores.push_back((*iter)->score);
    }
    //std::cout << std::endl;

    std::vector<int> best_scores;
    float maximum = -std::numeric_limits<float>::max();
    for (int i = 0; i < scores.size(); i++)
    {
        if (scores[i] > maximum)
        {
            maximum = scores[i];
            best_scores.clear();
            best_scores.push_back(i);
        }
        else if (scores[i] == maximum)
        {
            best_scores.push_back(i);
        }
    }

    return best_scores[rand() % best_scores.size()];
}

float AIInput::compute_move_2(int descend_depth)
{
    std::vector<Player> temp = std::vector<Player>();
    Game current_game(&temp, game->get_board()->get_width(), 0);
    game->copy(&current_game);

    TreeNode root;
    root.game = &current_game;

    std::vector<float> scores = descend_tree_2(&root, descend_depth);

    float result = 0;
    for (auto iter = scores.begin(); iter != scores.end(); iter++)
    {
        result += *iter;
    }

    return result;
}

void AIInput::descend_tree_1(TreeNode *root, int level, std::vector<Task> *tasks)
{
    float result;
    level--;

    int board_width = root->game->get_board()->get_width();

    for (int i = 0; i < board_width; i++)
    {
        result = 0;

        TreeNode *node = new TreeNode;
        root->children.push_back(node);
        
        std::vector<Player> node_players = std::vector<Player>();
        Game node_game(&node_players, board_width, 0);

        node->game = &node_game;
        root->game->copy(node->game);

        InternalInput II;
        std::vector<Player> *old_players = root->game->get_players();
        for (auto iter = old_players->begin(); iter != old_players->end(); iter++)
        {
            node_players.push_back(Player(iter->get_name(), iter->get_symbol(), &II));
        }
        II.set_next_input(i);

        node_game.set_players(&node_players);

        if (node->game->play_move())
        {
            if (node->game->get_current_player()->get_name() == this->player->get_name())
            {
                result += pow(board_width, level);
            }
            else
            {
                result -= pow(board_width, level);
            }
        }
        else if (level != 0)
        {
            descend_tree_1(node, level, tasks);
        }
        else
        {
            Task new_task;
            MessageSetup setup;
            setup.winning_streak = node->game->get_winning_streak();
            setup.board_width = node->game->get_board()->get_width();
            setup.move_count = node->game->get_moves()->size();
            setup.player_count = node->game->get_players()->size();
            setup.write_destination = node;
            setup.this_player = rank_in_game;
            setup.search_depth = search_depth;
            setup.cutoff_depth = search_cutoff;

            new_task.moves = new int[node->game->get_moves()->size()];

            int j = 0;
            for (auto iter = node->game->get_moves()->begin(); iter != node->game->get_moves()->end(); iter++)
            {
                new_task.moves[j++] = *iter;
            }

            new_task.setup = setup;

            tasks->push_back(new_task);
        }

        node->score += result;
    }

    return;
}

std::vector<float> AIInput::descend_tree_2(TreeNode *root, int level)
{
    float result;
    level--;

    int board_width = root->game->get_board()->get_width();

    std::vector<float> totals;

    for (int i = 0; i < board_width; i++)
    {
        result = 0;

        TreeNode node;
        root->children.push_back(&node);
        
        std::vector<Player> node_players = std::vector<Player>();
        Game node_game(&node_players, board_width, 0);

        node.game = &node_game;
        root->game->copy(node.game);

        InternalInput II;
        std::vector<Player> *old_players = root->game->get_players();
        for (auto iter = old_players->begin(); iter != old_players->end(); iter++)
        {
            node_players.push_back(Player(iter->get_name(), iter->get_symbol(), &II));
        }
        II.set_next_input(i);

        node_game.set_players(&node_players);

        if (node.game->play_move())
        {
            if (node.game->get_current_player()->get_name() == this->player->get_name())
            {
                result += pow(board_width, level);
            }
            else
            {
                result -= pow(board_width, level);
            }
        }
        else if (level != 0)
        {
            std::vector<float> subresults = descend_tree_2(&node, level);

            for (auto res_iter = subresults.begin(); res_iter != subresults.end(); res_iter++)
            {
                result += *res_iter;
            }
        }

        totals.push_back(result);
    }

    return totals;
}