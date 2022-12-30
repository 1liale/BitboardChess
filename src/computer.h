#ifndef __COMPUTER_H__
#define __COMPUTER_H__

#include "player.h"

class Board;

class Computer : public Player {
    public:
        Computer(bool isWhite, std::string& level);
        virtual int move(Board* b, std::istringstream& ss) override;
};

#endif
