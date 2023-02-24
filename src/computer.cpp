#include "computer.h"
#include "util.h"

using namespace std;

Computer::Computer(bool isWhite, string& level) : Player{isWhite} {}
int Computer::move(Board* chessBoard, istringstream &ss) {
    return GAME_OVER;
}
