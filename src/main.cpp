#include <iostream>
#include <utility>
#include <sstream>
#include <stdexcept>
#include <string>
#include "board.h"
#include "player.h"
#include "human.h"
#include "computer.h"
#include "observer.h"
#include "textObserver.h"
// #include "guiObserver.h"  TODO: ADD GUI WHEN ALL ELSE IS COMPLETE

using namespace std;

// REMINDER: PREMATURE OPTIMIZATION IS ROOT OF ALL EVIL

// Controller controls game flow and handles user inputs:
// commands: 
//     game [player1] [player2]     starts a new game
//     load [fen_string]            loads board state from fen_string
//     showstat                     show game stats
//     exit                         clean up memory and exit program
//     move [source] [target] [?p]  move piece from source to target with the option to promote
//     forfeit                      player surrenders


class Controller {
    int wins, losses, draws; // from player1's POV
    Board* chessBoard;
    Player* players[2];
    vector<Observer*> observers;
    bool isGameSetup;

    void printStat() {
        cout << endl << "Games played: " << (wins + losses + draws) << endl;
        cout << "Player 1: " << wins << " wins, " << losses << " losses, " << draws << " draws" << endl;
        cout << "Player 2: " << losses << " wins, " << wins << " losses, " << draws << " draws" << endl << endl; 
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

    // TODO: (LOW PRIORITY) ADJUST GAME STATE TO SIDE AND FLAG
    // 0 = lost, 1 = win, 2 = tie
    void updateGameState(int side, int flag) {
        switch(flag) {
            case ILLEGAL_MOVE:
                throw runtime_error("Illegal move, try again!");
            case GAME_OVER:
                if (side == WHITE_SIDE) ++wins;
                else ++losses;
                break;
            case DRAW:
                ++draws;
                break;
            default:
                break;
        }
    }

    void handleRunningGame(string& command, istringstream& ss) {
        int side = chessBoard->getSide();
        Player* curPlayer = players[side];
        if (command == "move") {
            int flag = curPlayer->move(chessBoard, ss);
            // need to handle illegal move flag + win condition (update score + reset board)
            updateGameState(side, flag);
        } else if (command == "undo") {
            int num = 1;
            if (!ss.str().empty()) {
                ss >> num;
            }
            for (int i = 0; i < num; ++i) {
                chessBoard->undoMove();
            }
        } else if (command == "forfeit") {
            updateGameState(side, GAME_OVER);
            isGameSetup = false;
        } else {
            throw runtime_error("Command not recognized, try again!"); 
        }
    }

    void handleSetup(string& command, istringstream& ss) {
        if (command != "game" && command != "load") {
            throw runtime_error("Command not recognized, try again!"); 
        }

        if (command == "game") {
            chessBoard = new Board();
            setupPlayers(ss);
        } 
        // not essential, leave as is for now
        else if (command == "load") {
            setupPlayers(ss);
            string fenString;
            ss >> fenString;
            chessBoard = new Board(fenString);
        }
        isGameSetup = true;
        observers.clear(); 
        observers.push_back(new TextObserver(chessBoard));
        // observers.push_back(new GuiObserver(chessBoard));
        chessBoard->render();
    }

public:
    Controller(): wins{0}, losses{0}, draws{0}, chessBoard{nullptr}, isGameSetup{false} {}
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
                    continue;
                }

                if (isGameSetup) {
                    handleRunningGame(command, ss);
                    chessBoard->render();
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
