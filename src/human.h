#ifndef __HUMAN_H__
#define __HUMAN_H__

#include "player.h"

class Board;

class Human : public Player {
public:
    Human(bool isWhite);
    virtual int move(Board* chessBoard, std::istringstream &ss) override;
};

#endif
