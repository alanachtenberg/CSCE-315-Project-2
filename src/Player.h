#ifndef PLAYER_H
#define PLAYER_H

#define MINMAX_DEPTH 5
#define ALPHABETA_DEPTH 8
#include "Board.h"
#include <vector>
using namespace std;


class Player {
public:
    /*DATA STRUCTURES*/
    enum MODE { HUMAN, AI };
	enum DIFFICULTY { EASY, MEDIUM, HARD, RANDOM };/*PUBLIC DATA MEMBERS*/
private:
    /*PRIVATE DATA MEMBERS*/
    Cell::STATE color;//color of the player
    MODE mode;
    DIFFICULTY difficulty;
    /*PRIVATE MEMBER FUNCTIONS*/
    vector<int> calc_random(Board board); //returns a random move, EASY AI
    vector<int> calc_mininmax(Board board);// returns a move determined using minimax, MEDIUM AI
    vector<int> calc_ab_pruning(Board board);// returns a move determined using alpha beta pruning, HARD AI

    int minimax(Board board, int depth, Cell::STATE max_player);
    int ab_pruning(Board board, int depth, Cell::STATE max_player);
public:


	/*CONSTRUCTORS*/
	Player();
	Player(MODE md, DIFFICULTY df, Cell::STATE clr);

    /*PUBLIC MEMBER FUNCTIONS*/
    //returns a vector containing the x and y of the AI's Desired move
    vector<int> calc_move(Board& board);

    /*OPERATORS*/
	friend std::ostream& operator<<(std::ostream &out, const Player &player);
};

#endif
