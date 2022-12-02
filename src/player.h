#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "move.h"

class Player {
public:
    enum PlayerType {
        HUMAN,
        COMPUTER_1,
        COMPUTER_2
    };

    Player(PlayerType type, bool isWhite);
    virtual ~Player();
    virtual Move move(std::istringstream& ss) = 0;
};

#endif
