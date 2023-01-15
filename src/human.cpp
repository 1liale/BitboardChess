#include "human.h"
#include "board.h"
#include <iostream>

using namespace std;

Human::Human(bool isWhite) : Player{isWhite} {}
int Human::move(Board* chessBoard, istringstream &ss) {
    string source, target;
    char promote = 'x';
    ss >> source;
    ss >> target;
    ss >> promote;
    return chessBoard->makeMove(source, target, promote);
}
