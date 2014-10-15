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
		for(int i = 1; i <= 15; i++) {
			for(int j = 1; j <= 15; j++) {
				cells[i-1][j-1].setState(Cell::EMPTY);
				cells[i-1][j-1].setPosition(i, j);
			}
		}
	}

	void setCell(int x, int y, Cell::STATE state);
	Cell getCell(int x, int y) { return cells[x-1][y-1]; }
	int checkPath(Cell start, int count, Cell::DIRECTION direction);
	int checkPath(Cell start, Cell::DIRECTION direction);
	bool isMoveValid(Cell start);
	void finish(Cell::STATE state);

	friend std::ostream& operator<<(std::ostream &out, Board &board);
};
