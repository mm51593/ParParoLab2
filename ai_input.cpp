#include "ai_input.hpp"
#include "game.hpp"
#include "tree_node.hpp"
#include "player.hpp"
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
    Game current_game(&temp, game->get_board_width(), 0);
    game->copy(&current_game);

    TreeNode root;
    root.value = &current_game;

    std::vector<int> scores = descend_tree(&root, search_depth);


    std::vector<int> best_scores(current_game.get_board_width());
    int maximum = INT32_MIN;
    int i = 0;
    for (auto iter = scores.begin(); iter != scores.end(); iter++)
    {
        std::cout << *iter << " ";
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
    std::cout << std::endl;

    return best_scores.at(std::rand() % best_scores.size());
}

std::vector<int> AIInput::descend_tree(TreeNode *root, int level)
{
    int result;
    level--;

    std::vector<int> totals;

    for (int i = 0; i < root->value->get_board_width(); i++)
    {
        result = 0;

        TreeNode node;
        root->children.push_back(&node);
        
        std::vector<Player> node_player = std::vector<Player>();
        Game node_game(&node_player, root->value->get_board_width(), 0);

        node.value = &node_game;
        root->value->copy(node.value);

        if (node.value->play_move(i))
        {
            if (node.value->get_current_player()->get_name() == this->player->get_name())
            {
                result += pow(root->value->get_board_width(), level);
            }
            else
            {
                result -= pow(root->value->get_board_width(), level);
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