#include <iostream>
#include <iomanip>
#include <exception>
#include <stdexcept>
#include <unistd.h>
#include <algorithm>
#include "Board.h"
#include "Server.h"
#include "Player.h"

#define SERVER_PASS "aquafina"
#define PORT 2323

using namespace std;
string& to_lower(string& s){
     std::transform(s.begin(), s.end(), s.begin(), ::tolower);
     return s;
}
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
   	Server myServer(PORT);
    string pass;                          // string for password
    string server_pass=SERVER_PASS;       // SERVER_PASS is #define'd
    while (pass != server_pass){
      myServer.send_msg("PASSWORD\n");
      pass = myServer.read_msg();
      to_lower(pass);
      sleep(1);
    }

    string player_setting;
    myServer.send_msg("WELCOME\n");
    while (player_setting!="human-ai"){
        player_setting=myServer.read_msg();
        to_lower(player_setting);
    }

    myServer.send_msg("OK\n");
    string diff_setting;
    while (diff_setting!="easy"&&diff_setting!="medium"&&diff_setting!="hard"){
        diff_setting=myServer.read_msg();
        to_lower(diff_setting);
    }
    myServer.send_msg("OK\n");

    Player::DIFFICULTY diff;

    if (diff_setting=="easy")
        diff=Player::EASY;
    else if (diff_setting=="medium")
        diff=Player::MEDIUM;
    else if (diff_setting=="hard")
        diff=Player::HARD;
    Player AI(Player::AI,diff,true);//true means Player is white player
    bool is_displaying=false;
    string input;
    while(board.game_won==false){
        input=myServer.read_msg();
        to_lower(input);
        if (input=="exit"){
            myServer.send_msg("EXITING\n");
            exit(0);
            }
        if(input=="display"){
            myServer.send_msg("OK\n");
            is_displaying=!is_displaying;
            if (is_displaying)//if it is now displaying send them the board
                myServer.send_msg(board.get_string_board(board));
        }
        else if (input.size()==2){ //size equals 2 must be a move
            string result=board.command(input);//makes move for player
            if (result=="ERROR")//if the move was invalid do nothing
                cout<<board.error_message;
            else{
            if (board.game_won){
                myServer.send_msg(result);
                break;//end game
            }
            else{
                myServer.send_msg("OK\n");
                vector<int> AI_move=AI.calc_move(board);
                board.placePiece(AI_move[0],AI_move[1]);
                if (is_displaying)
                    myServer.send_msg(board.get_string_board(board));
                else{
                string move_msg;
                stringstream ss;
                ss<<AI_move[0]<<AI_move[1];
                ss>>move_msg;
                    myServer.send_msg(move_msg);
                }
            }

            }
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
    //  parse = parse_player("HUMAN", "RANDOM", "false")
      Player HUMAN1(Player::HUMAN, Player::RANDOM, false);//BLACK PLAYA
  //    parse = parse_player("HUMAN", "RANDOM", "true")
      Player HUMAN2(Player::HUMAN, Player::RANDOM, true);//WHITE PLAYA
    }

    else if(temp == "2"){
      parse = parse_player("HUMAN", "RANDOM", "false");
      Player HUMAN(Player::HUMAN, Player::RANDOM, false);//BLACK PLAYA

      myServer.send_msg("Please enter difficulty for the AI (EASY, MEDIUM, or HARD): ");
      response = myServer.read_msg();
      response = board.AI_difficulty(response);
      if(response == "EASY"){
        myServer.send_msg(response);
  //      parse = parse_player("AI", response, "true");
        Player AI(Player::AI, Player::EASY, true); 
      }
      if(response == "MEDIUM"){
        myServer.send_msg(response);
  //      parse = parse_player("AI", response, "true");
        Player AI(Player::AI, Player::MEDIUM, true); 
      }
      if(response == "HARD"){
        myServer.send_msg(response);
  //      parse = parse_player("AI", response, "true");
        Player AI(Player::AI, Player::HARD, true); 
      }
      else 
        myServer.send_msg(board.error_message);
    }

    else if(temp == "3"){
      myServer.send_msg("Please enter difficulty of the first AI (EASY, MEDIUM, or HARD): ");
      response = myServer.read_msg();
      response = board.AI_difficulty(response);

      if(response == "EASY"){
        myServer.send_msg(response);
  //      parse = parse_player("AI", response, "true");
        Player AI_1(Player::AI, Player::EASY, false); 
      }
      if(response == "MEDIUM"){
        myServer.send_msg(response);
  //      parse = parse_player("AI", response, "true");
        Player AI_1(Player::AI, Player::MEDIUM, false); 
      }
      if(response == "HARD"){
        myServer.send_msg(response);
  //      parse = parse_player("AI", response, "true");
        Player AI_1(Player::AI, Player::HARD, false); 
      }
      else
        myServer.send_msg(board.error_message);

      myServer.send_msg("Please enter difficulty of the Second AI (EASY, MEDIUM, or HARD): ");
      response = myServer.read_msg();
      response = board.AI_difficulty(response);
      if(response == "EASY"){
        myServer.send_msg(response);
  //      parse = parse_player("AI", response, "true");
        Player AI_2(Player::AI, Player::EASY, true); 
      }
      if(response == "MEDIUM"){
        myServer.send_msg(response);
  //      parse = parse_player("AI", response, "true");
        Player AI_2(Player::AI, Player::MEDIUM, true); 
      }
      if(response == "HARD"){
        myServer.send_msg(response);
  //      parse = parse_player("AI", response, "true");
        Player AI_2(Player::AI, Player::HARD, true); 
      }
      else
        myServer.send_msg(board.error_message);

    }
    myServer.send_msg("GAME OVER, SERVER EXITING\n");
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
