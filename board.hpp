#include <vector>

#ifndef BOARD_IN
#define BOARD_IN

#define SEARCH_VECTORS_SIZE 4
#define DIMENSION 2

class Board
{
	private:
		const char EMPTY_SPOT = '.';
		const int SEARCH_VECTORS[SEARCH_VECTORS_SIZE][DIMENSION] = {{1, 0}, {1, 1}, {0, 1}, {-1, 1}};
		std::vector<std::vector<char>> rows;
		int width;

		std::vector<char> get_empty_row(int width);

		void add_row();

	public:
		Board(int width);

		void drop_token(char token, int column, int *result);

		void print_board();

		unsigned get_longest_streak(int row, int column);
};

#endif