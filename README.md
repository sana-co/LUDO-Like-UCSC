# 🎲 Ludo Game (C Implementation)

**Author:** Sanuth Wijayarathna  
**Date:** September 2024  

---

## 📌 Overview

This project is a C-based implementation of the traditional **Ludo board game**, inspired by the ancient Indian game *Pachisi*.  

### Enhancements
- Counter-clockwise movement  
- Mystery cells with special effects  

### Limitations
- Player behaviors not implemented  
- Block mechanics not included  

---

## 🗂️ Project Structure

```
main.c
logic.c
logic.h
types.h
```

---

## 📦 types.h

Defines constants, enums, and structures used in the game.

### 🔢 Constants

- `BOARDSIZE` – Total number of board cells (52)  
- `PLAYERS` – Number of players (4)  
- `PIECES_PER_PLAYER` – Pieces per player (4)  
- `HOMESTRAIGHTSTART` – Start of home stretch  
- `CLOCKWISE`, `COUNTERCLOCKWISE` – Movement directions  
- Travel constants for reaching home  
- Special cells:
  - `BHAWANA`
  - `KOTUWA`
  - `PITAKOTUWA`
- Player approach positions:
  - `YELLOWAPP`, `BLUEAPP`, `REDAPP`, `GREENAPP`

### 🧱 Player Structure

Stores all player-related data:

- `color` – Enum representing player color  
- `position[]` – Positions of pieces  
- `direction[]` – Movement direction  
- `cellstravelled[]` – Distance traveled  
- `piecescaptured[]` – Captured pieces count  
- `effectType[]` – Bhawana effects  
- `effectRoundsLeft[]` – Effect duration  
- `roundsPassApproach[]` – Rounds passed approach  
- `roundsLeftInKotuwa[]` – Rounds stuck in Kotuwa  
- `consecutiveThrees[]` – Tracks consecutive rolls  

### ⚙️ Design Advantages

- Efficient memory usage (contiguous structure)  
- Improved cache performance  
- Better data integrity  
- Improved readability using enums and macros  

---

## 📦 logic.h

Header file containing function declarations.

### Key Concepts

- **Encapsulation** – Separates interface from implementation  
- **Abstraction** – Hides implementation details  
- **Modularity** – Organizes code into logical components  
- **Reusability** – Shared across multiple source files  
- **Maintainability** – Centralized updates  
- **Compile-time checking** – Ensures type safety  

---

## ⚙️ logic.c

Contains the full game logic.

### 🔧 Initialization

- Initializes 4 players with identical starting states  
- Uses arrays for efficient access  

Example:
```c
players[0].position[0];
```

---

### 🎲 Core Functions

#### `gameintro()`
Displays player and game details.

#### `rando()`
Generates random numbers (1–6).

#### `findmax()`
Determines player order based on dice rolls.

#### `roll()`
Main game loop:
- Handles turns  
- Rolls dice  
- Moves pieces  
- Checks win conditions  

---

### 🚶 Movement Functions

#### `movePieceToBoard()`
- Moves a piece from base to board  
- Time Complexity: **O(P)**  

#### `movePieceOnBoard()`
- Handles movement within the board  
- Supports direction changes and effects  
- Time Complexity: **O(P)**  

---

## 🎯 Capture System

#### `checkAndCapturePiece()`

- Detects overlapping pieces  
- Sends opponent piece back to base  
- Updates game state  
- Returns capture status  

---

## 🎭 Mystery Cell System

Introduces randomness and strategy.

### 🔮 Functions

#### `cellstravelledMysterycell()`
- Updates movement after mystery cell  
- Complexity: **O(1)**  

#### `kotuwa()`
- Sends piece to Kotuwa (stuck for 4 rounds)

#### `pitakotuwa()`
- Changes direction or redirects to Kotuwa  

#### `bhawana()`
Applies random effect:
- **Energized** → double movement  
- **Sick** → half movement  

#### `updateMysteryCell()`
- Randomly changes mystery cell position  

#### `mysterycellcheck()`
- Detects landing on mystery cell  
- Applies corresponding effect  

---

## 🚀 main.c

Program entry point.

### Execution Flow

```c
srand(time(NULL));
game();
return 0;
```

- Seeds randomness  
- Starts the game  
- Ends execution  

---

## ⚡ Key Features

- Turn-based multiplayer gameplay  
- Clockwise & counterclockwise movement  
- Mystery cell mechanics  
- Capture system  
- Modular architecture  

---

## ⚠️ Limitations

- No block mechanics  
- No advanced AI/player behavior  

---

## 📈 Complexity Summary

| Function                | Complexity |
|------------------------|-----------|
| Movement functions     | O(P)      |
| Capture logic          | O(P²)     |
| Mystery cell functions | O(1)      |

---

## 🧠 Future Improvements

- Add AI/player strategies  
- Implement blocking mechanics  
- Improve UI (CLI → GUI)  
- Optimize capture system  

---

## 🏁 Conclusion

This project demonstrates:

- Modular C programming  
- Efficient memory usage  
- Game logic design  
- Use of randomness and effects  

---

## 📚 Reference

Original project documentation: :contentReference[oaicite:0]{index=0}

---
