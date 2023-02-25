#ifndef __BASIC_ENGINE_H__
#define __BASIC_ENGINE_H__

#include "engine.h"
#include <utility>
#include <unordered_map>

class BasicEng : public Engine {
    std::unordered_map<char,int> weightMap;
    const short pawnTable[64];
    const short knightTable[64]; 
    const short bishopTable[64];
    const short kingTable[64];

    int evaluateBoard(int maxSide);
    int evaluatePiece(char piece, int square, int side);
public:
    BasicEng(Board *chessBoard, int depth);
    virtual uint16_t getEngineMove() override;
    std::pair<uint16_t, int> minimax(int depth, bool maxPlayer, int maxSide, int side);
};

#endif