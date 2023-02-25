#include "seqEngine.h"
#include "board.h"

using namespace std;

SequentialEng::SequentialEng(Board *chessBoard, int depth): Engine(chessBoard, depth) {}
uint16_t SequentialEng::getEngineMove() {
    Move emptyMove{e7, e5, QUIET};
    return emptyMove.move;
}

pair<uint16_t, int> SequentialEng::negamax(int alpha, int beta, int depth) {
    return {NULL, 0};
}


