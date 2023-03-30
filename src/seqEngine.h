#ifndef __SEQ_ENGINE_H__
#define __SEQ_ENGINE_H__

#include "engine.h"
#include <utility>
#include <vector>
#include <unordered_map>

class SequentialEng : public Engine {
// std::vector<std::unordered_map<EncMove, int>> killers; TODO: IMPLEMENT KILLERS HEURISTICS
std::vector<EncMove> pvTable; // pv table used for move ordering

int getGamePhaseScore();
int heuristicOrder(EncMove move, int depth, int side);
std::vector<EncMove> sortMoves(std::vector<EncMove> moves, int depth, bool &isFollowPv);
std::pair<EncMove, int> alphabeta(int depth, int alpha, int beta,int maxSide, int side);

public:
    SequentialEng(Board *chessBoard, int depth);
    virtual EncMove getEngineMove() override;
    virtual int evaluateBoard(int side) override;
};

#endif