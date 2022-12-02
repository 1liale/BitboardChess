#include <iostream>
#include <utility>
#include <sstream>
#include <stdexcept>
#include <string>
#include "board.h"
#include "player.h"
#include "human.h"
#include "computer.h"

using namespace std;

// Controller controls game flow and handles user inputs:
// commands: 
//     game [player1] [player2]     starts a new game
//     load [fen_string]            loads board state from fen_string
//     showstat                     show game stats
//     exit                         clean up memory and exit program
//     move [org] [dest]            attempts to move piece from org to dest 
//     forfeit                      player surrenders, prompts user again


class Controller {
    int wins, losses, ties; // from player1's POV
    Board* chessBoard;
    Player* players[2];
    bool isGameSetup;
    int turn;

    void printStat() {
        cout << endl << "Game Stats" << endl;
        cout << "Games played: " << (wins + losses + ties) << endl;
        cout << "Player 1: " << wins << " wins, " << losses << " losses, " << ties << " ties" << endl;
        cout << "Player 2: " << losses << " wins, " << wins << " losses, " << ties << " ties" << endl; 
    }

    void setupPlayers(string& white, string& black) {
        if (white == "human") {
            players[0] = new Human(true);
        } else {
            players[0] = new Computer(true, white);
        }

        if (black == "human") {
            players[1] = new Human(false);
        } else {
            players[1] = new Computer(false, black);
        }
    }
public:
    Controller(): wins{0}, losses{0}, ties{0}, chessBoard{nullptr}, isGameSetup{false}, turn{0} {}
    ~Controller() {
        delete players[0];
        delete players[1];
        delete chessBoard;
    }
    void start() {
        string inputs;
        // handles game command inputs
        while (getline(cin, inputs)) {
            try {
                istringstream ss{inputs};
                string command;
                ss >> command;
                if (command == "exit") {
                    break;
                } else if (command == "showstat") {
                    printStat(); 
                }

                if (isGameSetup) {
                    Player* curPlayer = players[turn % 2];
                    turn++;
                    if (command == "move") {
                        int flag = curPlayer->move(chessBoard, ss);
                        if (flag == 0) {
                            ++wins;
                        } else if (flag == 1) {
                            ++losses;
                        } else if (flag == 2) {
                            ++ties;
                        } else if (flag == 3) {
                            chessBoard->print();
                        } else {
                            cout << "Illegal move, try again!" << endl;
                        }
                    } else if (command == "undo") {
                        int num = 1;
                        if (!ss.str().empty()) {
                            ss >> num;
                        }
                        chessBoard->undo(num);
                    } else if (command == "forfeit") {
                        if (turn % 2) {
                            ++losses;    
                        } else {
                            ++wins;
                        }
                        delete chessBoard;
                        isGameSetup = false;
                    } else {
                        cout << "Command not recognized, try again!" << endl;
                    }
                } else {
                    if (command == "game") {
                        chessBoard = new Board();
                        string white, black;
                        ss >> white;
                        ss >> black;
                        setupPlayers(white, black);
                        isGameSetup = true;
                    } 
                    // not essential, leave as is for now
                    else if (command == "load") {
                        string fenString;
                        ss >> fenString;
                        chessBoard = new Board(fenString);
                        isGameSetup = true;
                    } else {
                        cout << "Command not recognized, try again!" << endl;
                    }
                }
            } catch(runtime_error& e) {
                cerr << e.what() << endl;
            }
        }
    }
};

int main() {
    Controller game{};
    game.start();
    return 0;
}
