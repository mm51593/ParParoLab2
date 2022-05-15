#ifndef TREE_NODE_IN
#define TREE_NODE_IN

/*#include "game.hpp"*/
#include <vector>

class Game;

class TreeNode
{
    public:
        Game *value;
        std::vector<TreeNode*> children;
};

#endif