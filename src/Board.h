#ifndef BOARD_H
#define BOARD_H

#include <stack>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <string>
#include "Cell.h"
#include "Player.h"
#include "Timer.h"

class Board {
private:
//Data members
//----------------------------------------------------------------------
	//board as 2D array of cells
	Cell cells[15][15];

	//stacks for undo, redo
	std::stack<Cell> game_history, undo_history;

	//2 players only
	Player black, white;
	int moves;

	bool display;

	//which player's turn is it?
	Cell::STATE turn;

	Timer timer;

//internal helper functions
//----------------------------------------------------------------------
	int isMoveValid(Cell start);
	Cell getCell(int x, int y);

	int checkPath(Cell start, int count, Cell::DIRECTION direction);
	int checkPath(Cell start, Cell::DIRECTION direction);

	std::string finish(Cell::STATE state);

public:
    std::string error_message;
//constructors
//----------------------------------------------------------------------
	Board() {
		timer.start();
		turn = Cell::BLACK;
		moves = 0;
		display = true;
		for(int i = 1; i <= 15; i++) {
			for(int j = 1; j <= 15; j++) {
				cells[i-1][j-1].setState(Cell::EMPTY);
				cells[i-1][j-1].setPosition(i, j);
			}
		}
	}

//public APIs
//----------------------------------------------------------------------
	Cell::STATE getTurn() { return turn; }
	std::string placePiece(int x, int y);

	std::string undo();
	std::string redo();

	std::string command(std::string cmd);

	void shell() {
		while(true) {
			std::string cmd;
			std::cin >> cmd;
			std::cout << this->command(cmd);
		}
	}

	void random_ai();

	//print out board's current state
	friend std::ostream& operator<<(std::ostream &out, Board &board);
	std::string get_string_board(Board& board);
	//read into the board from a stream
	friend std::istream& operator>>(std::istream &in, Board &board);
};

#endif
