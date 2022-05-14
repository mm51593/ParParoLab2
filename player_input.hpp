#include "input_method.hpp"

#ifndef PLAYER_INPUT_IN
#define PLAYER_INPUT_IN

class PlayerInput : public InputMethod
{
    public:
        PlayerInput();
        int return_input();
};

#endif