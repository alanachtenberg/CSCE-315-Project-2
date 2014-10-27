#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include <exception>
#include <stdexcept>
#include <string>
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

string Board::placePiece(int x, int y) {
	Cell cell;
	cell.setState(turn);
	cell.setPosition(x, y);

	stringstream out;

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
		if(display) out << *this;
		return out.str();
	case 1:
		//set cell to current piece
		cells[x-1][y-1] = cell;
		game_history.push(cell);
		if(turn == Cell::WHITE) {
		}
		else {
			moves++;
		}
		if(display) out << *this;
        game_won=true;
		out << finish(turn);
		return out.str();
	case 2:
		error_message = "Position already taken\n";
		return "ERROR";
	case 3:
		error_message = "Cannot place a piece that would simultaneously form multiple open rows of three\n";
		return "ERROR";
	case 4:
		error_message = "Cannot place an empty piece\n";
		return "ERROR";
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

string Board::finish(Cell::STATE state) {
	timer.finish();

	stringstream out;

	if(state == Cell::BLACK) out << "Black ";
	else out << "White ";

	out << "player has won the game!\n";
	out << "***STATS***\n";
	out << "Game Time: " << timer << "\n";
	out << "Moves: " << moves << "\n";
	return out.str();
}

string Board::undo() {
	if(game_history.size() > 1) {
        Cell cell = game_history.top();//undo AI move
        cells[cell.getX()-1][cell.getY()-1].setState(Cell::EMPTY);
        game_history.pop();
        undo_history.push(cell);//save ai move for redo

        cell = game_history.top();
		game_history.pop();//undo user move
		cells[cell.getX()-1][cell.getY()-1].setState(Cell::EMPTY);

        //In case of undo the turn should not change!
		//turn = (turn == Cell::WHITE) ? turn = Cell::BLACK : turn = Cell::WHITE;
		undo_history.push(cell); //save user move for redo
		return "";
	}
	else {
		return "No moves to undo\n";
	}
}

string Board::redo() {
	if(undo_history.size() > 1) {
		Cell cell = undo_history.top(); //redo user move
		undo_history.pop();
		cells[cell.getX()-1][cell.getY()-1].setState(cell.getState());
        game_history.push(cell);

        cell = undo_history.top(); //redo AI move
		undo_history.pop();
		cells[cell.getX()-1][cell.getY()-1].setState(cell.getState());
		//turn should not change because redo both moves
	//	turn = (turn == Cell::WHITE) ? turn = Cell::BLACK : turn = Cell::WHITE;
		game_history.push(cell);
		return "";
	}
	else {
		return "No moves to redo\n";//return instead of cout
	}
}

string Board::command(std::string cmd) {
	//we have a comment, ignore rest of line
	if(cmd.at(0) == ';') ;
	//we have a move
	else {
		if(cmd == "UNDO") return undo();
		else if(cmd == "REDO") return redo();
		else if(cmd == "EXIT") exit(0);
		else if(cmd == "DISPLAY") display = (display) ? false : true;
		else if (cmd.size()==2){
			char x_char, y_char;
			x_char = cmd.at(0);
			y_char = cmd.at(1);

			int x, y;
			std::stringstream ss;
			ss << x_char << " " << y_char;
			ss >> hex >> x;
			ss >> hex >> y;
			if ((x>16||x<1)  || (y<1||y>16)){
				error_message= "ERROR OUT OF RANGE\n";
				return "ERROR";
			}
			return placePiece(x, y);
		}
		else{
			error_message="INVALID COMMAND\n";
			return "ERROR";
		}
	}

	return "";
}
//--------------------------------
//--------------------------------

void Board::random_ai(){
    Cell cell = game_history.top();

	bool moved = false;								// used for while loop determining where to move

	int dirs[8];
	dirs[0] = checkPath(cell, Cell::N);
	dirs[1] = checkPath(cell, Cell::NE);
	dirs[2] = checkPath(cell, Cell::E);
	dirs[3] = checkPath(cell, Cell::SE);
	dirs[4] = checkPath(cell, Cell::S);
	dirs[5] = checkPath(cell, Cell::SW);
	dirs[6] = checkPath(cell, Cell::W);
	dirs[7] = checkPath(cell, Cell::NW);

	int N_to_S = dirs[0] + dirs[4]-1;// minus 1 because middle vaule is counted twice
	int NE_to_SW = dirs[1] + dirs[5]-1;
	int E_to_W = dirs[2] + dirs[6]-1;
	int SE_to_NW = dirs[3] + dirs[7]-1;
	int random_X;									// In case AI makes random move we will need this
	int random_Y;									// In case AI makes random move we will need this


	if(N_to_S >= 3){
		// if N can be covered cover
		if(getCell(cell.getX(), cell.getY() - dirs[0]).getState() == Cell::EMPTY){
			placePiece(cell.getX(), cell.getY() - dirs[0]);
			moved = true;
		}
		// if S can be covered cover
		else if(getCell(cell.getX(), cell.getY() + dirs[4]).getState() == Cell::EMPTY){
			placePiece(cell.getX(), cell.getY() + dirs[4]);
			moved = true;
		}
	}
	else if(NE_to_SW >= 3){
		// if NE can be covered cover
		if(getCell(cell.getX() + dirs[1], cell.getY() - dirs[1]).getState() == Cell::EMPTY){
			placePiece(cell.getX() + dirs[1], cell.getY() - dirs[1]);
			moved = true;
		}
		// if SW can be covered cover
		else if(getCell(cell.getX() - dirs[5], cell.getY() + dirs[5]).getState() == Cell::EMPTY){
			placePiece(cell.getX() - dirs[5], cell.getY() + dirs[5]);
			moved = true;
		}
	}
	else if(E_to_W >= 3){
		// if E can be covered cover
		if(getCell(cell.getX() + dirs[2], cell.getY()).getState() == Cell::EMPTY){
			placePiece(cell.getX() + dirs[2], cell.getY());
			moved = true;
		}
		// if W can be covered cover
		else if(getCell(cell.getX() - dirs[6], cell.getY()).getState() == Cell::EMPTY){
			placePiece(cell.getX() - dirs[6], cell.getY());
			moved = true;
		}
	}
	else if(SE_to_NW >= 3){
		// if SE can be covered cover
		if(getCell(cell.getX() + dirs[3], cell.getY() + dirs[3]).getState() == Cell::EMPTY){
			placePiece(cell.getX() + dirs[3], cell.getY() + dirs[3]);
			moved = true;
		}
		// if NW can be covered cover
		else if(getCell(cell.getX() - dirs[7], cell.getY() - dirs[6]).getState() == Cell::EMPTY){
			placePiece(cell.getX() - dirs[7], cell.getY() - dirs[6]);
			moved = true;
		}
	}

	while(!moved){				// If no obvious moves or cant cover  make random move
		random_X = 1 + rand() % 15;		// create a random number from 1 - 15 for X
		random_Y = 1 + rand() % 15;		// create a random number from 1 - 15 for Y
		if(getCell(random_X, random_Y).getState() == Cell::EMPTY){
			placePiece(random_X, random_Y);
			moved = true;
		}
	}

}

bool Board::is_adjacent(Board& board, Cell cell){//checks if cell is adjacent
        std::vector<Cell> adjacent_cells;
        int x=cell.getX();
        int y=cell.getY();

        adjacent_cells.push_back(board.getCell(x,y-1));    //N cell
        adjacent_cells.push_back(board.getCell(x-1,y-1));   //NW cell
        adjacent_cells.push_back(board.getCell(x+1,y-1));   //NE cell
        adjacent_cells.push_back(board.getCell(x,y+1));     //S cell
        adjacent_cells.push_back(board.getCell(x-1,y+1));   //SW cell
        adjacent_cells.push_back(board.getCell(x+1,y+1));   //SE cell
        adjacent_cells.push_back(board.getCell(x-1,y));     //W cell
        adjacent_cells.push_back(board.getCell(x+1,y));     //E cell

        for (unsigned int i=0;i<adjacent_cells.size();++i)//check if cells are occupied
            if (adjacent_cells[i].getState()!=Cell::OFF_BOARD && adjacent_cells[i].getState()!=cell.EMPTY)
                return true;
        return false;

}
/*determines adjacent moves first and then valid moves from those that are adjacent because
 isMoveValid is a more expensive operation than is adjacent*/
 std::vector<Cell> Board::get_moves(Board& board){//returns vector of moves that are valid and also adjacent

    std::vector<Cell> adj_moves; //determine adjacent moves first
    for (int i=0; i<15;++i)
        for (int j=0; j<15; ++j){
            if (board.getCell(i,j).getState()!=Cell::EMPTY);//ignore cells that are not empty
                if (is_adjacent(board, board.getCell(i,j))) //check  if adjacent
                    adj_moves.push_back(board.getCell(i,j)); // save cell
        }

    std::vector<Cell> valid_moves; //create new list of valid moves from adjacent moves
    for (unsigned int k=0; k<adj_moves.size();++k){
        if(isMoveValid(adj_moves[k].setState(getTurn()))==0)
            valid_moves.push_back(adj_moves[k]);
    }

    return valid_moves;

 }

 bool Board::is_board_empty(){
    if (game_history.size()==0)
        return true;
    else
        return false;
 }
//--------------------------------
//--------------------------------

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

string Board::get_string_board(Board& board) {
	stringstream board_ss;
	board_ss<<board;
	return board_ss.str();
}


istream& operator>>(istream &in, Board &board) {
	std::string cmd;
	in >> cmd;
	if(cmd.at(0) == ';') {
		getline(in, cmd);
		cmd = ";" + cmd;
	}
	board.command(cmd);

	return in;
}
