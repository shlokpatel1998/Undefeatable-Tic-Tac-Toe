#include "ttt.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char*argv[]) {

    init_boards();
    init_board(START_BOARD);
    join_graph(START_BOARD);
    compute_score();
    int i = 1;
    while(i < argc) {
        int hash;
        sscanf(argv[i], "%d", &hash);
        print_node(htable[hash]);
        printf("best move: %d\n", best_move(hash));
        i++;
    }
    return 0;
}

