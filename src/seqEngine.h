#ifndef __SEQ_ENGINE_H__
#define __SEQ_ENGINE_H__

#include "engine.h"
#include <utility>

class SequentialEng : public Engine {
public:
    SequentialEng(Board *chessBoard, int depth);
    virtual uint16_t getEngineMove() override;
    std::pair<uint16_t, int> negamax(int alpha, int beta, int depth);
};

#endif