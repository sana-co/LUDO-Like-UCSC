#ifndef LOGIC_H
#define LOGIC_H

void gameintro();

short int rando();

const char* colorToString(enum Color color);

void findmax(short int rolledvalue[], struct Player players[], short int n);

void choosedirection(short int currentplayer, short int pieceIndex, struct Player* players);

void movePieceToBoard(short int currentplayer, struct Player* players);

short int checkAndCapturePiece(short int currentPlayer, struct Player* players);

void homestraight(short int currentPlayer, short int pieceIndex, struct Player* players);

//-----------------MYSTERY CELL LOGICS -----------------------------------------------------

void cellstravelledMysterycell(short int currentPlayer, short int pieceindex, struct Player* players, short int cellnum);

void kotuwa(short int currentPlayer, short int pieceIndex, struct Player* players);

void pitakotuwa(short int currentPlayer, short int pieceindex, struct Player* players, short int cellnum);

void bhawana(short int currentPlayer, short int pieceindex, struct Player* players, short int startPosition, short int approachCell);

void updateMysteryCell(struct Player* players);

void mysterycellcheck(short int currentPlayer, short int roll, short int pieceindex, struct Player* players, short int startPosition, short int approachCell);

//----------------MOVING PIECES ON BOARD----------------------------------------------------

void movePieceOnBoard(short int currentPlayer, short int roll, struct Player* players);

short int handleConsecutiveSixes(short int rolledValue, short int* consecutiveSixes, struct Player* player);

short int printOutputOfPlayers(short int finishorder[]);

short int KotuwaRollLogic(short int zerotothree, short int kotuwaPieceIndex, struct Player* players);

void roll(short int rolledvalue[], struct Player players[]);

short int game();

#endif