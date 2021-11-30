
#include <ctype.h>
#include <string.h>

#include "othellofuncts.h"

void printBoard(char** board){

    printf("   ");
    for(int r = 0; r < boardSize; ++r){
        printf("%c ", r + 'A');
    }
    printf("\n");

    for(int r = 0; r < boardSize; ++r){
        printf("%2.d ", boardSize - r);
        for(int c = 0; c < boardSize; ++c){
            if (board[r][c] == 'X'){
                printf("● ");
            } else if (board[r][c] == 'O'){
                printf("○ ");
            } else {
                printf("  ");
            }
            //● ○
        }
        printf("%d\n", boardSize - r);
    }

    printf("   ");
    for(int r = 0; r < boardSize; ++r){
        printf("%c ", r + 'A');
    }
    printf("\n");
    
}

void resetBoard(char** board){
    for(int r = 0; r < boardSize; ++r){
        for(int c = 0; c < boardSize; ++c){
            board[r][c] = ' ';
        }
    }
    board[boardSize / 2 - 1][boardSize / 2 - 1] = 'O';
    board[boardSize / 2 - 1][boardSize / 2] = 'X';
    board[boardSize / 2][boardSize / 2 - 1] = 'X';
    board[boardSize / 2][boardSize / 2] = 'O';
}

void getNumDiscs(char** board, int* black, int* white){
    for(int r = 0; r < boardSize; ++r){
        for(int c = 0; c < boardSize; ++c){
            if (board[r][c] == 'O'){
                ++*black;
            } else if (board[r][c] == 'X'){
                ++*white;
            }
        }
    }
}

char legalPlayerChar(char player, char* funcName){
    if(player == 'O'){
        return 'X';
    } else if(player == 'X'){
        return 'O';
    } else {
        fprintf(stderr, "Bad player input to function %s: %c\n", funcName, player);
        exit(1);
    }
}

gameSquare getMove(){

    char* col = (char*) malloc(sizeof(char));
    int* row = (int*) malloc(sizeof(int));

    printf("Choose a square to place your piece: ");
    fscanf(stdin, "\n%c\n%d", col, row);
    fflush(stdin);
    //printf("\n%d %d\n\n", *col, *row);
    gameSquare square;
    square.c = toupper(*col) - 'A';
    square.r = boardSize - *row;

    free(col); free(row);
    return square;
}

gameSquare randomMove(char** board, char player){
    gameSquare square;
    square.r = rand() % boardSize;
    square.c = rand() % boardSize;
    if (!legalMove(board, player, square)){
        return randomMove(board, player);
    }
    return square;
}

char nearbyChar(char** board, gameSquare square, int rowOffset, int colOffset){

    // Checks if checked character is off board, if so, returns null character
    if (square.r + rowOffset >= boardSize ||
        square.r + rowOffset <  0         ||
        square.c + colOffset >= boardSize ||
        square.c + colOffset <  0) {

                //fprintf(stderr, "Attempted to access identity of square outside of game board!\n");
                //exit(1);
        return 0;
    }

    return board[square.r + rowOffset][square.c + colOffset];
}

int distFlanking(char** board, gameSquare square, const char* direction){
    if(strncmp(direction, "up", 2) == 0){
        for (int row = 1; square.r - row >= 0; ++row){
            if(nearbyChar(board, square, -row, 0) == ' '){
                return 0;
            }
            if(nearbyChar(board, square, -row, 0) == board[square.r][square.c]){
                return row;
            }
        }
    }
    if(strncmp(direction, "down", 4) == 0){
        for (int row = 1; square.r + row < boardSize; ++row){
            if(nearbyChar(board, square, row, 0) == ' '){
                return 0;
            }
            if(nearbyChar(board, square, row, 0) == board[square.r][square.c]){
                return row;
            }
        }
    }
    if(strncmp(direction, "left", 4) == 0){
        for (int col = 1; square.c - col >= 0; ++col){
            if(nearbyChar(board, square, 0, -col) == ' '){
                return 0;
            }
            if(nearbyChar(board, square, 0, -col) == board[square.r][square.c]){
                return col;
            }
        }
    }
    if(strncmp(direction, "right", 5) == 0){
        for (int col = 1; square.c + col < boardSize; ++col){
            if(nearbyChar(board, square, 0, col) == ' '){
                return 0;
            }
            if(nearbyChar(board, square, 0, col) == board[square.r][square.c]){
                return col;
            }
        }
    }
    return 0;
}

int flipFlanked(char** board, gameSquare square){
    if (distFlanking(board, square, "up") > 1){
        board[square.r - (distFlanking(board, square,    "up") - 1)][square.c] = board[square.r][square.c];
    } else if (distFlanking(board, square, "down") > 1){
        board[square.r + (distFlanking(board, square,  "down") - 1)][square.c] = board[square.r][square.c];
    } else if (distFlanking(board, square, "left") > 1){
        board[square.r][square.c - (distFlanking(board, square,  "left") - 1)] = board[square.r][square.c];
    } else if (distFlanking(board, square, "right") > 1){
        board[square.r][square.c + (distFlanking(board, square, "right") - 1)] = board[square.r][square.c];
    } else {
        return 0;
    }
    return 1 + flipFlanked(board, square);
}

bool legalMove(char** board, char player, gameSquare square){
    
    // It not legal move if not empty space
    if(board[square.r][square.c] != ' '){
        return false;
    }

    char opponent = legalPlayerChar(player, "legalMove()");

    // It is necessary to change the inspected square to the current player's color 
    // to ensure distFlanking works propertly in finding discs of the same color
    board[square.r][square.c] = player;

    // Check each direction to ensure that an opponent's disc exists and that a 
    // player's disc exists to flank it
    if (    (nearbyChar(board, square, -1, 0) == opponent && distFlanking(board, square,    "up")) ||
            (nearbyChar(board, square,  1, 0) == opponent && distFlanking(board, square,  "down")) ||
            (nearbyChar(board, square, 0, -1) == opponent && distFlanking(board, square,  "left")) ||
            (nearbyChar(board, square,  0, 1) == opponent && distFlanking(board, square, "right"))) {
        // Restore tested space to a blank space
        board[square.r][square.c] = ' ';
        return true;
    }
    // Restore tested space to a blank space
    board[square.r][square.c] = ' ';
    return false;
    
/*
    // Checks if square is on top row, if not, checks square above to
    // to see if it is opponent's square. Then checks up the col to
    // find if player's square exists
    if (nearbyChar(board, square, -1, 0) == opponent){
        for (int row = square.r - 1; row >= 0; --row){
            if (board[row][square.c] == player){
                return true;
            }
        }
    }
    // Checks if square is on bot row, if not, checks square below to
    // to see if it is opponent's square. Then checks down the col to
    // find if player's square exists
    if (nearbyChar(board, square, 1, 0) == opponent){
        for (int row = square.r + 1; row <= 7; ++row){
            if (board[row][square.c] == player){
                return true;
            }
        }
    }

    // Checks if square is on first col, if not, checks square left to
    // to see if it is opponent's square. Then checks left on row to
    // find if player's square exists
    if (nearbyChar(board, square, 0, -1) == opponent){
        for (int col = square.c - 1; col >= 0; --col){
            if (board[square.r][col] == player){
                return true;
            }
        }
    }   

    // Checks if square is on final col, if not, checks square right to
    // to see if it is opponent's square. Then checks right on row to
    // find if player's square exists
    if (nearbyChar(board, square, 0, 1) == opponent){
        for (int col = square.c + 1; col <= 7; ++col){
            if (board[square.r][col] == player){
                return true;
            }
        }
    }

    // If no other conditions true, move is illegal
    return false;
*/
}

int takeMove(char** board, char player, gameSquare square){

    // Invokes recursion until selected move is legal
    if (!legalMove(board, player, square)){
        printf("That was an illegal move, try again!\n");
        return takeMove(board, player, getMove());
    }

    // Place player's disc
    board[square.r][square.c] = player;

    // Flip all opponent's discs
    // TODO may flip empty spaces to 
    return flipFlanked(board, square);

/*
    // Flip all flanked disc under the placed piece
    for (int r = 1; r < distFlanking(board, square, "down"); ++r){
        if(nearbyChar(board, square, r, 0) == opponent){
            board[square.r + r][square.c] = player;
            ++numFlipped;
        }
    }

    printf("distFlank: up: %d\n", distFlanking(board, square, "up"));
    // Flip all flanked disc above the placed piece
    for (int r = 1; r < distFlanking(board, square, "up"); ++r){
        printf("%c m\n", nearbyChar(board, square, -r, 0));
        if(nearbyChar(board, square, -r, 0) == opponent){
            board[square.r - r][square.c] = player;
            //++numFlipped;
        }
    }

    // Flip all flanked disc to the right of the placed piece
    for (int c = 1; c < distFlanking(board, square, "right"); ++c){
        if(nearbyChar(board, square, 0, c) == opponent){
            board[square.r][square.c + c] = player;
            ++numFlipped;
        }
    }

    // Flip all flanked disc to the left of the placed piece
    for (int c = 1; c < distFlanking(board, square, "left"); ++c){
        if(nearbyChar(board, square, 0, -c) == opponent){
            board[square.r][square.c - c] = player;
            ++numFlipped;
        }
    }
*/
}

int numLegalMoves(char** board, char player){

    int numLegalMoves = 0;

    for(int r = 0; r < boardSize; ++r){
        for(int c = 0; c < boardSize; ++c){
            gameSquare square;
            square.r = r;
            square.c = c;

            if(legalMove(board, player, square)){
                numLegalMoves++;
            }
        }
    }
    return numLegalMoves;
}

int printLegalMovesBoard(char** board, char player){
    
    int numLegalMoves = 0;

    printf("   ");
    for(int r = 0; r < boardSize; ++r){
        printf("%c ", r + 'A');
    }
    printf("\n");

    for(int r = 0; r < boardSize; ++r){
        printf("%2.d ", boardSize - r);
        for(int c = 0; c < boardSize; ++c){
            gameSquare square;
            square.r = r;
            square.c = c;

            if(legalMove(board, player, square)){
                printf("▫ ");
                numLegalMoves++;
            } else {
                if(board[r][c] == 'X'){
                    printf("● ");
                } else if(board[r][c] == 'O'){
                    printf("○ ");//● ○
                } else {
                    printf("  ");
                }
            }
        }
        printf("%d\n", boardSize - r);
    }

    printf("   ");
    for(int r = 0; r < boardSize; ++r){
        printf("%c ", r + 'A');
    }
    printf("\n");
    
    return numLegalMoves;
}
