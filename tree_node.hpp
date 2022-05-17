#ifndef TREE_NODE_IN
#define TREE_NODE_IN

/*#include "game.hpp"*/
#include <vector>

class Game;

class TreeNode
{
    public:
        Game *game;
        std::vector<TreeNode*> children;
        float score = 0;
};

#endif