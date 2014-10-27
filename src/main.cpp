#include <iostream>
#include <iomanip>
#include <exception>
#include <stdexcept>
#include <unistd.h>
#include "Board.h"
#include "Server.h"
#include "Player.h"

#define SERVER_PASS "aquafina"
#define PORT 2323

using namespace std;

string user_move(Server &myServer, Board &board) {
	while (1){
        myServer.send_msg("\nYour Move:\nPick a position to place piece [1-f][1-f]: ");
        string msg = myServer.read_msg();
        string resp= board.command(msg);
        if(msg=="UNDO"||msg=="REDO"||msg=="DISPLAY"){
            myServer.send_msg(resp);
            myServer.send_msg(board.get_string_board(board));
        }
        else
        {
            if (resp!="ERROR")
                return resp;
            else
                myServer.send_msg(board.error_message);
        }
    }

}

void run() { //send the board to and ask for a command
                        //when i gets a command back it will execute the command on the board and will choose a random other move
                        // and send the board state back to the client and ask for another move
                        //this happens until an end condition
    Board board;
   	Server myServer(PORT);
    string pass;
    string server_pass=SERVER_PASS;
    while (pass != server_pass){
      myServer.send_msg("Please input the password (case insensitive)\n");
      pass=myServer.read_msg();
      std::transform(pass.begin(), pass.end(), pass.begin(), ::tolower);
      sleep(1);
    }
   	while(true){ //win condition
   		myServer.send_msg(board.get_string_board(board));
   		string server_mesg=user_move(myServer, board);
   		myServer.send_msg(server_mesg);
   		//Insert AI MOVE function here
   		board.random_ai();
   	}



}


int main(int argc, char** argv) {
    
		try {
      cout<<"SERVER has started\n"<<"PORT is "<<PORT;
            run();

			return 0;
		}
		catch(runtime_error e) {
			cerr << e.what() << "\n";
		}
}
