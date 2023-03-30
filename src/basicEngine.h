#ifndef __BASIC_ENGINE_H__
#define __BASIC_ENGINE_H__

#include "engine.h"
#include <utility>
#include <unordered_map>

class BasicEng : public Engine {
    int evaluatePiece(char piece, int square, int side);
    std::pair<EncMove, int> minimax(int depth, bool maxPlayer, int maxSide, int side);
public:
    BasicEng(Board *chessBoard, int depth);
    virtual EncMove getEngineMove() override;
    virtual int evaluateBoard(int side) override;
};

#endif