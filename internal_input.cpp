#include "internal_input.hpp"
#include <iostream>

InternalInput::InternalInput()
{
    return;
}

int InternalInput::return_input()
{
    return next_input;
}

void InternalInput::set_next_input(int new_input)
{
    next_input = new_input;
}