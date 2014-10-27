#include "Player.h"
#include <limits.h>

    /*PRIVATE MEMBER FUNCTIONS*/
    //returns a random move, EASY AI
    vector<int> Player::calc_random(Board board){
        vector<int> move=vector<int>(2);
        if (board.is_board_empty()){
            move[0]=rand()%16; //get random x between 0 and 15
            move[1]=rand()%16; // get random y o and 15
            return move;
        }
        Cell cell = board.getGameHistory().top();

        bool moved = false;								// used for while loop determining where to move

        int dirs[8];
        dirs[0] = board.checkPath(cell, Cell::N);
        dirs[1] = board.checkPath(cell, Cell::NE);
        dirs[2] = board.checkPath(cell, Cell::E);
        dirs[3] = board.checkPath(cell, Cell::SE);
        dirs[4] = board.checkPath(cell, Cell::S);
        dirs[5] = board.checkPath(cell, Cell::SW);
        dirs[6] = board.checkPath(cell, Cell::W);
        dirs[7] = board.checkPath(cell, Cell::NW);

        int N_to_S = dirs[0] + dirs[4]-1;// minus 1 because middle vaule is counted twice
        int NE_to_SW = dirs[1] + dirs[5]-1;
        int E_to_W = dirs[2] + dirs[6]-1;
        int SE_to_NW = dirs[3] + dirs[7]-1;

        if(N_to_S >= 3){
            // if N can be covered cover
            if(board.getCell(cell.getX(), cell.getY() - dirs[0]).getState() == Cell::EMPTY){
                //placePiece(cell.getX(), cell.getY() - dirs[0]);
                move[0]=cell.getX();//x
                move[1]=cell.getY()- dirs[0];//y
                moved = true;
            }
            // if S can be covered cover
            else if(board.getCell(cell.getX(), cell.getY() + dirs[4]).getState() == Cell::EMPTY){
                //placePiece(cell.getX(), cell.getY() + dirs[4]);
                move[0]=cell.getX();
                move[1]=cell.getY() + dirs[4];
                moved = true;
            }
        }
        else if(NE_to_SW >= 3){
            // if NE can be covered cover
            if(board.getCell(cell.getX() + dirs[1], cell.getY() - dirs[1]).getState() == Cell::EMPTY){
                //placePiece(cell.getX() + dirs[1], cell.getY() - dirs[1]);
                move[0]=cell.getX()+dirs[1];//x
                move[1]=cell.getY()-dirs[1];//y
                moved = true;
            }
            // if SW can be covered cover
            else if(board.getCell(cell.getX() - dirs[5], cell.getY() + dirs[5]).getState() == Cell::EMPTY){
                //placePiece(cell.getX() - dirs[5], cell.getY() + dirs[5]);
                move[0]=cell.getX()-dirs[5];//x
                move[1]=cell.getY()+dirs[5];//y
                moved = true;
            }
        }
        else if(E_to_W >= 3){
            // if E can be covered cover
            if(board.getCell(cell.getX() + dirs[2], cell.getY()).getState() == Cell::EMPTY){
                //placePiece(cell.getX() + dirs[2], cell.getY());
                move[0]=cell.getX()+dirs[2];//x
                move[1]=cell.getY();//y
                moved = true;
            }
            // if W can be covered cover
            else if(board.getCell(cell.getX() - dirs[6], cell.getY()).getState() == Cell::EMPTY){
                //placePiece(cell.getX() - dirs[6], cell.getY());
                move[0]=cell.getX()-dirs[6];//x
                move[1]=cell.getY();//y
                moved = true;
            }
        }
        else if(SE_to_NW >= 3){
            // if SE can be covered cover
            if(board.getCell(cell.getX() + dirs[3], cell.getY() + dirs[3]).getState() == Cell::EMPTY){
                //placePiece(cell.getX() + dirs[3], cell.getY() + dirs[3]);
                move[0]=cell.getX()+dirs[3];//x
                move[1]=cell.getY()+dirs[3];//y
                moved = true;
            }
            // if NW can be covered cover
            else if(board.getCell(cell.getX() - dirs[7], cell.getY() - dirs[6]).getState() == Cell::EMPTY){
                //placePiece(cell.getX() - dirs[7], cell.getY() - dirs[6]);
                move[0]=cell.getX()-dirs[7];//x
                move[1]=cell.getY()-dirs[6];//y
                moved = true;
            }
        }

        if(!moved){				// If no obvious moves or cant cover  make random move
            //random_X = 1 + rand() % 15;		// create a random number from 1 - 15 for X
            //random_Y = 1 + rand() % 15;		// create a random number from 1 - 15 for Y
            vector<Cell> possible_moves=board.get_moves(board);
            if (possible_moves.size()<1){
                string err="ERR NO POSSSIBLE MOVES\n";
                cout<<err;
                throw err;
            }
            int random_move= rand() % possible_moves.size();//gets random index to pick from possible moves
            move[0]=possible_moves[random_move].getX();
            move[1]=possible_moves[random_move].getY();
        }
        return move;
    }

    // returns a move determined using minimax, MEDIUM AI
    vector<int> Player::calc_mininmax(Board board){
        vector<int>move(2); //handle corner case of board being empty
        if (board.is_board_empty()){
            move[0]=rand()%16; //get random x between 0 and 15
            move[1]=rand()%16; // get random y between 0 and 15
            return move;
        }
        vector<Cell> avail_moves=board.get_moves(board);
        int max=0;
        int new_max=0;
        int index=0;
       // Timer test;
        for (unsigned int i=0;i<avail_moves.size();++i){
         //   test.start();
            Board new_board=board;
            //NOTE placeValidatedPiece is an optimized place piece for when the piece has already been validated
            new_board.placeValidatedPiece(avail_moves[i].getX(),avail_moves[i].getY());//update board to new node
            new_max=minimax(board,MINMAX_DEPTH-1,true);
            if(new_max>max){
                max=new_max;
                index=i;
            }
           // test.finish();
        //    cout<<"TIMER SEC VAL "<<test.sec()<<" "<<"USEC VAL "<<test.usec()<<endl;
        }

        Cell best_move=avail_moves[index];//return move in vector form
        move[0]=best_move.getX();
        move[1]=best_move.getY();
        return move;
    }
    // returns a move determined using alpha beta pruning, HARD AI
    vector<int> Player::calc_ab_pruning(Board board){
        vector<int>move(2);
        if (board.is_board_empty()){
            move[0]=6+rand()%4;// hard AI makes the inteligent decision to make
            move[1]=6+rand()%4;// the first move near the center of the board
            return move;
        }
        return move;
    }

    int Player::minimax(Board board, int depth, bool max_player_turn){
        vector<Cell> avail_moves=board.get_moves(board);
        int best;
        int value=0;
        if (depth==0||avail_moves.size()==0)
            return 10;//RETURN THE EVAL FUNCTION
        if (max_player_turn){//if the turn is our AI's
            best=INT_MIN;//lowest possible int
            for (unsigned int i=0;i<avail_moves.size();++i){
                Board new_board=board;
                //NOTE placeValidated piece is an optimized function for placing a previously validated piece
                new_board.placeValidatedPiece(avail_moves[i].getX(),avail_moves[i].getY());// get child board
                value=minimax(board,depth-1,false);//magical recursion
                best=max(value,best); // during the AI turn we will try to find the move that helps the ai the most
            }
            return best;
        }
        else{
            best=INT_MAX;//highest possible int
            for (unsigned int i=0;i<avail_moves.size();++i){
                Board new_board=board;
                new_board.placeValidatedPiece(avail_moves[i].getX(),avail_moves[i].getY());// get child board
                value=minimax(board,depth-1,true);
                best=min(value,best); // during the oponent's turn we will find the move that hurts the most
            }
            return best;
        }
    }

    int Player::ab_pruning(Board board, int depth, int alpha, int beta, bool max_player_turn){
        vector<Cell> avail_moves=board.get_moves(board);
        if (depth==0||avail_moves.size()==0)
            return 10;//RETURN THE EVAL FUNCTION
        if (max_player_turn){//if the turn is our AI's
            for (unsigned int i=0;i<avail_moves.size();++i){
                Board new_board=board;
                new_board.placeValidatedPiece(avail_moves[i].getX(),avail_moves[i].getY());
                alpha = max(alpha, ab_pruning(new_board, depth - 1, alpha, beta, false));
                if (beta <= alpha)
                    break;
            }
            return alpha;
        }
        else {
            for (unsigned int i=0;i<avail_moves.size();++i){
                Board new_board=board;
                new_board.placeValidatedPiece(avail_moves[i].getX(),avail_moves[i].getY());
                beta = min(beta, ab_pruning(new_board, depth - 1, alpha, beta, true));
                if (beta <= alpha)
                    break;
            }
            return beta;
        }


        return 0;
        
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
        if(clr!=Cell::BLACK && clr!=Cell::WHITE)
            cerr<<"ERR! invalid cell state in player constructor\n";
        color=clr;
	}

    /*PUBLIC MEMBER FUNCTIONS*/
    //returns a vector containing the x and y of the AI's Desired move,
    //by calling the correspondinf calc function of the correct difficulty
    vector<int> Player::calc_move(Board& board){

        switch(difficulty) { //determine difficulty
        case Player::EASY:
            return calc_random(board);//random move

        case Player::MEDIUM:
            return calc_mininmax(board);//minimax move

        case Player::HARD:
            return calc_ab_pruning(board);// alpha beta pruning move

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
