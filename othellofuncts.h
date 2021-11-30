

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define boardSize 8

typedef struct gameSquare_struct {
    int r;
    int c;
} gameSquare;

void printBoard(char** board);

void resetBoard(char** board);

void getNumDiscs(char** board, int* black, int* white);

char legalPlayerChar(char player, char* funcName);

gameSquare getMove();

gameSquare randomMove(char** board, char player);

// gets identity of nearby character relative to 
char nearbyChar(char** board, gameSquare square, int rowOffset, int colOffset);

// returns distance to flanking token, 0 if no flanking token
int distFlanking(char** board, gameSquare square, const char* direction);

// returns number of flipped discs
int flipFlanked(char** board, gameSquare square);

bool legalMove(char** board, char player, gameSquare square);

// returns number of flipped discs
int takeMove(char** board, char player, gameSquare square);

int numLegalMoves(char** board, char player);

int printLegalMovesBoard(char** board, char player);
