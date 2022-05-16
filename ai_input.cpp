#include "ai_input.hpp"
#include "game.hpp"
#include "tree_node.hpp"
#include "player.hpp"
#include "board.hpp"
#include "internal_input.hpp"
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>

AIInput::AIInput(int search_depth) : search_depth(search_depth)
{
    std::srand(std::time(nullptr));
}

int AIInput::return_input()
{
    int column = compute_move();
    std::cout << column << std::endl;
    return column;
}

int AIInput::compute_move()
{
    std::vector<Player> temp = std::vector<Player>();
    Game current_game(&temp, game->get_board()->get_width(), 0);
    game->copy(&current_game);

    TreeNode root;
    root.value = &current_game;

    std::vector<int> scores = descend_tree(&root, search_depth);


    std::vector<int> best_scores(current_game.get_board()->get_width());
    int maximum = INT32_MIN;
    int i = 0;
    for (auto iter = scores.begin(); iter != scores.end(); iter++)
    {
        if (*iter > maximum)
        {
            maximum = *iter;
            best_scores.clear();
            best_scores.push_back(i);
        }
        else if (*iter == maximum)
        {
            best_scores.push_back(i);
        }

        i++;
    }

    return best_scores.at(std::rand() % best_scores.size());
}

std::vector<int> AIInput::descend_tree(TreeNode *root, int level)
{
    float result;
    level--;

    int board_width = root->value->get_board()->get_width();

    std::vector<int> totals;

    for (int i = 0; i < board_width; i++)
    {
        result = 0;

        TreeNode node;
        root->children.push_back(&node);
        
        std::vector<Player> node_players = std::vector<Player>();
        Game node_game(&node_players, board_width, 0);

        node.value = &node_game;
        root->value->copy(node.value);

        InternalInput II;
        std::vector<Player> *old_players = root->value->get_players();
        for (auto iter = old_players->begin(); iter != old_players->end(); iter++)
        {
            node_players.push_back(Player(iter->get_name(), iter->get_symbol(), &II));
        }
        II.set_next_input(i);

        node_game.set_players(&node_players);

        if (node.value->play_move())
        {
            if (node.value->get_current_player()->get_name() == this->player->get_name())
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
            std::vector<int> subresults = descend_tree(&node, level);

            for (auto res_iter = subresults.begin(); res_iter != subresults.end(); res_iter++)
            {
                result += *res_iter;
            }
        }

        totals.push_back(result);
    }

    return totals;
}