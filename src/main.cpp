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

// IN PROGRESS: PREMATURE OPTIMIZATION IS ROOT OF ALL EVIL

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

    void setupPlayers(istringstream& ss) {
        string white, black;
        ss >> white;
        ss >> black;
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

    // 0 = lost, 1 = win, 2 = tie
    void updateGameState(int flag) {
        switch(flag) {
            case 0:
                losses++;
                break;
            case 1:
                wins++;
                break;
            case 2:
                ties++;
                break;
            default:
                break;
        }
    }

    void handleRunningGame(string& command, istringstream& ss) {
        Player* curPlayer = players[turn % 2];
        if (command == "move") {
            int flag = curPlayer->move(chessBoard, ss);
            if (flag == 1) {
                chessBoard->print();
            } else if (flag == 2) {
                cout << "Illegal move, try again!" << endl;
            } else {
                if (flag >= 3) {
                    cout << "Game has ended, you can create a new game or exit" << endl;
                    isGameSetup = false;
                }
                if (flag == 3) flag = turn % 2 + 1;
                else if (flag == 4) flag = 2;
                else flag = -1;
                updateGameState(flag);  
            }
        } else if (command == "undo") {
            int num = 1;
            if (!ss.str().empty()) {
                ss >> num;
            }
            chessBoard->undo(num);
        } else if (command == "forfeit") {
            updateGameState(turn % 2);
            isGameSetup = false;
        } else {
            cout << "Command not recognized, try again!" << endl;
        }
    }

    void handleSetup(string& command, istringstream& ss) {
        if (command == "game") {
            chessBoard = new Board();
            setupPlayers(ss);
            isGameSetup = true;
            chessBoard->print();
        } 
        // not essential, leave as is for now
        else if (command == "load") {
            string fenString;
            ss >> fenString;
            chessBoard = new Board(fenString);
            isGameSetup = true;
            chessBoard->print();
        } else {
            cout << "Command not recognized, try again!" << endl;
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
                    handleRunningGame(command, ss);
                } else {
                    handleSetup(command, ss);
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
