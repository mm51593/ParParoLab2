#include "player_input.hpp"
#include <iostream>

PlayerInput::PlayerInput()
{
    return;
}

int PlayerInput::return_input()
{
    int column;
    std::cin >> column;

    return column;
}