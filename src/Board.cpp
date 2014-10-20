#include <iomanip>
#include <exception>
#include <stdexcept>
#include <sstream>
#include "Board.h"

using namespace std;

//internal helper functions
//----------------------------------------------------------------------
Cell Board::getCell(int x, int y) {
	if(x > 0 && y > 0 && x < 16 && y < 16) {
		return cells[x-1][y-1];
	}
	else {
		Cell cell;
		cell.setState(Cell::OFF_BOARD);
		return cell;
	}
}

void Board::placePiece(int x, int y) {
	Cell cell;
	cell.setState(turn);
	cell.setPosition(x, y);
	switch(isMoveValid(cell)) {
	case 0:
		//set cell to current piece
		cells[x-1][y-1] = cell;
		game_history.push(cell);
		if(turn == Cell::WHITE) {
			turn = Cell::BLACK;
		}
		else {
			moves++;
			turn = Cell::WHITE;
		}
		if(display) cout << *this;
		break;
	case 1:
		//set cell to current piece
		cells[x-1][y-1] = cell;
		game_history.push(cell);
		if(turn == Cell::WHITE) {
		}
		else {
			moves++;
		}
		if(display) cout << *this;

		finish(turn);
		break;
	case 2:
		cout << "Position (" << x << ", " << y << ") already taken\n";
		break;
	case 3:
		cout << "Cannot place a piece that would simultaneously form multiple open rows of three\n";
		break;
	case 4:
		cout << "Cannot place an empty piece\n";
		break;
	}
}

//wrapper to make it easy to use, recursive step needs count which the caller
//should not need to see, and doesn't have to remember the starting value
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

	//get next cell. If it is the same color as the current cell, recurse
	//if it is a different color, empty, or off the board, return count
	Cell nextCell = getCell(nextX, nextY);
	if(nextCell.getState() == start.getState()) {
		count++;
		return checkPath(getCell(nextX, nextY), count, direction);
	}
	else return count;
}

//Return value:
//	0: Move is valid
//	1: Move is valid, and finishes the game
//	2: Position is already taken
//	3: Rule of 3 would be violated
//	4: User tried to place an empty piece (should never happen)
int Board::isMoveValid(Cell cell) {
	if(getCell(cell.getX(), cell.getY()).getState() != Cell::EMPTY) {
		return 2; //position already taken
	}
	else if(cell.getState()  == Cell::EMPTY) {
		return 4; //user tried to place an empty piece
	}
	//get the number of pieces in all directions, and whether it is an
	//open row, in order of N, NE, E, SE, S, SW, W, NW
	int dirs[8];
	dirs[0] = checkPath(cell, Cell::N);
	dirs[1] = checkPath(cell, Cell::NE);
	dirs[2] = checkPath(cell, Cell::E);
	dirs[3] = checkPath(cell, Cell::SE);
	dirs[4] = checkPath(cell, Cell::S);
	dirs[5] = checkPath(cell, Cell::SW);
	dirs[6] = checkPath(cell, Cell::W);
	dirs[7] = checkPath(cell, Cell::NW);

	bool open[8];
	open[0] = (getCell(cell.getX(), 		  cell.getY() - dirs[0]).getState() == Cell::EMPTY);
	open[1] = (getCell(cell.getX() + dirs[1], cell.getY() - dirs[1]).getState() == Cell::EMPTY);
	open[2] = (getCell(cell.getX() + dirs[2], cell.getY()    	   ).getState() == Cell::EMPTY);
	open[3] = (getCell(cell.getX() + dirs[3], cell.getY() + dirs[3]).getState() == Cell::EMPTY);
	open[4] = (getCell(cell.getX(), 		  cell.getY() + dirs[4]).getState() == Cell::EMPTY);
	open[5] = (getCell(cell.getX() - dirs[5], cell.getY() + dirs[5]).getState() == Cell::EMPTY);
	open[6] = (getCell(cell.getX() - dirs[6], cell.getY()   	   ).getState() == Cell::EMPTY);
	open[7] = (getCell(cell.getX() - dirs[7], cell.getY() - dirs[6]).getState() == Cell::EMPTY);

	//check for winning condition of a row of exactly 5
	for(int i = 0; i < 4; i++) {
		if((dirs[i] + dirs[i+4] - 1) == 5) {
			return 1; //game is over, whover just played has won
		}
	}

	//check the surrounding pieces for the threes rule
	int threes_rule = 0;
	for(int i = 0; i < 8; i++) {
		if(dirs[i] == 4 && open[i]) {
			threes_rule++;
		}
	}

	if(threes_rule >= 2) {
		Cell test_for_loss = cell;
		if(cell.getState() == Cell::BLACK)
			test_for_loss.setState(Cell::WHITE);
		else
			test_for_loss.setState(Cell::BLACK);

		if(isMoveValid(test_for_loss) == 1) {
			//player would be forced to place here or else lose the game
			return 0;
		}
		else {
			//player truly has violated the double threes rule
			return 3;
		}
	}

	return 0;
}

void Board::finish(Cell::STATE state) {
	timer.finish();

	if(state == Cell::BLACK) cout << "Black ";
	else cout << "White ";

	cout << "player has won the game!\n";
	cout << "***STATS***\n";
	cout << "Game Time: " << timer << "\n";
	cout << "Moves: " << moves << "\n";
	exit(0);
}

void Board::undo() {
	if(game_history.size() > 0) {
		Cell cell = game_history.top();
		game_history.pop();
		cells[cell.getX()-1][cell.getY()-1].setState(Cell::EMPTY);

		turn = (turn == Cell::WHITE) ? turn = Cell::BLACK : turn = Cell::WHITE;
		undo_history.push(cell);
	}
	else {
		cout << "No moves to undo\n";
	}
}

void Board::redo() {
	if(game_history.size() > 0) {
		Cell cell = undo_history.top();
		undo_history.pop();
		cells[cell.getX()-1][cell.getY()-1].setState(cell.getState());

		turn = (turn == Cell::WHITE) ? turn = Cell::BLACK : turn = Cell::WHITE;
		game_history.push(cell);
	}
	else {
		cout << "No moves to redo\n";
	}
}

bool Board::command(std::string cmd) {
	//we have a comment, ignore rest of line
	if(cmd.at(0) == ';') ;
	//we have a move
	else if(cmd.size() == 2) {
		char x_char, y_char;
		x_char = cmd.at(0);
		y_char = cmd.at(1);

		int x, y;
		std::stringstream ss;
		ss << x_char << " " << y_char;
		ss >> hex >> x;
		ss >> hex >> y;
		placePiece(x, y);
	}
	else if(cmd == "UNDO") undo();
	else if(cmd == "REDO") redo();
	else if(cmd == "EXIT") exit(0);
	else if(cmd == "DISPLAY") display = (display) ? false : true;
	else return false;

	return true;
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

istream& operator>>(istream &in, Board &board) {
	std::string cmd;
	in >> cmd;
	if(cmd.at(0) == ';') {
		getline(in, cmd);
		cmd = ";" + cmd;
	}
	board.command(cmd);
}
