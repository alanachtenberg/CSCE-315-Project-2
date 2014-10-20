#include <iostream>
#include <iomanip>
#include <exception>
#include <stdexcept>
#include <unistd.h>
#include "Board.h"
#include "Server.h"
using namespace std;

void user_move(Server &myServer, Board &board) {
	myServer.send_msg("\nYour Move:\nPick a position to place piece [1-f][1-f]: ");
	string msg = myServer.read_msg();
	board.command(msg); 
}

void run() { //send the board to and ask for a command
                        //when i gets a command back it will execute the command on the board and will choose a random other move
                        // and send the board state back to the client and ask for another move
                        //this happens until an end condition
	Board board;
   	Server myServer(2323);
   	while(true){ //win condition
   		myServer.send_msg(board.get_string_board(board));
   		user_move(myServer, board);
   		myServer.send_msg(board.get_string_board(board));
   		//Insert AI MOVE function here
   	}



}


int main(int argc, char** argv) {
		Board b;
		
		try {
			//base
            //string mesg=myServer.read_msg();
            run();
           // cout<<mesg<<endl;
			
			return 0;
		}
		catch(runtime_error e) {
			cerr << e.what() << "\n";
		}
}
