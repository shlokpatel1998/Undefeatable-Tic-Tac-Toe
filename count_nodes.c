#include "ttt.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
//function counts the max legal nodes in the hash table
int main(int argc, char*argv[]) {

    init_boards();
    init_board(START_BOARD);
    join_graph(START_BOARD);

    int count = 0;
    for(int i = 0; i < HSIZE; i++) {
        if(htable[i].init == 1) {
            count++;
        }
    }
    printf("%d\n", count);
    return 0;
}
