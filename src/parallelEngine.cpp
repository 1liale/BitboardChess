#include "parallelEngine.h"
#include "board.h"

using namespace std;

ParallelEng::ParallelEng(Board *chessBoard, int depth): Engine(chessBoard, depth) {}
uint16_t ParallelEng::getEngineMove() {
    return 0;
}