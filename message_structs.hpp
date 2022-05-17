#ifndef MESSAGE_STRUCTS_IN
#define MESSAGE_STRUCTS_IN

#include "tree_node.hpp"
struct MessageSetup
{
    int winning_streak;
    int board_width;
    int move_count;
    int player_count;
    TreeNode *write_destination;
    int this_player;
    int search_depth;
    int cutoff_depth;
};

struct Task
{
    MessageSetup setup;
    int *moves;
};

struct Response
{
    TreeNode *write_destination;
    int index;
    float result;
};

#endif