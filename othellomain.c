/** othellomain.c
 * =========================================================== 
 * Name: C3C John Stanley
 * Section: M6/7
 * Project: Othello
 * Purpose: Demonstrate mastery of CS210 concepts in Final
 *          Project
 * Documentation: none
 * =========================================================== */

// IN ORDER TO OPERATE, STRETCH TERMINAL UP TO THE TOP OF THE SCREEN AND PRESS RUN

#include "othellofuncts.h"

int main (void) {

    char** board = (char**) malloc(boardSize * sizeof(char*));
    for (int c = 0; c < boardSize; ++c){
        board[c] = (char*) malloc(boardSize * sizeof(char));
    }

    resetBoard(board);

    //printBoard(board);

    do {
        if(printLegalMovesBoard(board, 'O')){
            printf("\nIt is Black's turn, make a move!\n");
            printf("'▫' indicates a legal move!\n");
            printf("%d discs flipped\n\n", takeMove(board, 'O', getMove()));
            printf("------------------------------\n\n");
        }
        
        if(numLegalMoves(board, 'X')){
            printf("White will make a move now...\n\n");
            printBoard(board);
            printf("\nWhite has moved!\n\n");
            int flippedDiscs = takeMove(board, 'X', randomMove(board, 'X'));
            printBoard(board);
            //printf("'▫' indicates a legal move!\n");
            printf("\n%d discs flipped\n\n", flippedDiscs);
            printf("------------------------------\n\n");
        }
    } while (numLegalMoves(board, 'O') || numLegalMoves(board, 'X'));

    printBoard(board);

    int blackDiscs = 0;
    int whiteDiscs = 0;
    getNumDiscs(board, &blackDiscs, &whiteDiscs);

    printf("\nBlack: %d, White: %d\n\n", blackDiscs, whiteDiscs);

    for (int c = 0; c < boardSize; ++c){
        free(board[c]);
    }
    free(board);

    return 0;
}