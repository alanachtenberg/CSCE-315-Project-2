#include <vector>
#include "Cell.h"
#include "Player.h"

class Board {
private:
	//board as 2D array of cells
	Cell cells[15][15];

	//2 players only
	Player black, white;

	//which player's turn is it?
	//  black = 0
	//  white = 1
	int turn;

public:
	Board() {
	}


	void setCell(int x, int y, Cell::STATE state);
	Cell getCell(int x, int y) { return cells[x-1][y-1]; }
	int gettt() { return 0; }

	friend std::ostream& operator<<(std::ostream &out, Board &board);
};
