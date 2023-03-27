# BitboardChess

## Setup

> Prereq: Install Docker, see [here](https://docs.docker.com/get-docker/) for steps

1. Clone repository
2. Run `./setup` to set up and run Docker container
3. On startup of the container, run `cd src` to navigate to the mounted project directory

### Useful scripts
1. `./clean` to clean previous Makefile created objects 
(Call when switching between DEBUG, TEST or NORMAL play modes)
2. `./play [MODE]` // modes: DEBUG, TEST or NORMAL (default no param) 
* Note: DEBUG includes additional debug messages and TEST runs a local test suite to evaluate move generation

### Game Commands
```
1. To start a new game run
game [player1] [player2] // player can be of one of "basic", "sequential", "parallel" engines or "human" player
* note only human and basic players supported at this time
2. Once a game has started
    a. forfeit // player surrenders
    b. move [source] [target] [?p] // moves piece from source to target with the option to specify promotion piece
    c. move // just call move to call for engine to make a move

showstat // shows current game stats
exit // exits the game
```

## About
BitboardChess is the second chess engine that I've worked on and is written from scratch in C++. The purpose of this project is to challenge myself to design an optimized chess program that demonstrates experience working in C++, ability to handle a large codebase, and problem-solving skills.

It is an ongoing passion project that I started working on the side since December 1st, 2022.

Although this project was inspired by the first c++ chess engine I made in collaboration with Daniel Lee for the CS246 end of term project, they share almost no resemblance in either design or implementation. 

For the sake of academic integrity, I cannot release the other project publicly. However, it is available upon request for potential employers.

## Features

1. 64-bit Bitboard representation (for effective move storage, generation, and evaluation)
2. 16-bit move encoding (6 bits for source, 6 bits for target, and 4 bits for move type)
3. Magic-bitboard algorithm for sliding piece move generation (Bishop, Rook, Queen)
4. Demonstration of OOP principles with the use of Observer and MVC patterns
5. Piece square tables
6. Naive engine implementation with minimax tree search

## Next Steps
- Implement UCI protocols to compare against other engines and obtain an accurate ELO evaluation
- Implement a Concrete Observer for GUI (only text-based interaction is supported at this time)
- Explore the use of various engine heuristics and techniques such as:
1. Move ordering (Null, Killer, and History heuristics)
2. Negamax tree search
3. Transpositional table
4. (optionally) PolyGlot opening book support
5. (optionally) neural network trained weights for evalutaion used in StockFish 15
