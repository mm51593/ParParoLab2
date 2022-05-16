#include "input_method.hpp"

#ifndef INTERNAL_INPUT_IN
#define INTERNAL_INPUT_IN

class InternalInput : public InputMethod
{
    private:
        int next_input;
    public:
        InternalInput();
        int return_input();
        void set_next_input(int new_input);
};

#endif