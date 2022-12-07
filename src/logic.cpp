#include "logic.h"

void Logic::setBit(uint64_t& bitboard, int square) {
    bitboard |= (1ULL << square);
}
void Logic::popBit(uint64_t& bitboard, int square) {
    bitboard &= ~(1ULL << square);
}
int Logic::getBit(uint64_t bitboard, int square) {
    return (bitboard & (1ULL << square)) ? 1 : 0;
}
int Logic::countBits(uint64_t bitboard) {
    return __builtin_popcountll(bitboard); 
}
int Logic::getLSBIndex(uint64_t bitboard) {
    return bitboard ? __builtin_ffsll(bitboard) : -1;
}