#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "types.h"
#include "logic.h"

struct Player players[4] = {
    {YELLOW, {-1,-1,-1,-1},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}},
    {BLUE, {-1,-1,-1,-1},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}},
    {RED, {-1,-1,-1,-1},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}},
    {GREEN, {-1,-1,-1,-1},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}
};


short int rolledvalue[4]; //store the rolled value
short int rounds = 0; //store the rounds of the game
short int mystery_cell = -1; //store the mystery cell number 
short int onBase[4] = {4,4,4,4}; //store the pieces on the base of each player 

void gameintro ()
{
    printf("\n\n");
    printf("The red player has four (04) pieces named R1, R2, R3, and R4.\n");
    printf("The yellow player has four (04) pieces named Y1, Y2, Y3, and Y4.\n");
    printf("The green player has four (04) pieces named G1, G2, G3, and G4.\n");
    printf("The blue player has four (04) pieces named B1, B2, B3, and B4.\n\n");
}

short int rando() 
{
    return (rand() % 6) + 1;
}

const char* colorToString(enum Color color) {
    switch (color) 
    {
        case YELLOW: return "yellow";
        case BLUE: return "blue";
        case RED: return "red";
        case GREEN: return "green";
    }
}

void findmax(short int rolledvalue[], struct Player players[], short int n) 
{
    short int max = 0;
    short int tie = 1;

    while (tie) {
        for (int i = 0; i < 4; ++i) {
            rolledvalue[i] = rando();
            if (rolledvalue[max] < rolledvalue[i]) {
                max = i;
            }
        }
        tie = 0;
        for (int i = 0; i < 4; ++i) {
            if (i != max && rolledvalue[i] == rolledvalue[max]) {
                tie = 1;
                break;
            }
        }
    }

    for (int i = 0; i < 4; ++i) {
        printf("%s rolls %d\n", colorToString(players[i].color), rolledvalue[i]);
    }
    printf("%s player has the highest roll and will begin the game.\n", colorToString(players[max].color));

    switch (max) 
    {
        case 0:
            players[0].color = YELLOW;
            players[1].color = BLUE;
            players[2].color = RED;
            players[3].color = GREEN;
            break;
        case 1:
            players[0].color = BLUE;
            players[1].color = RED;
            players[2].color = GREEN;
            players[3].color = YELLOW;
            break;
        case 2:
            players[0].color = RED;
            players[1].color = GREEN;
            players[2].color = YELLOW;
            players[3].color = BLUE;
            break;
        case 3:
            players[0].color = GREEN;
            players[1].color = YELLOW;
            players[2].color = BLUE;
            players[3].color = RED;
            break;
    }

    printf("The order of a single round is %s, %s, %s and %s.\n\n",
           colorToString(players[0].color),
           colorToString(players[1].color),
           colorToString(players[2].color),
           colorToString(players[3].color));
}

void choosedirection(short int currentplayer, short int pieceIndex, struct Player* players)
{
    short int dir = rand() % 2; // 0 for clockwise, 1 for counterclockwise
    switch (dir)
    {
    case 0: players[currentplayer].direction[pieceIndex] = CLOCKWISE; break;
    case 1: players[currentplayer].direction[pieceIndex] = COUNTER_CLOCKWISE; break;
    }
}

void movePieceToBoard(short int currentplayer, struct Player* players) 
{
    for (short int i = 0; i < PIECES_PER_PLAYER; i++) {
        if (players[currentplayer].position[i] == -1) // Piece is on the base
        {
            // Move piece to starting position based on color
            switch (players[currentplayer].color) 
            {
                case YELLOW: players[currentplayer].position[i] = YELLOWAPP + 2; break;
                case BLUE:   players[currentplayer].position[i] = BLUEAPP + 2; break;
                case RED:    players[currentplayer].position[i] = REDAPP + 2; break;
                case GREEN:  players[currentplayer].position[i] = GREENAPP + 2; break;
            }

            printf("%s player moves piece %d to the starting point.\n",
                   colorToString(players[currentplayer].color), i + 1);

            // Decrement the number of pieces on the base
            onBase[currentplayer] = onBase[currentplayer] -1; 

            // Calculate pieces on board
            short int piecesOnBoard = PIECES_PER_PLAYER - onBase[currentplayer];
            choosedirection(currentplayer,i,players);
            printf("%s player now has %d pieces on the board and %d pieces on the base.\n",
                   colorToString(players[currentplayer].color), piecesOnBoard, onBase[currentplayer]);

            return; // Exit after moving one piece
        }
    }
}

short int checkAndCapturePiece(short int currentPlayer, struct Player* players) 
{
    short int captureOccurred = 0;  // Flag to track if a capture has occurred
    
    for (short int i = 0; i < 4; i++) 
    {
        short int currentPos = players[currentPlayer].position[i];
        if (currentPos > -1) // Only consider pieces on the board
        {
            for (short int j = 0; j < PLAYERS; j++) 
            {
                if (j != currentPlayer) 
                {
                    for (short int k = 0; k < 4; k++) 
                    {
                        if (players[j].position[k] == currentPos) 
                        {
                            // Capture the piece
                            players[j].position[k] = -1; // Move the opponent's piece back to the base
                            players[j].cellstravelled[k] = 0; // Reset the distance traveled by the captured piece
                            players[j].direction[k] = 0; // Reset the direction (or set to a default)
                            players[j].piecescaptured[k] = 0; // Reset the captured pieces count for the opponent
                            onBase[j] = onBase[j] + 1;
                            short int piecesOnBoard = PIECES_PER_PLAYER - onBase[j];
                            printf("%s piece %c%d lands on square L%d, captures %s piece %c%d, and returns it to the base\n", 
                                   colorToString(players[currentPlayer].color),
                                   colorToString(players[currentPlayer].color)[0],i+1,
                                   (players[currentPlayer].position[i]),
                                   colorToString(players[j].color),
                                   colorToString(players[j].color)[0],j+1);

                            printf("%s player now has %d/4 on pieces on the board and %d/4 pieces on the base.\n",
                            colorToString(players[j].color),piecesOnBoard,onBase[j]);

                            players[currentPlayer].piecescaptured[i]++;
                            printf("%c%d has captured %d pieces\n", (colorToString(players[currentPlayer].color)[0]),
                                    i + 1, players[currentPlayer].piecescaptured[i]);

                            captureOccurred = 1;  // Set flag to indicate a capture occurred
                            break;  // Exit the inner loop after capturing
                        }
                    }
                }
            }
        }
    }

    return captureOccurred;  // Return whether a capture has occurred
}

void homestraight(short int currentPlayer, short int pieceIndex, struct Player* players) 
{
    if (players[currentPlayer].piecescaptured[pieceIndex] > 0) 
    {
        // Check if the piece has captured another piece in the second round
        if (players[currentPlayer].roundsPassAppraoch[pieceIndex] == 1)
        {
            if (players[currentPlayer].cellstravelled[pieceIndex] > BOARD_SIZE) 
            {
                players[currentPlayer].position[pieceIndex] = 
                ((players[currentPlayer].cellstravelled[pieceIndex] % BOARD_SIZE) + HOME_STRAIGHT_START) - 1;
            }
        }
        
        // Check if the piece has captured another piece in the first round
        if (players[currentPlayer].direction[pieceIndex] == CLOCKWISE) 
        {
            if (players[currentPlayer].cellstravelled[pieceIndex] > CLOCKWISE_CELLS_TRAVELLED_TO_HOME) 
            {
                players[currentPlayer].position[pieceIndex] = 
                ((players[currentPlayer].cellstravelled[pieceIndex] % CLOCKWISE_CELLS_TRAVELLED_TO_HOME) + HOME_STRAIGHT_START) - 1;
            }
        } 
        else 
        {
            if (players[currentPlayer].cellstravelled[pieceIndex] > COUNTER_CLOCKWISE_CELLS_TRAVELLED_TO_HOME) 
            {
                players[currentPlayer].position[pieceIndex] = 
                ((players[currentPlayer].cellstravelled[pieceIndex] % COUNTER_CLOCKWISE_CELLS_TRAVELLED_TO_HOME) + HOME_STRAIGHT_START) - 1;
            }
        }
    }
    
    if (players[currentPlayer].piecescaptured[pieceIndex] == 0)
    { 
        // Second round and more
        if (players[currentPlayer].roundsPassAppraoch[pieceIndex] == 1)
        {
            if (players[currentPlayer].cellstravelled[pieceIndex] > BOARD_SIZE)
            {
                players[currentPlayer].cellstravelled[pieceIndex] -= BOARD_SIZE;
                players[currentPlayer].roundsPassAppraoch[pieceIndex] = 1;
            }    
        }
        // First round
        if (players[currentPlayer].direction[pieceIndex] == CLOCKWISE) 
        {
            if (players[currentPlayer].cellstravelled[pieceIndex] > CLOCKWISE_CELLS_TRAVELLED_TO_HOME)
            {
                players[currentPlayer].cellstravelled[pieceIndex] -= CLOCKWISE_CELLS_TRAVELLED_TO_HOME;
                players[currentPlayer].roundsPassAppraoch[pieceIndex] = 1;
            }
        }
        else 
        {
            if (players[currentPlayer].cellstravelled[pieceIndex] > COUNTER_CLOCKWISE_CELLS_TRAVELLED_TO_HOME)
            {
                players[currentPlayer].cellstravelled[pieceIndex] -= COUNTER_CLOCKWISE_CELLS_TRAVELLED_TO_HOME;
                players[currentPlayer].roundsPassAppraoch[pieceIndex] = 1;
            }
        }
    }
}


// ---------------MYSTERY CELLS LOGIC ----------------------------------------------------

void cellstravelledMysterycell(short int currentPlayer, short int pieceindex, struct Player* players, short int cellnum)
{
    players[currentPlayer].roundsPassAppraoch[pieceindex] = 0;

    if (players[currentPlayer].direction[pieceindex] == CLOCKWISE)
    {
        // Bhawana
        if (cellnum == 1)
        {
            switch (players[currentPlayer].color) 
            {
                case YELLOW: players[currentPlayer].cellstravelled[pieceindex] = 7; break;   // YELLOW
                case BLUE: players[currentPlayer].cellstravelled[pieceindex] = 20; break;  // BLUE
                case RED: players[currentPlayer].cellstravelled[pieceindex] = 33; break;  // RED
                case GREEN: players[currentPlayer].cellstravelled[pieceindex] = 46; break;  // GREEN
            }
        }
        // Kotuwa
        if (cellnum == 2)
        {
            switch (players[currentPlayer].color) 
            {
                case YELLOW: players[currentPlayer].cellstravelled[pieceindex] = 25; break;   // YELLOW
                case BLUE: players[currentPlayer].cellstravelled[pieceindex] = 12; break;  // BLUE
                case RED: players[currentPlayer].cellstravelled[pieceindex] = 51; break;  // RED
                case GREEN: players[currentPlayer].cellstravelled[pieceindex] = 38; break;  // GREEN
            }
        }
        // Pitakotuwa
        if (cellnum == 3)
        {
            switch (players[currentPlayer].color) 
            {
                case YELLOW: players[currentPlayer].cellstravelled[pieceindex] = 44; break;   // YELLOW
                case BLUE: players[currentPlayer].cellstravelled[pieceindex] = 31; break;  // BLUE
                case RED: players[currentPlayer].cellstravelled[pieceindex] = 18; break;  // RED
                case GREEN: players[currentPlayer].cellstravelled[pieceindex] = 5; break;  // GREEN
            }
        }

        if (cellnum == 6)
        {
            players[currentPlayer].cellstravelled[pieceindex] = CLOCKWISE_CELLS_TRAVELLED_TO_HOME;
        }   
    }
    // Counterclockwise
    else 
    {
        // Bhawana
        if (cellnum == 1)
        {
            switch (players[currentPlayer].color) 
            {
                case YELLOW: players[currentPlayer].cellstravelled[pieceindex] = 45; break;   // YELLOW
                case BLUE: players[currentPlayer].cellstravelled[pieceindex] = 6; break;  // BLUE
                case RED: players[currentPlayer].cellstravelled[pieceindex] = 19; break;  // RED
                case GREEN: players[currentPlayer].cellstravelled[pieceindex] = 32; break;  // GREEN
            }
        }
        // Kotuwa
        if (cellnum == 2)
        {
            switch (players[currentPlayer].color) 
            {
                case YELLOW: players[currentPlayer].cellstravelled[pieceindex] = 27; break;   // YELLOW
                case BLUE: players[currentPlayer].cellstravelled[pieceindex] = 40; break;  // BLUE
                case RED: players[currentPlayer].cellstravelled[pieceindex] = 1; break;  // RED
                case GREEN: players[currentPlayer].cellstravelled[pieceindex] = 14; break;  // GREEN
            }
        }
        // Pitakotuwa
        if (cellnum == 3)
        {
            switch (players[currentPlayer].color) 
            {
                case YELLOW: players[currentPlayer].cellstravelled[pieceindex] = 8; break;   // YELLOW
                case BLUE: players[currentPlayer].cellstravelled[pieceindex] = 21; break;  // BLUE
                case RED: players[currentPlayer].cellstravelled[pieceindex] = 34; break;  // RED
                case GREEN: players[currentPlayer].cellstravelled[pieceindex] = 47; break;  // GREEN
            }
        }
        if (cellnum == 6)
        {
            players[currentPlayer].cellstravelled[pieceindex] = COUNTER_CLOCKWISE_CELLS_TRAVELLED_TO_HOME;
        } 
    }
}

void kotuwa(short int currentPlayer, short int pieceIndex, struct Player* players) 
{
    players[currentPlayer].position[pieceIndex] = 27;  // Kotuwa position from the yellow approach
    players[currentPlayer].roundsLeftInKotuwa[pieceIndex] = 4;  // Set to 4 rounds in Kotuwa
    players[currentPlayer].consecutiveThrees[pieceIndex] = 0;  // Reset the consecutive threes count
    
    printf("%s player's piece %c%d attends briefing and cannot move for four rounds.\n", colorToString(players[currentPlayer].color), colorToString(players[currentPlayer].color)[0], pieceIndex + 1);
}

void pitakotuwa(short int currentPlayer, short int pieceindex, struct Player* players, short int cellnum)
{
    if (players[currentPlayer].direction[pieceindex] == CLOCKWISE)
    {
        players[currentPlayer].direction[pieceindex] = COUNTER_CLOCKWISE;
        players[currentPlayer].position[pieceindex] = 46;
        cellstravelledMysterycell(currentPlayer, pieceindex, players, cellnum);
    }
    else
    {
        players[currentPlayer].position[pieceindex] = 27;
        cellstravelledMysterycell(currentPlayer, pieceindex, players, cellnum);
    }
}

void bhawana(short int currentPlayer, short int pieceindex, struct Player* players, short int startPosition, short int approachCell)
{
    players[currentPlayer].position[pieceindex] = 9; 
    short int effect = rando() % 2; // Randomly choose between energized (0) and sick (1)
    if (effect == 0) {
        players[currentPlayer].effectType[pieceindex] = 1; // Energized
        printf("%s player's piece %c%d feels energized, and movement speed doubles\n", colorToString(players[currentPlayer].color), colorToString(players[currentPlayer].color)[0], pieceindex + 1);
    } else {
        players[currentPlayer].effectType[pieceindex] = 2; // Sick
        printf("%s player's piece %c%d feels sick, and movement speed halves\n", colorToString(players[currentPlayer].color), colorToString(players[currentPlayer].color)[0], pieceindex + 1);
    }
    players[currentPlayer].effectRoundsLeft[pieceindex] = 4; // Set effect duration to 4 rounds
}

void updateMysteryCell(struct Player* players)
{
    short int occupiedCells[52] = {0}; // Array to track occupied cells
    short int availableCells[52];
    short int count = 0;

    // Mark occupied cells
    for (short int i = 0; i < PLAYERS; ++i) 
    {
        for (short int j = 0; j < 4; ++j) 
        {
            if (players[i].position[j] >= 0 && players[i].position[j] < 52) 
            {
                occupiedCells[players[i].position[j]] = 1; // Cell is occupied
            }
        }
    }

    // Collect all available cells
    for (short int i = 0; i < 52; ++i) 
    {
        if (!occupiedCells[i]) 
        {
            availableCells[count++] = i;
        }
    }

    // Randomly select one of the available cells
    if (count > 0) 
    {
        mystery_cell = availableCells[rand() % count];
    }
}

void mysterycellcheck(short int currentPlayer, short int roll, short int pieceindex, struct Player* players, short int startPosition, short int approachCell)
{
    // Update mystery cell if needed
    if (rounds == 2 || (rounds > 2 && (rounds - 2) % 4 == 0)) 
    {
        updateMysteryCell(players);
    }

    // Check if the piece is on the mystery cell
    if (players[currentPlayer].position[pieceindex] == mystery_cell) 
    {
        players[currentPlayer].roundsPassAppraoch[pieceindex] = 0;
        short int var = rando();
        char *mystery;
        switch (var)
        {
            case 1: mystery = "Bhawana"; break;
            case 2: mystery = "Kotuwa"; break;
            case 3: mystery = "Pita-Kotuwa"; break;
            case 4: mystery = "Base"; break;
            case 5: mystery = "X of the piece colour"; break;
            case 6: mystery = "Approach of the piece colour"; break; 
        }
        printf("%s player lands on a mystery cell and teleported to %s\n", colorToString(players[currentPlayer].color), mystery);
        switch (var)
        {
        case 1:
            printf("%s piece %c%d teleported to Bhawana\n", colorToString(players[currentPlayer].color),
            colorToString(players[currentPlayer].color)[0], pieceindex + 1);

            cellstravelledMysterycell(currentPlayer, pieceindex, players, var); 
            bhawana(currentPlayer, pieceindex, players, startPosition, approachCell); 
            break;
        
        case 2:
            printf("%s piece %c%d teleported to Kotuwa\n", colorToString(players[currentPlayer].color),
            colorToString(players[currentPlayer].color)[0], pieceindex + 1);
            cellstravelledMysterycell(currentPlayer, pieceindex, players, var);
            kotuwa(currentPlayer, pieceindex, players);
            break;
        case 3:
            printf("%s piece %c%d teleported to PitaKotuwa\n", colorToString(players[currentPlayer].color),
            colorToString(players[currentPlayer].color)[0], pieceindex + 1);  
            pitakotuwa(currentPlayer, pieceindex, players, var);
            break;
        case 4:
            printf("%s piece %c%d teleported to Base\n", colorToString(players[currentPlayer].color),
            colorToString(players[currentPlayer].color)[0], pieceindex + 1);
            players[currentPlayer].cellstravelled[pieceindex] = 0;
            players[currentPlayer].position[pieceindex] = -1;
            onBase[currentPlayer]++; 
            break;
        case 5:
            printf("%s piece %c%d teleported to X\n", colorToString(players[currentPlayer].color),
            colorToString(players[currentPlayer].color)[0], pieceindex + 1);
            players[currentPlayer].cellstravelled[pieceindex] = 0;
            players[currentPlayer].position[pieceindex] = startPosition;
            break;

        case 6:
            printf("%s piece %c%d teleported to Approach\n", colorToString(players[currentPlayer].color),
            colorToString(players[currentPlayer].color)[0], pieceindex + 1);
            players[currentPlayer].position[pieceindex] = approachCell;
            if (players[currentPlayer].direction[pieceindex] == CLOCKWISE)
            {
                players[currentPlayer].cellstravelled[pieceindex] = CLOCKWISE_CELLS_TRAVELLED_TO_HOME;
            }
            else
            {
                players[currentPlayer].cellstravelled[pieceindex] = COUNTER_CLOCKWISE_CELLS_TRAVELLED_TO_HOME;
            }
            break;
        }
    }
}


//----------------MOVING PIECES ON BOARD----------------------------------------------------

void movePieceOnBoard(short int currentPlayer, short int roll, struct Player* players) 
{
    short int startPosition, approachCell;

    switch (players[currentPlayer].color) {
        case YELLOW: approachCell = 0; startPosition = 2; break;   // YELLOW
        case BLUE: approachCell = 13; startPosition = 15; break;  // BLUE
        case RED: approachCell = 26; startPosition = 28; break;  // RED
        case GREEN: approachCell = 39; startPosition = 41; break;  // GREEN
    }

    for (short int i = 0; i < 4; i++) 
    {
        if (players[currentPlayer].position[i] == -2)   // Skip finished pieces
        {
            continue;
        }
        if (players[currentPlayer].position[i] < -1)    // Handle pieces in the home straight 
        {
            if (players[currentPlayer].position[i] + roll > -2) // Skip if the rolled value is higher than home 
            {
                break;
            }
            if (players[currentPlayer].position[i] + roll < -2) // Moving in the home straight
            {
                break; 
            }
            if (players[currentPlayer].position[i] + roll == -2) // Reach home
            {
                players[currentPlayer].position[i] += roll;
                printf("The piece %c%d has reached home\n", colorToString(players[currentPlayer].color)[0], i + 1);
                break;
            }
        }
        if (players[currentPlayer].position[i] == -1) // Skip the pieces in the base
        {
            continue;
        }

         // Apply Bhawana effect if any
        if (players[currentPlayer].effectRoundsLeft[i] > 0)
        {
            if (players[currentPlayer].effectType[i] == 1) // Energized
            {
                roll *= 2;
                printf("The piece %c%d is energized! Moving double the rolled value.\n", colorToString(players[currentPlayer].color)[0], i + 1);
            }
            else if (players[currentPlayer].effectType[i] == 2) // Sick
            {
                roll /= 2;
                printf("The piece %c%d is sick! Moving half the rolled value.\n", colorToString(players[currentPlayer].color)[0], i + 1);
            }

            players[currentPlayer].effectRoundsLeft[i]--;

            if (players[currentPlayer].effectRoundsLeft[i] == 0)
            {
                players[currentPlayer].effectType[i] = 0; // Reset effect
                printf("The piece %c%d is no longer under any effect.\n", colorToString(players[currentPlayer].color)[0], i + 1);
            }
        }
        
        if (players[currentPlayer].direction[i] == CLOCKWISE) // CLOCKWISE movement
        {
            printf("%s moves piece %d from location %d ", colorToString(players[currentPlayer].color), i + 1, players[currentPlayer].position[i]);
            players[currentPlayer].position[i] += roll;
            if (players[currentPlayer].position[i] > 51)
            {
                players[currentPlayer].position[i] -= 52;  
            }
            printf("to %d by %d units in clockwise direction.\n", players[currentPlayer].position[i], roll);
        }
        else // COUNTERCLOCKWISE movement
        {
            printf("%s moves piece %d from location %d ", colorToString(players[currentPlayer].color), i + 1, players[currentPlayer].position[i]);
            players[currentPlayer].position[i] -= roll;
            if (players[currentPlayer].position[i] < 0) 
            {
                players[currentPlayer].position[i] += 52;
            }
            printf("to %d by %d units in counter-clockwise direction.\n", players[currentPlayer].position[i], roll);
        }
        players[currentPlayer].cellstravelled[i] += roll;
        homestraight(currentPlayer, i, players);
        mysterycellcheck(currentPlayer, roll, i, players, startPosition, approachCell);
        break;
    }
}

short int handleConsecutiveSixes(short int rolledValue, short int* consecutiveSixes, struct Player* player) 
{
    if (rolledValue == 6) 
    {
        (*consecutiveSixes)++; // Increment the counter for consecutive sixes

        if (*consecutiveSixes == 3) 
        {
            printf("%s player rolled a third consecutive 6. Turn is skipped.\n", colorToString(player->color));
            return -1; // Return -1 to indicate the turn should end
        }

        return 6; // Return 6 to add to the piece's movement
    } 
    else 
    {
        *consecutiveSixes = 0; // Reset counter if the roll isn't a six
    }
    return rolledValue; // Return the rolled value for normal movement
}

short int printOutputOfPlayers(short int finishorder[])
{
    // Colour of the player
    for (short int i = 0; i < 4; ++i)
    {
        //skip players who have already finished 
        if (finishorder[i] != -1) 
        {
            continue;
        }

        short int piecesOnBoard = PIECES_PER_PLAYER - onBase[i];

        printf("\n\n");  // Corrected newline character
        // Printing general player information
        printf("Color %s player has %d/4 pieces on the board and %d/4 pieces on the base.\n", 
               colorToString(players[i].color), piecesOnBoard, onBase[i]);
        printf("=============================================================================================\n");
        printf("Location of %s pieces\n", colorToString(players[i].color));
        printf("=============================================================================================\n");
        // Piece of the player 
        for (short int j = 0; j < 4; ++j)
        {
            // Correctly handling switch cases and scoping
            switch (players[i].position[j])
            {
                case -1: 
                {
                    char *var = "base";
                    printf("Piece %c%d in %s\n", colorToString(players[i].color)[0], j + 1, var);
                    break;  // Added break statement
                }
                case -2: 
                {
                    char *var = "home";
                    printf("Piece %c%d in %s\n", colorToString(players[i].color)[0], j + 1, var);
                    break;  // Added break statement
                }
                default:
                    if (players[i].position[j] < -2)
                    {
                        printf("Piece %c%d in homepath L%d\n", colorToString(players[i].color)[0], j + 1, players[i].position[j] + 7);
                        break;
                    }
                    else
                    {
                        printf("Piece %c%d in L%d\n", colorToString(players[i].color)[0], j + 1, players[i].position[j]);
                        break;
                    }
            }
        }
    }

    return 0;  // Function should return an integer
}

short int KotuwaRollLogic(short int zerotothree, short int kotuwaPieceIndex, struct Player* players)
{
    for (short int pieceIndex = 0; pieceIndex < 4; ++pieceIndex) 
    {
        if (players[zerotothree].roundsLeftInKotuwa[pieceIndex] > 0) 
        {
            players[zerotothree].roundsLeftInKotuwa[pieceIndex]--;
            printf("%s player's piece %d is in Kotuwa for %d more rounds.\n", 
                   colorToString(players[zerotothree].color), pieceIndex + 1, 
                   players[zerotothree].roundsLeftInKotuwa[pieceIndex]);

            if (rolledvalue[zerotothree] == 3) 
            {
                players[zerotothree].consecutiveThrees[pieceIndex]++;
                if (players[zerotothree].consecutiveThrees[pieceIndex] == 3) 
                {
                    printf("%s player's piece %d rolled three consecutive threes and is teleported to the base!\n", 
                           colorToString(players[zerotothree].color), pieceIndex + 1);
                    players[zerotothree].position[pieceIndex] = -1;  // Move the piece to the base
                    players[zerotothree].cellstravelled[pieceIndex] = 0;
                    players[zerotothree].roundsLeftInKotuwa[pieceIndex] = 0;  // Reset Kotuwa rounds
                    players[zerotothree].consecutiveThrees[pieceIndex] = 0;  // Reset consecutive threes
                }
            } 
            else 
            {
                players[zerotothree].consecutiveThrees[pieceIndex] = 0;  // Reset if roll is not 3
            }

            // Skip this player's turn for this piece in Kotuwa
            kotuwaPieceIndex = pieceIndex;
        }
    }
}

void roll(short int rolledvalue[], struct Player players[]) 
{
    rounds = 0;  // Reset rounds at the start
    short int finishOrder[PLAYERS] = {-1, -1, -1, -1}; // The order of finishing players
    short int finishCount = 0;

    for (short int i = 1; i <= 1000; ++i) // Assuming 1000 rounds for the game
    {
        rounds++;  // Increment the round counter
        printf("\n\n");
        printf("Round %d\n", rounds);
    
        for (short int zerotothree = 0; zerotothree < PLAYERS; ++zerotothree) 
        {
            // Skip players who have already finished
            if (finishOrder[zerotothree] != -1) 
            {
                continue;
            }

            // Check if any piece is in Kotuwa and handle Kotuwa-specific logic
            short int kotuwaPieceIndex = -1; // Index of the piece in Kotuwa, if any
            
            KotuwaRollLogic(zerotothree, kotuwaPieceIndex, players);

            // If all pieces are not in Kotuwa, proceed with normal rolling logic
            if (kotuwaPieceIndex == -1) 
            {
                short int consecutiveSixes = 0; // Initialize counter for consecutive sixes
                short int playerTurnEnded = 0;  // Flag to indicate the end of the player's turn
                
                do {
                    rolledvalue[zerotothree] = rando();
                    printf("%s player rolled %d\n", colorToString(players[zerotothree].color), rolledvalue[zerotothree]);

                    short int movement = handleConsecutiveSixes(rolledvalue[zerotothree], &consecutiveSixes, &players[zerotothree]);

                    if (movement == -1) 
                    {
                        playerTurnEnded = 1; // End the player's turn
                        break;
                    }

                    if (rolledvalue[zerotothree] == 6)
                    {
                        short int pieceMovedToBoard = 0;
                        for (short int j = 0; j < 4; j++) 
                        {
                            if (players[zerotothree].position[j] == -1) 
                            {
                                movePieceToBoard(zerotothree, players);
                                pieceMovedToBoard = 1;
                                break;
                            }
                        }

                        if (!pieceMovedToBoard) 
                        {
                            movePieceOnBoard(zerotothree, movement, players); // Move by the accumulated sixes              
                        }
                    } 
                    else 
                    {
                        movePieceOnBoard(zerotothree, movement, players); // Move by the rolled value
                    }

                    // Check and capture a piece, allowing an extra turn if a capture occurs
                    if (checkAndCapturePiece(zerotothree, players) == 1) 
                    {
                        printf("%s player captured an opponent piece and gets an extra turn!\n", colorToString(players[zerotothree].color));
                        continue; // Allow another roll without ending the turn
                    }

                    playerTurnEnded = 1; // End player's turn since no additional rolls are awarded

                    // Check if all pieces have reached home for the player
                    if (players[zerotothree].position[0] == -2 && 
                        players[zerotothree].position[1] == -2 && 
                        players[zerotothree].position[2] == -2 && 
                        players[zerotothree].position[3] == -2) 
                    {
                        finishOrder[zerotothree] = finishCount + 1; // Record the player's finish order
                        finishCount++;
                        printf("%s player finished in position %d\n", colorToString(players[zerotothree].color), finishOrder[zerotothree]);
                        playerTurnEnded = 1; // End the player's turn
                    }

                    if (finishCount == 3) 
                    {
                        printf("Three players have finished!\n");
                        printf("End Game\n");
                        return; // End the game once all players have finished
                    }
                
                } while ((rolledvalue[zerotothree] == 6 || !playerTurnEnded)); // Continue if a 6 is rolled, a piece is captured, or turn isn't ended
            }
        }
        // Display the pieces and its positions
        printOutputOfPlayers(finishOrder);
        // Display the current mystery cell position
        if (rounds % 4 == 0)
        {
            printf("A mystery cell has spawned in location L%d and will be at this location for the next four rounds.\n", mystery_cell);
        }
    }
}

short int game()
{
    gameintro();
    findmax(rolledvalue, players, PLAYERS);
    roll(rolledvalue,players);
    return 0;
}
