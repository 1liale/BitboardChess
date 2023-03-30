#ifndef __COMPUTER_H__
#define __COMPUTER_H__

#include <memory>
#include "player.h"
#include "engine.h"

class Board;

class Computer : public Player {
    int prefDepth = 6;
    std::unique_ptr<Engine> engine;
    public:
        Computer(int side, std::string& level, Board *chessBoard);
        virtual int move(Board* b, std::istringstream& ss) override;
};

#endif
