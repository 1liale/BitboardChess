#include "move.h"

// move = flag + dest + origin (ffff dddddd oooooo)
uint64_t encodeMove(MoveType flag, Square from, Square to) {
    return ((flag & 0xf) << 12) | ((from & 0x3f) << 6) | (to & 0x3f);
}
Move decodeMove(uint16_t move) {
    Square to = static_cast<Square>(move & 0x3f);
    move >>= 6;
    Square from = static_cast<Square>(move & 0x3f);
    move >>= 6;
    MoveType flag = static_cast<MoveType>(move & 0xf);
    return Move{flag, from, to};
}
