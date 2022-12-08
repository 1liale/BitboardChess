#ifndef __BITUTIL_H__
#define __BITUTIL_H__

#include <stdint.h>

inline namespace bitutil {
    const uint64_t notAFile = 18374403900871474942ULL;
    const uint64_t notHFile = 9187201950435737471ULL;
    const uint64_t notHGFile = 4557430888798830399ULL;
    const uint64_t notABFile = 18229723555195321596ULL;

    void setBit(uint64_t& bitboard, int square);
    void popBit(uint64_t& bitboard, int square);
    int getBit(uint64_t bitboard, int square);
    int countBits(uint64_t bitboard);
    int getLSBIndex(uint64_t bitboard);
}

#endif