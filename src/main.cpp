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
int* getHextoDec(string s){
        int output[2];
        stringstream ss;
        ss<<hex<<s.at(0);
        ss>>output[0];
        ss.ignore(256);
        ss.clear();
        ss<<hex<<s.at(1);
        ss>>output[1];
        return output;
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
void HUMAN_AI(Board& board){

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
        //myServer->send_msg("1,1,BLACK;2,2,WHITE;");
        to_lower(input);
        test_exit(input);//exit if input is exit
        if (input.size()==2){ //size equals 2 must be a move
            string result=board.command(input);//makes move for player
            if (result=="ERROR"){//if the move was invalid do nothing
                cout<<board.error_message;
                myServer->send_msg(board.error_message+"\n");
                }
            else{
            if (board.game_won){
                myServer->send_msg(result+"\n");
                break;//end game
            }
            else{
                vector<int> AI_move=AI.calc_move(board);
                board.placePiece(AI_move[0],AI_move[1]);

                stringstream strstream;
                int* user_move=getHextoDec(input);
                string serverResponse;
                strstream<<user_move[0]<<","<<user_move[1]<<","<<"BLACK;";
                strstream<<AI_move[0]<<","<<AI_move[1]<<","<<"WHITE;";
                serverResponse=strstream.str();
                myServer->send_msg(serverResponse+"\n");//add new line for get line in client
                }
            if (board.game_won){
                myServer->send_msg(board.finish(Cell::WHITE)+"\n");
                break;//end game
            }
            }

            }
            else
                myServer->send_msg("BAD INPUT\n");

        }
        myServer->send_msg("GAME WON\n");
    }

void run() {

    Board board;
    string pass;                          // string for password
    string server_pass=SERVER_PASS;       // SERVER_PASS is #define'd
    while (pass != server_pass){
      myServer->send_msg("PASSWORD\n");
      pass = myServer->read_msg();
      to_lower(pass);
      test_exit(pass);//if pass=="exit" will exit
    }

    string player_setting;
    myServer->send_msg("WELCOME\n");
    while (player_setting!="human-ai" && player_setting.substr(0,5)!="ai-ai"){
        player_setting=myServer->read_msg();
        to_lower(player_setting);
        test_exit(player_setting);
    }
    myServer->send_msg("OK\n");
    if (player_setting=="human-ai")
        HUMAN_AI(board);
    else if (player_setting.substr(0,5)=="ai-ai")
        AI_AI(player_setting.substr(5,player_setting.size()));

  // myServer->send_msg("GAME OVER, SERVER EXITING\n");
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
		catch(...){
            cerr<<"SOMETHING BAD HAPPENED\n";
		}
}
