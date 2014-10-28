#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include <exception>
#include <limits.h>
#include <stdexcept>
#include <string>
#include <sstream>
#include "Board.h"
#include <algorithm>

using namespace std;

//internal helper functions
//----------------------------------------------------------------------
Cell Board::getCell(int x, int y) {
	if(x > 0 && y > 0 && x <= 15 && y <= 15) {
		return cells[x-1][y-1]; // we must range check greater than 1 instead of 0
	}
	else {
		Cell cell;
		cell.setState(Cell::OFF_BOARD);
		return cell;
	}
}
/*Intended to be used with the player AI, to optimize minimax algorithm, pieces placed by this function have already been
validated with the get_moves() function, DO NOT USE THIS FUNCTION OUTSIDE OF PLAYER*/
void Board::placeValidatedPiece(int x, int y){
    Cell cell;
    cell.setState(turn);
    cell.setPosition(x, y);
    cells[x-1][y-1] = cell;
    if(turn == Cell::WHITE) {
        turn = Cell::BLACK;
    }
    else {
        turn = Cell::WHITE;
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
    default:
        error_message= "Unknown isMoveValid result\n";
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
	// convert the strings to lowercase so that it is case insensitive
	std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
	//we have a comment, ignore rest of line
	if(cmd.at(0) == ';') ;
	//we have a move
	else {
		if(cmd == "undo") return undo();
		else if(cmd == "redo") return redo();
		else if(cmd == "exit") exit(0);
		else if(cmd == "display") display = (display) ? false : true;
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

string Board::game_mode(std::string gm){
	if(gm == "1") return "1";
	else if(gm == "2") return "2";
	else if(gm == "3") return "3";
	else{
		error_message = "INVALID SELECTION\n";
		return "ERROR";
	}
}

string Board::AI_difficulty(std::string dif){
	std::transform(dif.begin(), dif.end(), dif.begin(), ::tolower);		// make case insensitive
	if(dif == "easy") return "EASY";
	else if(dif == "medium") return "MEDIUM";
	else if(dif == "hard") return "HARD";
	else{
		error_message = "INVALID DIFFICULTY\n";
		return "ERROR";
	}
}
//--------------------------------
//--------------------------------
bool Board::is_adjacent(Board& board, Cell cell){//checks if cell is adjacent
        int x=cell.getX();
        int y=cell.getY();
        if (x==15)
            x=15;
        Cell::STATE temp;
        //removed vector implementation
        //used these ridiculous if statements to improve the efficiency
        //ordered it NSEW first because it is more likely to get a hit on those if it is an edge piece
        temp=board.getCell(x,y-1).getState();
        if (temp!=Cell::EMPTY && temp!=Cell::OFF_BOARD)    //N cell
            return true;
        temp=board.getCell(x,y+1).getState();
        if (temp!=Cell::EMPTY && temp!=Cell::OFF_BOARD)    //S cell
            return true;
        temp=board.getCell(x+1,y).getState();
        if (temp!=Cell::EMPTY && temp!=Cell::OFF_BOARD)    //E cell
            return true;
        temp=board.getCell(x-1,y).getState();
        if (temp!=Cell::EMPTY && temp!=Cell::OFF_BOARD)    //W cell
            return true;
        temp=board.getCell(x-1,y-1).getState();
        if (temp!=Cell::EMPTY && temp!=Cell::OFF_BOARD)    //NW cell
            return true;
        temp=board.getCell(x+1,y-1).getState();
        if (temp!=Cell::EMPTY && temp!=Cell::OFF_BOARD)    //NE cell
            return true;
        temp=board.getCell(x-1,y+1).getState();
        if (temp!=Cell::EMPTY && temp!=Cell::OFF_BOARD)    //SW cell
            return true;
        temp=board.getCell(x+1,y+1).getState();
        if (temp!=Cell::EMPTY && temp!=Cell::OFF_BOARD)    //SE cell
            return true;


        return false;

}

/*determines adjacent moves first and then valid moves from those that are adjacent because
 isMoveValid is a more expensive operation than is adjacent*/
std::vector<Cell> Board::get_moves(Board& board){//returns vector of moves that are valid and also adjacent
   //Timer test_timer;
   // test_timer.start();
    std::vector<Cell> adj_moves; //determine adjacent moves first
    for (int i=1; i<16;++i){ //because get cell returns x-1 y-1, there are 15 positions
        for (int j=1; j<16; ++j){
            if (board.getCell(i,j).getState()==Cell::EMPTY);//ignore cells that are not empty
                if (is_adjacent(board, board.getCell(i,j))) //check  if adjacent
                    adj_moves.push_back(board.getCell(i,j)); // save cell
        }
    }

    std::vector<Cell> valid_moves; //create new list of valid moves from adjacent moves
    for (unsigned int k=0; k<adj_moves.size();++k){
        int val=isMoveValid(adj_moves[k].setState(getTurn()));
        if(val==0||val==1)
            valid_moves.push_back(adj_moves[k]);
    }

    //test_timer.finish();
    //cout<<"TIMER SEC VAL "<<test_timer.sec()<<" "<<"USEC VAL "<<test_timer.usec()<<endl;
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

int Board::evaluate_board(bool isWhite){
    //cout<<*this;
	int total=0;
	int good_val=0, evil_val=0;
	for(int i=1;i<16;++i)
        for (int j=1;j<16;++j)
            {
                int dirs[8];
                bool dirs_open[8];
                Cell cell= getCell(i,j);
                if (isWhite&&cell.getState()==Cell::WHITE || !isWhite && cell.getState()==Cell::BLACK){
                    dirs[0] = checkPath(cell, Cell::N);
                    dirs[1] = checkPath(cell, Cell::NE);
                    dirs[2] = checkPath(cell, Cell::E);
                    dirs[3] = checkPath(cell, Cell::SE);
                    dirs[4] = checkPath(cell, Cell::S);
                    dirs[5] = checkPath(cell, Cell::SW);
                    dirs[6] = checkPath(cell, Cell::W);
                    dirs[7] = checkPath(cell, Cell::NW);

                    dirs_open[0]=getCell(cell.getX(),cell.getY()-dirs[0]).getState()==Cell::EMPTY;
                    dirs_open[1]=getCell(cell.getX()+dirs[1],cell.getY()-dirs[1]).getState()==Cell::EMPTY;
                    dirs_open[2]=getCell(cell.getX()+dirs[2],cell.getY()).getState()==Cell::EMPTY;
                    dirs_open[3]=getCell(cell.getX()+dirs[3],cell.getY()+dirs[3]).getState()==Cell::EMPTY;
                    dirs_open[4]=getCell(cell.getX(),cell.getY()+dirs[4]).getState()==Cell::EMPTY;
                    dirs_open[5]=getCell(cell.getX()-dirs[5],cell.getY()+dirs[5]).getState()==Cell::EMPTY;
                    dirs_open[6]=getCell(cell.getX()-dirs[6],cell.getY()).getState()==Cell::EMPTY;
                    dirs_open[7]=getCell(cell.getX()-dirs[7],cell.getY()-dirs[7]).getState()==Cell::EMPTY;
                    int max_our = 0;
                    int N_to_S = dirs[0] + dirs[4]-1;// minus 1 because middle vaule is counted twice
                    int NE_to_SW = dirs[1] + dirs[5]-1;
                    int E_to_W = dirs[2] + dirs[6]-1;
                    int SE_to_NW = dirs[3] + dirs[7]-1;

                    bool N_to_S_open = dirs_open[0] && dirs_open[4];// minus 1 because middle vaule is counted twice
                    bool NE_to_SW_open = dirs_open[1] && dirs_open[5];
                    bool E_to_W_open = dirs_open[2] && dirs_open[6]-1;
                    bool SE_to_NW_open = dirs_open[3] && dirs_open[7]-1;

                    int axis[4];
                    bool axis_open[4];

                    axis[0]=N_to_S;
                    axis[1]= NE_to_SW;
                    axis[2]= E_to_W;
                    axis[3]= SE_to_NW;

                    axis_open[0]=N_to_S_open;
                    axis_open[1]= NE_to_SW_open;
                    axis_open[2]= E_to_W_open;
                    axis_open[3]= SE_to_NW_open;

                    int five=0;
                    int live_four=0, sleep_four=0, double_sleep_four=0;
                    int live_three=0, sleep_three=0, double_sleep_three=0;
                    int live_two=0, sleep_two=0, double_sleep_two=0;

                    for (int i=0;i<4;++i){
                        if(axis[i]==5) five++;

                        else if(axis[i]==4 && (dirs_open[i] && dirs_open[i+4])) live_four++;
                        else if(axis[i]==4 && (dirs_open[i] || dirs_open[i+4])) sleep_four++;

                        else if(axis[i]==3 && (dirs_open[i] && dirs_open[i+4])) live_three++;
                        else if(axis[i]==3 && (dirs_open[i] || dirs_open[i+4])) sleep_three++;

                        else if(axis[i]==2 && (dirs_open[i] && dirs_open[i+4])) live_two++;
                        else if(axis[i]==2 && (dirs_open[i] || dirs_open[i+4])) sleep_two++;
                    }

                    if (five>0)
                        good_val = max(100000,good_val);
                    else if((live_four>=4) || (sleep_four>=8) || (sleep_four>=4 && live_three>=3))
                        good_val =  max(10000,good_val);
                    else if(live_three >=6)
                        good_val =  max(5000,good_val);
                    else if((live_three >=3) && (sleep_three >=3))
                        good_val =  max(1000,good_val);
                    else if((sleep_four >=4))
                        good_val =  max(500,good_val);
                    else if((live_three >=3))
                        good_val =  max(200,good_val);
                    else if((live_two >=4))
                        good_val =  max(100,good_val);
                    else if(sleep_three >=3)
                        good_val =  max(50,good_val);
                    else if(sleep_two >=4)
                        good_val =  max(10,good_val);
                    else if(live_two >=2)
                        good_val =  max(5,good_val);
                    else if(sleep_two >=2)
                        good_val =  max(3,good_val);
                 //   else
                 //       good_val = 0;
                }
                if (isWhite&&cell.getState()==Cell::BLACK || !isWhite && cell.getState()==Cell::WHITE){
                    dirs[0] = checkPath(cell, Cell::N);
                    dirs[1] = checkPath(cell, Cell::NE);
                    dirs[2] = checkPath(cell, Cell::E);
                    dirs[3] = checkPath(cell, Cell::SE);
                    dirs[4] = checkPath(cell, Cell::S);
                    dirs[5] = checkPath(cell, Cell::SW);
                    dirs[6] = checkPath(cell, Cell::W);
                    dirs[7] = checkPath(cell, Cell::NW);

                    dirs_open[0]=getCell(cell.getX(),cell.getY()-dirs[0]).getState()==Cell::EMPTY;
                    dirs_open[1]=getCell(cell.getX()+dirs[1],cell.getY()-dirs[1]).getState()==Cell::EMPTY;
                    dirs_open[2]=getCell(cell.getX()+dirs[2],cell.getY()).getState()==Cell::EMPTY;
                    dirs_open[3]=getCell(cell.getX()+dirs[3],cell.getY()+dirs[3]).getState()==Cell::EMPTY;
                    dirs_open[4]=getCell(cell.getX(),cell.getY()+dirs[4]).getState()==Cell::EMPTY;
                    dirs_open[5]=getCell(cell.getX()-dirs[5],cell.getY()+dirs[5]).getState()==Cell::EMPTY;
                    dirs_open[6]=getCell(cell.getX()-dirs[6],cell.getY()).getState()==Cell::EMPTY;
                    dirs_open[7]=getCell(cell.getX()-dirs[7],cell.getY()-dirs[7]).getState()==Cell::EMPTY;
                    int max_our = 0;
                    int N_to_S = dirs[0] + dirs[4]-1;// minus 1 because middle vaule is counted twice
                    int NE_to_SW = dirs[1] + dirs[5]-1;
                    int E_to_W = dirs[2] + dirs[6]-1;
                    int SE_to_NW = dirs[3] + dirs[7]-1;

                    bool N_to_S_open = dirs_open[0] && dirs_open[4];// minus 1 because middle vaule is counted twice
                    bool NE_to_SW_open = dirs_open[1] && dirs_open[5];
                    bool E_to_W_open = dirs_open[2] && dirs_open[6]-1;
                    bool SE_to_NW_open = dirs_open[3] && dirs_open[7]-1;

                    int axis[4];
                    bool axis_open[4];

                    axis[0]=N_to_S;
                    axis[1]= NE_to_SW;
                    axis[2]= E_to_W;
                    axis[3]= SE_to_NW;

                    axis_open[0]=N_to_S_open;
                    axis_open[1]= NE_to_SW_open;
                    axis_open[2]= E_to_W_open;
                    axis_open[3]= SE_to_NW_open;

                    int five=0;
                    int live_four=0, sleep_four=0, double_sleep_four=0;
                    int live_three=0, sleep_three=0, double_sleep_three=0;
                    int live_two=0, sleep_two=0, double_sleep_two=0;

                    for (int i=0;i<4;++i){
                        if(axis[i]==5) five++;

                        else if(axis[i]==4 && (dirs_open[i] && dirs_open[i+4])) live_four++;
                        else if(axis[i]==4 && (dirs_open[i] || dirs_open[i+4])) sleep_four++;

                        else if(axis[i]==3 && (dirs_open[i] && dirs_open[i+4])) live_three++;
                        else if(axis[i]==3 && (dirs_open[i] || dirs_open[i+4])) sleep_three++;

                        else if(axis[i]==2 && (dirs_open[i] && dirs_open[i+4])) live_two++;
                        else if(axis[i]==2 && (dirs_open[i] || dirs_open[i+4])) sleep_two++;
                    }

                    if (five>0)
                         evil_val = min(-100000,evil_val);
                    else if((live_four>=4) || (sleep_four>=8) || (sleep_four>=4 && live_three>=3))
                         evil_val = min(-10000,evil_val);
                    else if(live_three >=6)
                         evil_val = min(-5000,evil_val);
                    else if((live_three >=3) && (sleep_three >=3))
                         evil_val = min(-1000,evil_val);
                    else if((sleep_four >=4))
                         evil_val = min(-500,evil_val);
                    else if((live_three >=3))
                         evil_val = min(-200,evil_val);
                    else if((live_two >=4))
                         evil_val = min(-100,evil_val);
                    else if(sleep_three >=3)
                         evil_val = min(-50,evil_val);
                    else if(sleep_two >=4)
                         evil_val = min(-10,evil_val);
                    else if(live_two >=2)
                         evil_val = min(-5,evil_val);
                    else if(sleep_two >=2)
                         evil_val = min(-3,evil_val);
                }


            }
            if(good_val > evil_val*-1)
                    return good_val;
                else
                    return evil_val;
}
