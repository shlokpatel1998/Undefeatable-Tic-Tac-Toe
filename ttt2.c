#include "ttt.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
//initializes all nodes in hash table with init value of 0
void init_boards() {
    for(int i = 0; i < HSIZE; i++) {
        htable[i].init = 0;
    }
}
//calculates the number of moves made on the board
int depth(Board board) {
    int count = 0;
    for(int i = 0; i < 9; i++) {
        if( (board[pos2idx[i]] == 'X') | (board[pos2idx[i]] == 'O') ) {
            count++;
        }
    }
    return count;
}
//determines the winner based on the baord inputed 
char winner(Board board) {
    int xCountTotal = 0;
    int oCountTotal = 0;

    for(int i = 0; i < 8; i++) {
        int xCount = 0;
        int oCount = 0;
        for(int j = 0; j < 3; j++) {
            int index = WINS[i][j];
            if(board[pos2idx[index]] == 'X') {
                xCount++;
                xCountTotal++;
            }
            else if(board[pos2idx[index]] == 'O') {
                oCount++;
                oCountTotal++;
            }
        }
        if(xCount == 3) {
            return 'X';
        }
        if(oCount == 3) {
            return 'O';
        }
    }
    if(xCountTotal + oCountTotal == 24) { //8 combos, if all 8 combos tried and full, total should be 24 (8*(3combos))
        return '-';
    }
    else {
        return '?';
    }
}
//determines if it is O's or X's turn or if the game is over 
char turn(Board board) {
    int moves = depth(board);
    if(moves != 9 && winner(board) == '?') {
        if(moves % 2 == 0) {
            return 'X';
        }
        else {
            return 'O';
        }
    }
    else {
        return '-';
    }
}
//initializes the board with values for turn, init, winner, depth, turn and its board string
void init_board(Board board) {
    int hash = board_hash(board);
    htable[hash].init = 1;
    htable[hash].winner = winner(board);
    htable[hash].turn = turn(board);
    htable[hash].depth = depth(board);
    strcpy(htable[hash].board, board);
}
//joins the graph by calling the function recursively if the board hasn't been already initialized and is a valid move (game not over)
void join_graph(Board board) {
    int hash = board_hash(board);
    for(int i = 0; i < 9; i++) {
        char piece = board[pos2idx[i]];
        if( (piece == 'X') | (piece == 'O') | (winner(board) != '?') ) {
            htable[hash].move[i] = -1;
        }
        else {
            Board boardcopy;
            strcpy(boardcopy, board);
            boardcopy[pos2idx[i]] = turn(board);
            int newhash = board_hash(boardcopy);
            htable[hash].move[i] = newhash;
            if(htable[newhash].init == 0) {
                init_board(boardcopy);
                join_graph(boardcopy);
            }
        }
    }
}
//calls this function which recursively goes to the end of the graph and works its way "out" of recursion to assign a score for every VALID node

//assigns a score for each node based on its children nodes 
//goes through all the nodes in each depth starting from deepest to least 
void compute_score() {    
    htable[-1].init = 0;
    for(int j = 9; j >= 0; j--) {
        for(int i = 0; i < HSIZE; i++) {
            if( (htable[i].init == 1) && (htable[i].depth == j) ) {   
                struct BoardNode node = htable[i];
                int hash = board_hash(node.board);
                if(node.winner == '-') {
                    htable[hash].score = 0;
                }
                else if(node.winner == 'O') {
                    htable[hash].score = -1;
                }
                else if(node.winner == 'X') {
                    htable[hash].score = 1;
                }
                else {
                    if(node.turn == 'X') {
                        int max = -2;
                        for(int i = 0; i < 9; i++) {
                            if( (htable[node.move[i]].init) == 1 && (node.move[i] != -1) )  { 
                                struct BoardNode nodechild = htable[node.move[i]]; 
                                if(max < nodechild.score) {
                                    max = nodechild.score;
                                }
                            }
                        }
                        htable[hash].score = max;
                    }
                    else if(node.turn == 'O') {
                        int min = 2;
                        for(int i = 0; i < 9; i++) {
                            if( (htable[node.move[i]].init == 1) && (node.move[i] != -1) ) {
                                struct BoardNode nodechild = htable[node.move[i]]; 
                                if(min > nodechild.score) {
                                    min = nodechild.score;
                                }
                            }
                        }
                        htable[hash].score = min;
                    }
                }
            }
        }
    }
}

//calculates the best move based on the score of nodes in the next move (looks 1 board ahead)
int best_move(int board) {
    struct BoardNode node = htable[board];
    int bestmoveX = -2;
    int bestmoveIndexX;
    int bestmoveO = 2;
    int bestmoveIndexO;
    if(node.turn == 'X') {    
        for(int i = 0; i < 9; i++) {
            if(htable[node.move[i]].init == 1) {
                if(bestmoveX < htable[node.move[i]].score) {
                    bestmoveX = htable[node.move[i]].score;
                    bestmoveIndexX = i;
                } 
            }
        }
        return bestmoveIndexX;
    }
    else if(node.turn == 'O') {
        for(int i = 0; i < 9; i++) {
            if(htable[node.move[i]].init == 1) {
                if(bestmoveO > htable[node.move[i]].score) {
                    bestmoveO = htable[node.move[i]].score;
                    bestmoveIndexO = i; 
                } 
            }
        }
        return bestmoveIndexO;
    }
    return 0;
}
