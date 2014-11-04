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

Board board;
int port_num;
Server* myServer;
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

void test_exit(string& s){
    if (s=="exit"){
            myServer->send_msg("EXITING\n");
            exit(0);
        }
}

void AI_AI(string input){//string contains server and port
    stringstream ss;
    ss<<input;
    string opp_server;
    string opp_port;
    ss>>opp_server;
    ss>>opp_port;
}
void HUMAN_AI(){
    myServer->send_msg("OK\n");
    string diff_setting;
    while (diff_setting!="easy"&&diff_setting!="medium"&&diff_setting!="hard"){
        diff_setting=myServer->read_msg();
        to_lower(diff_setting);
        test_exit(diff_setting);
    }
    myServer->send_msg("OK\n");

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
        input=myServer->read_msg();
        to_lower(input);
        test_exit(input);//exit if input is exit
        if(input=="display"){
            myServer->send_msg("OK\n");
            is_displaying=!is_displaying;
            if (is_displaying)//if it is now displaying send them the board
                myServer->send_msg(board.get_string_board(board));
        }
        else if (input.size()==2){ //size equals 2 must be a move
            string result=board.command(input);//makes move for player
            if (result=="ERROR")//if the move was invalid do nothing
                cout<<board.error_message;
            else{
            if (board.game_won){
                myServer->send_msg(result);
                break;//end game
            }
            else{
                //myServer->send_msg("OK\n");
                vector<int> AI_move=AI.calc_move(board);
                board.placePiece(AI_move[0],AI_move[1]);

                string move_msg;
                stringstream ss;
                ss<<input.at(0)<<","<<input.at(1)<<","<<"BLACK;";
                ss<<hex<<AI_move[0]<<","<<hex<<AI_move[1]<<","<<"WHITE;";
                ss>>move_msg;
                    myServer->send_msg(move_msg);
                }
            }

            }
        }
    }

void run() {
    string pass;                          // string for password
    string server_pass=SERVER_PASS;       // SERVER_PASS is #define'd
    while (pass != server_pass){
      myServer->send_msg("PASSWORD\n");
      pass = myServer->read_msg();
      to_lower(pass);
      test_exit(pass);//if pass=="exit" will exit
      sleep(1);
    }

    string player_setting;
    myServer->send_msg("WELCOME\n");
    while (player_setting!="human-ai" && player_setting.substr(0,5)!="ai-ai"){
        player_setting=myServer->read_msg();
        to_lower(player_setting);
        test_exit(player_setting);
    }
    if (player_setting=="human-ai")
        HUMAN_AI();
    else if (player_setting.substr(0,5)=="ai-ai")
        AI_AI(player_setting.substr(5,player_setting.size()));

   myServer->send_msg("GAME OVER, SERVER EXITING\n");
}


int main(int argc, char** argv) {

		try {
            cout<<"input port number of server\n";
            cin>>port_num;
            cout<<"SERVER has started\n"<<"PORT is "<<port_num<<endl;
            myServer= new Server(port_num);
            run();
			return 0;
		}
		catch(runtime_error e) {
			cerr << e.what() << "\n";
		}
}
