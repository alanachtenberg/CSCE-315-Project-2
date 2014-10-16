#include <stack>
#include "Cell.h"
#include "Player.h"

class Board {
private:
//Data members
//----------------------------------------------------------------------
	//board as 2D array of cells
	Cell cells[15][15];

	//stacks for undo, redo
	std::stack<Board> game_history, undo_history;

	//2 players only
	Player black, white;

	//which player's turn is it?
	Cell::STATE turn;

//internal helper functions
//----------------------------------------------------------------------
	int isMoveValid(Cell start);
	void setCell(int x, int y, Cell::STATE state);
	Cell getCell(int x, int y);

	int checkPath(Cell start, int count, Cell::DIRECTION direction);
	int checkPath(Cell start, Cell::DIRECTION direction);

	void finish(Cell::STATE state);

public:
//constructors
//----------------------------------------------------------------------
	Board() {
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
	void placePiece(int x, int y);
	void undo();
	void redo();
	void setDifficulty(Player::DIFFICULTY difficulty);


	friend std::ostream& operator<<(std::ostream &out, Board &board);
};
