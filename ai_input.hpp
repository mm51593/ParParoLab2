#ifndef AI_INPUT_IN
#define AI_INPUT_IT

#include "input_method.hpp"
/*#include "tree_node.hpp"*/

class TreeNode;

#include <vector>

class AIInput : public InputMethod
{
    public:
        AIInput(int search_depth);
        int return_input();
    private:
        int search_depth;
        int compute_move();
        std::vector<int> descend_tree(TreeNode *root, int level);
};

#endif