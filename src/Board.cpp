#include <iomanip>
#include <exception>
#include "Board.h"

using namespace std;

void Board::setCell(int x, int y, Cell::STATE state) {
	if(getCell(x, y).getState() != Cell::EMPTY) {
		//throw exception
		exception ex;
		throw ex;
	}
	else {
		cells[x-1][y-1].setState(state);
	}
}





ostream& operator<<(ostream &out, Board &board) {

	out << "  ";
	for(int i = 1; i < 16; i++) {
		out << hex << i << " ";
	}
	out << "\n  -------------------------------\n";

	for(int i = 1; i < 16; i++) {
		out << hex << i << "|";
		for(int j = 1; j < 16; j++) {
			//swap loop indices to display board correctly across columns, not rows
			Cell c = board.getCell(j, i);
			out << c << " ";
		}
		out << "\n";
	}

	return out;
}
