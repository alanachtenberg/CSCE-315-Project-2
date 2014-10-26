#ifndef PLAYER_H
#define PLAYER_H

class Player {

private:
    /*PRIVATE DATA MEMBERS*/
    Cell::STATE color;//color of the player
    MODE mode;
    DIFFICULTY difficulty;
    /*PRIVATE MEMBER FUNCTIONS*/
    vector<int> calc_random(Board board); //returns a random move, EASY AI
    vector<int> calc_mininmax(Board board, int depth);// returns a move determined using minimax, MEDIUM AI
    vector<int> calc_ab_pruning(Board board, int depth);// returns a move determined using alpha beta pruning, HARD AI
public:
    /*PUBLIC DATA MEMBERS*/
    enum MODE { HUMAN, AI };
	enum DIFFICULTY { EASY, MEDIUM, HARD, RANDOM };

	/*CONSTRUCTORS*/
	Player();
	Player(MODE md, DIFFICULTY df, Cell::State clr);

    /*PUBLIC MEMBER FUNCTIONS*/
    //returns a vector containing the x and y of the AI's Desired move
    vector<int> calc_move(Board board);

    /*OPERATORS*/
	friend std::ostream& operator<<(std::ostream &out, const Player &player);
};

#endif
