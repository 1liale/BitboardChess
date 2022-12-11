#include "util.h"

void bitutil::setBit(uint64_t& bitboard, int square) {
    bitboard |= (1ULL << square);
}
void bitutil::popBit(uint64_t& bitboard, int square) {
    bitboard &= ~(1ULL << square);
}
int bitutil::getBit(uint64_t bitboard, int square) {
    return (bitboard & (1ULL << square)) ? 1 : 0;
}
int bitutil::countBits(uint64_t bitboard) {
    return __builtin_popcountll(bitboard); 
}
int bitutil::getLSBIndex(uint64_t bitboard) {
    return __builtin_ffsll(bitboard) - 1;
}