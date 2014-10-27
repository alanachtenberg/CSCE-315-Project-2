#include "Player.h"
    /*PRIVATE MEMBER FUNCTIONS*/
    //returns a random move, EASY AI
    vector<int> Player::calc_random(Board board){

        return vector<int>();
    }

    // returns a move determined using minimax, MEDIUM AI
    vector<int> Player::calc_mininmax(Board board, int depth){
        return vector<int>();
    }

    // returns a move determined using alpha beta pruning, HARD AI
    vector<int> Player::calc_ab_pruning(Board board, int depth){
        return vector<int>();
    }

	/*CONSTRUCTORS*/
	//default constructor, creates an easy AI with color white
	Player::Player(){
        mode=Player::AI;
        difficulty=Player::EASY;
        color=Cell::WHITE;
	}

	Player::Player(MODE md, DIFFICULTY df, Cell::STATE clr){
        mode=md;
        difficulty=df;
        if(clr!=Cell::BLACK||clr!=Cell::WHITE)
            cerr<<"ERR! invalid cell state in player constructor\n";
        color=clr;
	}

    /*PUBLIC MEMBER FUNCTIONS*/
    //returns a vector containing the x and y of the AI's Desired move,
    //by calling the correspondinf calc function of the correct difficulty
    vector<int> Player::calc_move(Board& board){

        switch(difficulty) { //determin difficulty
        case Player::EASY:
            return calc_random(board);//random move

        case Player::MEDIUM:
            return calc_mininmax(board, MINMAX_DEPTH);//minimax move

        case Player::HARD:
            return calc_ab_pruning(board, ALPHABETA_DEPTH);// alpha beta pruning move

        default:
            cerr<<"ERR, unknown DIFICULTY for AI\n";
            return vector<int>();
	}

    }

    /*OPERATORS*/
    std::ostream& operator<<(std::ostream &out, const Player &player){
        //Does nothing for now add print later maybe
        return out;
	}
