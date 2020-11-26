#include "ttt.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>

void init_boards() {
    for(int i = 0; i < HSIZE; i++) {
        htable[i].init = 0;
    }
}

int depth(Board board) {
    int count = 0;
    for(int i = 0; i < 9; i++) {
        if( (board[pos2idx[i]] == 'X') | (board[pos2idx[i]] == 'O') ) {
            count++;
        }
    }
    return count;
}

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

void init_board(Board board) {
    int hash = board_hash(board);
    htable[hash].init = 1;
    htable[hash].winner = winner(board);
    htable[hash].turn = turn(board);
    htable[hash].depth = depth(board);
    strcpy(htable[hash].board, board);
}

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

void recursive(struct BoardNode node) {
    if(node.winner == '?') {
       for(int i = 0; i < 9; i++) {
           if(htable[node.move[i]].init == 1) {
               recursive(htable[node.move[i]]);
           }
       }
    }
    int hash = board_hash(node.board);
    if(htable[hash].winner == '-') {
        htable[hash].score = 0;
    }
    else if(htable[hash].winner == 'O') {
        htable[hash].score = -1;
    }
    else if(htable[hash].winner == 'X') {
        htable[hash].score = 1;
    }
    else{
        if(htable[hash].turn == 'X') {
            int max = -2;
            for(int i = 0; i < 9; i++) {
                if(htable[node.move[i]].init == 1) { // 
                    if(max < htable[node.move[i]].score) {
                        max = htable[node.move[i]].score;
                    }
                }
            }
            htable[hash].score = max;
        }
        else if(htable[hash].turn == 'O') {
            int min = 2;
            for(int i = 0; i < 9; i++) {
                if(htable[node.move[i]].init == 1) {
                    if(min > htable[node.move[i]].score) {
                        min = htable[node.move[i]].score;
                    }
                }
            }
            htable[hash].score = min;
        }
    }
}


void compute_score() {    
    recursive(htable[0]);
}

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
