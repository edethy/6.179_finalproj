#ifndef GameOfLife_H
#define GameOfLife_H

#include <iostream>
#include <vector>

using namespace std;

class GameOfLife {
	private:
		int rows, cols;
		std::vector<bool> board;
		void init_random_game();
		bool get_new_cell_state(int i, int j, bool alive);
		int check_cell(int i, int j);
		void advance_step();
	public:
		GameOfLife(int n, int m);
		GameOfLife();
		GameOfLife(ifstream& game_file);
		void advance_game(int steps);
		std::vector<bool> get_next_board();
		int advance_to_steady_state(int start_t);
		void print_next_board(const std::vector<bool> &b, int rows, int cols);
		friend std::ostream &operator<<(std::ostream &os, const GameOfLife &g);
};

#endif