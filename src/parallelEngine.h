#ifndef __PARALLEL_ENGINE_H__
#define __PARALLEL_ENGINE_H__

#include "engine.h"

class ParallelEng: public Engine {
public:
    ParallelEng(Board *chessBoard, int depth);
    virtual EncMove getEngineMove() override;
    virtual int evaluateBoard(int side) override;
};

#endif