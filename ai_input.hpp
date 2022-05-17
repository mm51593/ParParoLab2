#ifndef AI_INPUT_IN
#define AI_INPUT_IT

#include "input_method.hpp"
#include "message_structs.hpp"
/*#include "tree_node.hpp"*/

class TreeNode;

#include <vector>

class AIInput : public InputMethod
{
    public:
        AIInput(int search_depth, int search_cutoff);
        int return_input();
        float compute_move_2(int descend_depth);
    private:
        int search_depth;
        int search_cutoff;
        int compute_move_1();
        void descend_tree_1(TreeNode *root, int level, std::vector<Task> *tasks);
        std::vector<float> descend_tree_2(TreeNode *root, int level);
};

#endif