#ifndef __MOVE_H__
#define __MOVE_H__

#include <stdint.h>

// Move type defined following the From-To Based format
// https://www.chessprogramming.org/Encoding_Moves
enum MoveType {
    QUIET, 
    DOUBLE_MOVE, 
    K_CASTLE, 
    Q_CASTLE, 
    CAPTURE,
    EN_PASSANT, 

    IGNORE1 = 6,
    IGNORE2 = 7,

    KNIGHT_PROMOTION,
    BISHOP_PROMOTION,
    ROOK_PROMOTION,
    QUEEN_PROMOTION,
    KNIGHT_PROMOTION_CAPTURE,
    BISHOP_PROMOTION_CAPTURE,
    ROOK_PROMOTION_CAPTURE,
    QUEEN_PROMOTION_CAPTURE
};

struct Move {
    uint16_t move; // 16-bit move encoding (6 source, 6 target, 4 flag)
    Move(int source, int target, MoveType moveType);
    Move(uint16_t move);
    int getSource() const;
    int getTarget() const;
    MoveType getMoveType() const;
    bool isPromotion() const;
    bool isCapture() const;
    bool isCastle() const;
};

#endif
