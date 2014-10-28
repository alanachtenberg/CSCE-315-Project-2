#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <stack>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <string>
#include "Cell.h"
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
	//Player black, white; removed to avoid foward declaring board in player.h, player is determined by member turn anyway
	int moves;

	bool display;

	//which player's turn is it?
	Cell::STATE turn;

	Timer timer;

//internal helper functions
//----------------------------------------------------------------------
	Cell getCell(int x, int y);
	int isMoveValid(Cell start);

	int checkPath(Cell start, int count, Cell::DIRECTION direction);
	int checkPath(Cell start, Cell::DIRECTION direction);

    std::stack<Cell> getGameHistory(){return game_history;}

    bool is_adjacent(Board& board, Cell cell);//determines if cell is adjacent to other cells

    /*Intended to be used with the player AI does not update history or validate piece, to optimize minimax algorithm
    pieces placed by this function have already been
    validated with the get_moves() function, DO NOT USE THIS FUNCTION OUTSIDE OF PLAYER OR BEFORE get_moves()*/
    void placeValidatedPiece(int x, int y);



	std::string finish(Cell::STATE state);

public:
    bool game_won;
    std::string error_message;
//constructors
//----------------------------------------------------------------------
	Board() {
        game_won=false;
		timer.start();
		turn = Cell::BLACK;
		moves = 0;
		display = true;
		for(int i = 1; i < 16; i++) {
			for(int j = 1; j < 16; j++) {
				cells[i-1][j-1].setState(Cell::EMPTY);
				cells[i-1][j-1].setPosition(i, j);
			}
		}
	}
    Board(Board& b){
        for(int i=0;i<15;++i)
            for (int j=0;j<15;++j)
                cells[i][j]=b.cells[i][j];
        turn=b.turn;
        moves=b.moves;
        timer=b.timer;
        display=b.display;
        game_history=b.game_history;
        undo_history=b.undo_history;
        game_won=b.game_won;
        error_message=b.error_message;
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

    std::vector<Cell> get_moves(Board& board);//returns vector of moves that are valid and also adjacent
    bool is_board_empty();

	//print out board's current state
	friend std::ostream& operator<<(std::ostream &out, Board &board);
	std::string get_string_board(Board& board);
	//read into the board from a stream
	friend std::istream& operator>>(std::istream &in, Board &board);
	friend class Player;
	int evaluate_board(bool isWhite);
};

#endif
