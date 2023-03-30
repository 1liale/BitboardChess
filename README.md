# BitboardChess

## Setup

> Prereq: Install Docker, see [here](https://docs.docker.com/get-docker/) for steps

1. Clone repository
2. Run `./setup` to set up and run Docker container (for non-unix users, ignore steps 2 and 3 otherwise)
3. On startup of the container, run `cd src` to navigate to the mounted project directory

### Useful scripts
1. `./clean` to clean previous Makefile created objects
2. `./play [MODE]` // modes: DEBUG, TEST or NORMAL
* Note: DEBUG includes additional debug messages and TEST runs a local test suite to evaluate move generation

### Game Commands
```
1. To start a new game run
game [player1] [player2] // player can be of one of "basic", "sequential", "parallel" engines or "human" player
2. Once a game has started
    a. forfeit // player surrenders
    b. move [source] [target] [?p] // moves piece from source to target with the option to specify promotion piece
    c. move // just call move to call for engine to make a move

showstat // shows current game stats
exit // exits the game
```

> * note that the parallel player is not supported at this time and sequential runs on the local machine but doesn't appear to work on the ubuntu image on docker due to error caused by the nnue library used.

## About
BitboardChess is an experimental chess engine written from scratch in C++. It is an ongoing passion project that I started working on the side since December 1st, 2022. The purpose of this project is to challenge myself to design an optimized chess program that demonstrates experience working in C++, ability to handle a large codebase, and problem-solving skills.

Additionally, I wanted to use this opportunity to experiment working with various chess optimization techniques / heuristics as well as attempt to create a parallelized version using multithreading.

## Features

1. 64-bit Bitboard representation (for effective move storage, generation, and evaluation)
2. 16-bit move encoding (6 bits for source, 6 bits for target, and 4 bits for move type)
3. Magic-bitboard algorithm for sliding piece move generation (Bishop, Rook, Queen)
4. Demonstration of OOP principles with the use of Observer and MVC patterns
5. Piece square tables
6. Naive engine implementation with minimax tree search (*basic player*)
7. Improved implementation with alpha beta pruning on a minimax search tree(*sequential player*)
8. Move Ordering for Principal Variation
9. Iterative Deepening (in progress, will combine with Transposition Table and threading for greater effectiveness)

## Next Steps
- Implement UCI protocols to compare against other engines and obtain an accurate ELO evaluation
- Implement a Concrete Observer for GUI (only text-based interaction is supported at this time)
- Explore the use of various engine heuristics and techniques such as:
1. Transpositional table
2. Parallizing search (Try different techniques such as Lazy SMP, Principal Variation Split, Root Split, etc)
3. Experiment with Monte Carlo Tree Search like in AlphaZero
4. (optionally) PolyGlot opening book support