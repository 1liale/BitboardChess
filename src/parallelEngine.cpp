#include "parallelEngine.h"
#include "board.h"

using namespace std;

ParallelEng::ParallelEng(Board *chessBoard, int depth): Engine(chessBoard, depth) {}
EncMove ParallelEng::getEngineMove() {
    return 0;
}
int ParallelEng::evaluateBoard(int side) {
    return 0;
}