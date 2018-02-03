#include "GameOfLife.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>

using namespace std;

GameOfLife::GameOfLife() {
	rows = random() % 20;
	cols = random() % 20;
	board.resize(rows*cols);
	init_random_game();
}

GameOfLife::GameOfLife(ifstream& game_file) {
	string row_string;
	string col_string;
	if (game_file.good()) {
		getline(game_file, col_string, ' ');
		getline(game_file, row_string);
		stringstream ss;
		ss << row_string << ' ' << col_string;
		ss >> rows >> cols;
	}
	board.resize(rows*cols);
	for (int i=0;i<rows;i++) {
		string new_row;
		getline(game_file, new_row);
		istringstream iss(new_row);
		vector<string> results(istream_iterator<string>{iss},istream_iterator<string>());
		for (int j=0;j<cols;j++) {
			string res = results[j];
			if (res[0] == 'O') {
				board[cols*i + j] = true;
			} else {
				board[cols*i + j] = false;
			}
		}	
	}
}

GameOfLife::GameOfLife(int n, int m) {
	rows = m;
	cols = n;
	board.resize(n*m);
	init_random_game();
}
	

void GameOfLife::init_random_game() {
	for(int i=0; i<rows; i++) {
		for (int j=0; j<cols;j++) {
			bool alive = rand() % 2;
			board[cols*i+j] = alive;
		}
	}
}

int GameOfLife::check_cell(int i, int j) {
	if (i <0 || j < 0 || i >= rows || j >= cols) {
		return 0;
	} else if (board[cols*j + i]) {
		return 1;
	} else {
		return 0;
	} 
}

bool GameOfLife::get_new_cell_state(int i, int j, bool alive) {
	int num_neighbors = 0;
	num_neighbors += check_cell(i-1, j);
	num_neighbors += check_cell(i, j-1);
	num_neighbors += check_cell(i, j+1);
	num_neighbors += check_cell(i+1, j);

	if (num_neighbors == 3 || (num_neighbors == 2 && alive)) {
		return true;
	} if (num_neighbors == 3 && alive) {
		return false;
	} if (num_neighbors <2 ) {
		return false;
	}
	return false;
}

std::vector<bool> GameOfLife::get_next_board() {
	std::vector<bool> new_board;
	new_board.resize(rows*cols);
	for (int j = 0; j<rows;j++) {
		for (int i=0; i<cols; i++) {
			new_board[cols*j + i] = get_new_cell_state(i, j, board[cols*j + i]);
		}
	}
	return new_board;
}


void GameOfLife::advance_step() {
	board = get_next_board();
}

int GameOfLife::advance_to_steady_state(int start_t) {
	std::vector<bool> next_board;
	int counter = 0;
	while(true) {
		counter++;
		next_board = get_next_board();
				if (board == next_board) {
			board = next_board;
			return start_t + counter-1;
		}
		this_thread::sleep_for(500ms);
		printf("Board at t=%d\n",start_t+counter);
		print_next_board(next_board, rows, cols);
		board = next_board;
	}
}

void GameOfLife::print_next_board(const std::vector<bool> &b, int rows, int cols) {
	for (int j=0;j<rows;j++) {
		for (int i=0;i<cols;i++) {
			if (b[cols*j+i]) {
				printf(" * ");
			}	else {
				printf(" ");
			}
		}
		printf("\n");
	}
}

std::ostream &operator<<(std::ostream &os, const GameOfLife &game) {
	for (int j=0;j<game.rows;j++) {
		for (int i=0;i<game.cols;i++) {
			if (game.board[game.cols*j + i]) {
				os << " * ";
			} else {
				os << "  ";
			}
		}
		os << "\n";
	}
	return os;
}

void GameOfLife::advance_game(int steps) {
	for (int i=0; i<steps;i++) {
		advance_step();
	}
}


int main(int argc, char* argv[]) {
	GameOfLife game;
	if (argc == 1) {
		int n = random() % 20;
		int m = random() % 20;
		printf("Initialized Random Game with Dimensions %d x %d\n",n,m);
		game = GameOfLife(n, m);
	} else if (argc == 2) {
		ifstream game_file(argv[1]);
		printf("Initialized Game from File %s\n",argv[1]);
		game = GameOfLife(game_file);
	} else if (argc == 3) {
		int n = atoi(argv[1]);
		int m = atoi(argv[2]);
		printf("Initialized Random Game with Provided Dimensions %d x %d\n",n,m);
		game = GameOfLife(n, m);
	}
	printf("Board at t=0\n");
	std::cout << game << "\n";
	int t=0;
	while(true) {
		char user_input;
		cout << "Enter Number of Steps to Advance or C to advance to steady state: ";
		cin >> user_input;
		if (user_input == 'C') {
			int steps_to_steady_state = game.advance_to_steady_state(t);
			printf("Steady state reached at t=%d\n",steps_to_steady_state);
			return 0;
		} else {
			int steps_to_advance = atoi(&user_input);
			game.advance_game(steps_to_advance);
			t += steps_to_advance;
			printf("Board at t=%d\n", t);
			std::cout << game << "\n";
		}
	}

}