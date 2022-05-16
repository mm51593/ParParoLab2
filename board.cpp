#include <vector>
#include <iostream>
#include <exception>
#include "board.hpp"

std::vector<char> Board::get_empty_row(int width)			// TODO: destructor to clean up this mess
{
	std::vector<char> row(width, EMPTY_SPOT);

	return row;
}

void Board::add_row()
{
	rows.push_back(get_empty_row(width));
}

Board::Board(int width) : width(width)
{
	add_row();
}

void Board::drop_token(char token, int column, int *result)		// throws exception
{
	if (column < 0 || column >= width)
	{
		throw std::out_of_range("Column number invalid.");
	}

	// Find spot to land
	int level = rows.size();
	for (auto it = rows.rbegin(); it != rows.rend(); it++)
	{
		if (it->at(column) != EMPTY_SPOT)
		{
			break;
		}
		level--;
	}

	// Land
	rows[level][column] = token;

	// Check if board needs to be extended
	if (level == rows.size() - 1)
	{
		add_row();
	}

	result[0] = level;
	result[1] = column;

	return;
}

void Board::print_board()
{
	for (auto row = rows.rbegin(); row != rows.rend(); row++)
	{
		for (auto col = row->begin(); col != row->end(); col++)
		{
			std::cout << *col << " ";
		}
		std::cout << std::endl;
	}
	for (int i = 0; i < width; i++)
	{
		std::cout << i << " ";
	}
	std::cout << std::endl;
}

unsigned Board::get_longest_streak(int row, int column)
{
	char current_token = rows[row][column];
	unsigned maximum_streak = 0;
	for (int i = 0; i < SEARCH_VECTORS_SIZE; i++)
	{
		const int *direction = SEARCH_VECTORS[i];

		unsigned count_in_a_row = 1;
		int new_row;
		int new_column;


		const int subdirection[] = {-1, 1};
		for (int j = 0; j < sizeof(subdirection) / sizeof(int); j++)
		{
			new_row = row;
			new_column = column;

			while (true)
			{
				new_row += subdirection[j] * direction[0];
				new_column += subdirection[j] * direction[1];

				if (new_row < 0 || new_row >= rows.size() || new_column < 0 || new_column >= width)
				{
					break;
				}
				else if (rows[new_row][new_column] != current_token)
				{
					break;
				}
				else
				{
					count_in_a_row++;
				}
			}

		}

		maximum_streak = count_in_a_row > maximum_streak ? count_in_a_row : maximum_streak;
	}

	return maximum_streak;
}

void Board::copy(Board *other)
{
	other->rows = this->rows;
	other->width = this->width;
}

int Board::get_width()
{
	return width;
}