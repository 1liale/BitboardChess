#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdint.h>

inline namespace bitutil {
    void setBit(uint64_t& bitboard, int square);
    void popBit(uint64_t& bitboard, int square);
    int getBit(uint64_t bitboard, int square);
    int countBits(uint64_t bitboard);
    int getLSBIndex(uint64_t bitboard);
}

inline namespace definitions {
    enum {
        a8, b8, c8, d8, e8, f8, g8, h8,
        a7, b7, c7, d7, e7, f7, g7, h7,
        a6, b6, c6, d6, e6, f6, g6, h6,
        a5, b5, c5, d5, e5, f5, g5, h5,
        a4, b4, c4, d4, e4, f4, g4, h4,
        a3, b3, c3, d3, e3, f3, g3, h3,
        a2, b2, c2, d2, e2, f2, g2, h2,
        a1, b1, c1, d1, e1, f1, g1, h1, NA
    }
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

    const uint64_t notAFile = 18374403900871474942ULL;
    const uint64_t notHFile = 9187201950435737471ULL;
    const uint64_t notHGFile = 4557430888798830399ULL;
    const uint64_t notABFile = 18229723555195321596ULL;
}

#endif