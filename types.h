#define BOARD_SIZE 52
#define PLAYERS 4
#define PIECES_PER_PLAYER 4
#define HOME_STRAIGHT_START -7

#define CLOCKWISE +1
#define COUNTER_CLOCKWISE -1
#define CLOCKWISE_CELLS_TRAVELLED_TO_HOME 50
#define COUNTER_CLOCKWISE_CELLS_TRAVELLED_TO_HOME 54

#define BHAWANA 9
#define KOTUWA 27
#define PITAKOTWA 46

#define YELLOWAPP 0
#define BLUEAPP 13
#define REDAPP 26
#define GREENAPP 39

#ifndef TYPES_H
#define TYPES_H

enum Color {YELLOW, BLUE, RED, GREEN };

struct Player {
    enum Color color;
    short int position[PIECES_PER_PLAYER];
    short int direction[PIECES_PER_PLAYER];
    short int cellstravelled[PIECES_PER_PLAYER];
    short int piecescaptured[PIECES_PER_PLAYER];

    short int effectType[PIECES_PER_PLAYER];
    short int effectRoundsLeft[PIECES_PER_PLAYER];

    short int roundsPassAppraoch[PIECES_PER_PLAYER];
    short int roundsLeftInKotuwa[PIECES_PER_PLAYER];
    short int consecutiveThrees[PIECES_PER_PLAYER];
};

extern struct Player players[4];

#endif







