#include "computer.h"
#include "board.h"
#include "seqEngine.h"
#include "parallelEngine.h"
#include "basicEngine.h"
#include "util.h"

using namespace std;

Computer::Computer(int side, string& engineStr, Board *chessBoard) : Player{side} {
    if (engineStr == "basic") {
        engine = make_unique<BasicEng>(chessBoard, prefDepth);
    }
    // sequential engine
    else if (engineStr == "sequential") {
        engine = make_unique<SequentialEng>(chessBoard, prefDepth);
    }
    // parallel engine
    else if (engineStr == "parallel") {
        engine = make_unique<ParallelEng>(chessBoard, prefDepth);
    }
}

int Computer::move(Board* chessBoard, istringstream &ss) {
    (void) ss;
    EncMove move = engine.get()->getEngineMove();
    cout << "engine move found: " << Move{move};
    return chessBoard->makeMove(move);
}
