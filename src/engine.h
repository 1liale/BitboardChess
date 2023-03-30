#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "stdint.h"
#include "util.h"

class Board;

class Engine {
protected:
    Board *chessBoard;
    int depth;
    
public:
    Engine(Board *b, int depth);
    virtual ~Engine();
    virtual EncMove getEngineMove() = 0;
    virtual int evaluateBoard(int side) = 0;
};
#endif