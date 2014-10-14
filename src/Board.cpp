#include <iomanip>
#include <exception>
#include <stdexcept>
#include <sstream>
#include "Board.h"

using namespace std;

void Board::setCell(int x, int y, Cell::STATE state) {
	if(getCell(x, y).getState() != Cell::EMPTY) {
		//throw exception
		stringstream message;
		message << "Position (" << x << ", " << y << ") already taken";
		throw runtime_error(message.str());
	}
	else if(state == Cell::EMPTY) {
		stringstream message;
		message << "Cannot place an empty piece";
		throw runtime_error(message.str());
	}
	else {
		cells[x-1][y-1].setState(state);
		cells[x-1][y-1].setPosition(x, y);
	}
}

//wrapper to make it easy to check, recursive step needs count which the
//caller should not need to see
int Board::checkPath(Cell start, Cell::DIRECTION direction) {
		return checkPath(start, 1, direction);
}

//Checks to see how many cells in a row are the same color.
//Given an initial starting point, it recursively calls this function
//on a cells children, which become the new start.
int Board::checkPath(Cell start, int count, Cell::DIRECTION direction) {
	//recursively defined
	//BASE CASE: cell in particular direction is of different color, or is off the board
	//RECURSIVE STEP: go to next cell in particular direction, incrementing count

	//determine position of next piece, based on direction flag
	int nextX = 0;
	int nextY = 0;
	switch(direction) {
		case Cell::N:
			nextX = start.getX();
			nextY = start.getY() - 1;
			break;
		case Cell::NE:
			nextX = start.getX() + 1;
			nextY = start.getY() - 1;
			break;
		case Cell::E:
			nextX = start.getX() + 1;
			nextY = start.getY();
			break;
		case Cell::SE:
			nextX = start.getX() + 1;
			nextY = start.getY() + 1;
			break;
		case Cell::S:
			nextX = start.getX();
			nextY = start.getY() + 1;
			break;
		case Cell::SW:
			nextX = start.getX() - 1;
			nextY = start.getY() + 1;
			break;
		case Cell::W:
			nextX = start.getX() - 1;
			nextY = start.getY();
			break;
		case Cell::NW:
			nextX = start.getX() - 1;
			nextY = start.getY() - 1;
			break;
	}

	//next piece is off the board, return current count
	if(nextX > 15 || nextX < 1 ||  nextY > 15 || nextY < 1) return count;

	//get next cell. If it is the same color as the current cell, recurse
	Cell nextCell = getCell(nextX, nextY);
	if(nextCell.getState() == start.getState()) {
		count++;
		return checkPath(getCell(nextX, nextY), count, direction);
	}
	else return count;
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
			//swap loop indices to display board correctly across rows, not columns
			Cell c = board.getCell(j, i);
			out << c << " ";
		}
		out << "\n";
	}

	return out;
}
