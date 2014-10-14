#include "Cell.h"


using namespace std;

ostream& operator<<(ostream &out, Cell &cell) {

	switch(cell.getState()) {
	case Cell::WHITE:
		out << "O";
		break;
	case Cell::BLACK:
		out << "@";
		break;
	case Cell::EMPTY:
	default:
		out << ".";
		break;
	}

	return out;
}

