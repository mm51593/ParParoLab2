#include <iostream>
#include <vector>
#include "board.hpp"

int main(void)
{
    char tokens[] = {'o', 'x'};

    Board b(7);

    int turn = 0;
    int column;

    int drop_location[DIMENSION];
    while (true)
    {
        std::cout << std::endl;
        b.print_board();
        std::cout << std::endl << "Input column: ";
        std::cin >> column;
        b.drop_token(tokens[turn++ % 2], column, drop_location);
        std::cout << "Streak: " << b.get_longest_streak(drop_location[0], drop_location[1]) << std::endl;
    }
    return 0;
}
