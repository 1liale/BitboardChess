#ifndef __MOVE_H__
#define __MOVE_H__

#include <stdint.h>
#include "util.h"

// Move type defined following the From-To Based format
// https://www.chessprogramming.org/Encoding_Moves

struct Move {
    uint16_t move; // 16-bit move encoding (6 source, 6 target, 4 flag)
    Move(int source, int target, MoveType moveType);
    Move(uint16_t move);
    int getSource() const;
    int getTarget() const;
    definitions::MoveType getMoveType() const;
    bool isPromotion() const;
    bool isCapture() const;
    bool isCastle() const;
};

#endif
