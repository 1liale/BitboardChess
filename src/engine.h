#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "stdint.h"

class Board;

class Engine {
protected:
    Board *chessBoard;
    int depth;
    
public:
    Engine(Board *b, int depth);
    virtual ~Engine();
    virtual uint16_t getEngineMove() = 0;
};
#endif