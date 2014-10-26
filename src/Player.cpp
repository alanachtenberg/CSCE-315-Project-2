#include "Player.h"
    /*PRIVATE MEMBER FUNCTIONS*/
    //returns a random move, EASY AI
    vector<int> Player::calc_random(Board board){

    }

    // returns a move determined using minimax, MEDIUM AI
    vector<int> Player::calc_mininmax(Board board, int depth){

    }

    // returns a move determined using alpha beta pruning, HARD AI
    vector<int> Player::calc_ab_pruning(Board board, int depth);

	/*CONSTRUCTORS*/
	//default constructor, creates an easy AI with color white
	Player::Player(){
        mode=MODE.HUMAN;
        difficulty=DIFFICULTY.EASY;
        color=Cell::STATE::WHITE;
	}

	Player::Player(MODE md, DIFFICULTY df, Cell::State clr){
        mode=md;
        difficulty=df;
        if(clr!=Cell::STATE::BLACK||clr!=Cell::STATE::WHITE)
            cerr<<"ERR! invalid cell state in player constructor\n";
        color=clr;
	}

    /*PUBLIC MEMBER FUNCTIONS*/
    //returns a vector containing the x and y of the AI's Desired move
    vector<int> calc_move(Board board){

    }

    /*OPERATORS*/
	friend std::ostream& operator<<(std::ostream &out, const Player &player){
        //Does nothing for now add print later maybe
        return out;
	}
