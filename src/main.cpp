#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include "board.h"
#include "player.h"

using namespace std;

// Controller handles user inputs:
// commands: 
//     game [player1] [player2]     starts a new game
//     load [fen_string]            loads board state from fen_string
//     showstat                     show game stats
//     exit                         clean up memory and exit program
//     move [org] [dest]            attempts to move piece from org to dest 
//     forfeit                      player surrenders, prompts user again


class Controller {
    Board* chessBoard;
    Player* players[2];
    void setPlayer(int player) {
        
    }
    void loop() {

    }
public:
    Controller(Board* b): chessBoard{b} {}
    void start() {
        string inputs;
        // handles game command inputs
        while (getline(cin, inputs)) {
            try {
                istringstream ss{inputs};
                

            } catch(runtime_error& e) {
                cerr << e.what() << endl;
            }
        }
    }
};

int main() {
    Board chessBoard;
    Controller game{&chessBoard};
    game.start();
    return 0;
}
