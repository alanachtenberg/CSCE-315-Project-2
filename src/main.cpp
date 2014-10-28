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

// used for parsing the player
string parse_player(string player, string dif, string color){
  string parse;
  parse = "Player::" + player + ",Player::" + dif + "," + color;
  return parse;
}

string user_move(Server &myServer, Board &board) {
	while (1){
        myServer.send_msg("\nYour Move:\nPick a position to place piece [1-f][1-f]: ");
        string msg = myServer.read_msg();
        std::transform(msg.begin(), msg.end(), msg.begin(), ::tolower); // convert msg to lowercase for case insensitive
        string resp = board.command(msg);
        if(msg=="undo"||msg=="redo"||msg=="display"){
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
	Player easy_ai_black(Player::AI,Player::HARD,false);//BLACK PLAYA
	Player easy_ai_white(Player::AI,Player::MEDIUM,true);//WHITE PLAYA
	vector<int> move;
	string temp;
	while(!board.game_won && temp!="q"){ //simulate MEDIUM AI vs MEDIUM AI
	move=easy_ai_black.calc_move(board);
	cout<<board.placePiece(move[0],move[1]);
	cout<<"END BLACK MOVE\n";
    move=easy_ai_white.calc_move(board);
	cout<<board.placePiece(move[0],move[1]);
	cout<<"END WHITE MOVE\n";
	//cin>>temp;
	}
   	Server myServer(PORT);
    string pass;                          // string for password
    string server_pass=SERVER_PASS;       // SERVER_PASS is #define'd 
    while (pass != server_pass){
      myServer.send_msg("Please input the password (case insensitive)\n");
      pass = myServer.read_msg();
      std::transform(pass.begin(), pass.end(), pass.begin(), ::tolower);
      sleep(1);
    }

    string response, parse;
    Player Player1, Player2;
    myServer.send_msg("Please enter number corresponding to desired game mode: \n");
    myServer.send_msg("1. Player vs. Player\n");
    myServer.send_msg("2. Player vs. AI\n");
    myServer.send_msg("3. AI vs. AI\n \n");
    response = myServer.read_msg();
    string temp = board.game_mode(response);

    if(temp == "1"){
      parse = parse_player("HUMAN", "RANDOM", "false")
      Player HUMAN1(parse);//BLACK PLAYA
      parse = parse_player("HUMAN", "RANDOM", "true")
      Player HUMAN2(parse);//WHITE PLAYA
    }

    else if(temp == "2"){
      parse = parse_player("HUMAN", "RANDOM", "false")
      Player HUMAN(parse);//BLACK PLAYA

      myServer.send_msg("Please enter difficulty for the AI (EASY, MEDIUM, or HARD): ");
      response = myServer.read_msg();
      response = board.AI_difficulty(response);
      if(response == "EASY" || response == "MEDIUM" || response == "HARD"){
        myServer.send_msg(response);
        parse = parse_player("AI", response, "true");
        Player AI(parse);
      }
      else 
        myServer.send_msg(board.error_message);
    }

    else if(temp == "3"){
      myServer.send_msg("Please enter difficulty of the first AI (EASY, MEDIUM, or HARD): ");
      response = myServer.read_msg();
      response = board.AI_difficulty(response);

      if(response == "EASY" || response == "MEDIUM" || response == "HARD"){
        myServer.send_msg(response);
        parse = parse_player("AI", response, "false");          // parse the player info
        Player AI_1(parse); //BLACK PLAYA
      }
      else
        myServer.send_msg(board.error_message);

      myServer.send_msg("Please enter difficulty of the Second AI (EASY, MEDIUM, or HARD): ");
      response = myServer.read_msg();
      response = board.AI_difficulty(response);
      if(response == "EASY" || response == "MEDIUM" || response == "HARD"){
        myServer.send_msg(response);
        parse = parse_player("AI", response, "true");           // parse the player info
        Player AI_2(parse); //WHITE PLAYA
      }
      else
        myServer.send_msg(board.error_message);
    }
    else
      myServer.send_msg(board.error_message);
    

   	while(true){ //win condition
   		myServer.send_msg(board.get_string_board(board));
   		string server_mesg=user_move(myServer, board);
   		myServer.send_msg(server_mesg);
   	}



}


int main(int argc, char** argv) {

		try {
      cout<<"SERVER has started\n"<<"PORT is "<<PORT<<endl;
            run();

			return 0;
		}
		catch(runtime_error e) {
			cerr << e.what() << "\n";
		}
}
