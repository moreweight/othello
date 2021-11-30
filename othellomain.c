
#include "othellofuncts.h"

int main (void) {

    char** board = (char**) malloc(boardSize * sizeof(char*));
    for (int c = 0; c < boardSize; ++c){
        board[c] = (char*) malloc(boardSize * sizeof(char));
    }

    resetBoard(board);

    //printBoard(board);

    do {
        if(printLegalMovesBoard(board, 'X')){
            printf("It is White's turn, make a move!\n");
            printf("'▫' indicates a legal move!\n");
            printf("%d discs flipped\n", takeMove(board, 'X', getMove()));
        }
        
        if(printLegalMovesBoard(board, 'O')){
            printf("It is Black's turn, make a move!\n");
            printf("'▫' indicates a legal move!\n");
            printf("%d discs flipped\n", takeMove(board, 'O', randomMove(board, 'O')));
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