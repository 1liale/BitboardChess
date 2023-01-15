#include "util.h"
#include <iostream>

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
int helpers::getSquareFromStr(std::string& coord) {
    return coord[0] - 'a' + ('8' - coord[1]) * BOARD_WIDTH;
}
uint64_t helpers::maskPawnAttacks(int side, int square) {
    uint64_t rays = 0ULL, bb = 0ULL;
    bitutil::setBit(bb, square);

    if (side == WHITE_SIDE) {
        if ((bb >> 7) & NOT_A_FILE) rays |= (bb >> 7);
        if ((bb >> 9) & NOT_H_FILE) rays |= (bb >> 9);
    } else {
        if ((bb << 9) & NOT_A_FILE) rays |= (bb << 9);
        if ((bb << 7) & NOT_H_FILE) rays |= (bb << 7);
    }

    return rays;
}
uint64_t helpers::maskKnightAttacks(int square) {
    uint64_t rays = 0ULL, bb = 0ULL;
    bitutil::setBit(bb, square);

    if ((bb >> 17) & NOT_H_FILE) rays |= (bb >> 17);
    if ((bb >> 15) & NOT_A_FILE) rays |= (bb >> 15);
    if ((bb << 6) & NOT_HG_FILE) rays |= (bb << 6);
    if ((bb << 10) & NOT_AB_FILE) rays |= (bb << 10);
    if ((bb << 17) & NOT_A_FILE) rays |= (bb << 17);
    if ((bb << 15) & NOT_H_FILE) rays |= (bb << 15);
    if ((bb >> 6) & NOT_AB_FILE) rays |= (bb >> 6);
    if ((bb >> 10) & NOT_HG_FILE) rays |= (bb >> 10);

    return rays;
}
uint64_t helpers::maskKingAttacks(int square) {
    uint64_t rays = 0ULL, bb = 0ULL;
    bitutil::setBit(bb, square);

    if ((bb >> 9) & NOT_H_FILE) rays |= (bb >> 9);
    if (bb >> 8) rays |= (bb >> 8);
    if ((bb >> 7) & NOT_A_FILE) rays |= (bb >> 7);
    if ((bb >> 1) & NOT_H_FILE) rays |= (bb >> 1);
    if ((bb << 1) & NOT_A_FILE) rays |= (bb << 1);
    if ((bb << 7) & NOT_H_FILE) rays |= (bb << 7);
    if (bb << 8) rays |= (bb << 8);
    if ((bb << 9) & NOT_A_FILE) rays |= (bb << 9);

    return rays;
}
uint64_t helpers::maskBishopAttacks(int square) {
    uint64_t rays = 0ULL;
    int row = square / BOARD_WIDTH;
    int col = square % BOARD_WIDTH;

    for (int i = row + 1, j = col + 1; i <= 6 && j <= 6; i++, j++) rays |= (1ULL << (i * 8 + j)); // top-right
    for (int i = row - 1, j = col + 1; i >= 1 && j <= 6; i--, j++) rays |= (1ULL << (i * 8 + j)); // bottom-right 
    for (int i = row + 1, j = col - 1; i <= 6 && j >= 1; i++, j--) rays |= (1ULL << (i * 8 + j)); // top-left
    for (int i = row - 1, j = col - 1; i >= 1 && j >= 1; i--, j--) rays |= (1ULL << (i * 8 + j)); // bottom-left

    return rays;
}
uint64_t helpers::maskRookAttacks(int square) {
    uint64_t rays = 0ULL;
    int row = square / BOARD_WIDTH;
    int col = square % BOARD_WIDTH;

    for (int i = row + 1; i <= 6; i++) rays |= (1ULL << (i * 8 + col));
    for (int i = row - 1; i >= 1; i--) rays |= (1ULL << (i * 8 + col));
    for (int j = col + 1; j <= 6; j++) rays |= (1ULL << (row * 8 + j));
    for (int j = col - 1; j >= 1; j--) rays |= (1ULL << (row * 8 + j));

    return rays;
}
uint64_t helpers::maskBishopAttacksWithBlocks(int square, uint64_t block) {
    uint64_t attacks = 0ULL, bishopPos;
    int row = square / BOARD_WIDTH;
    int col = square % BOARD_WIDTH;

    for (int i = row + 1, j = col + 1; i <= 7 && j <= 7; ++i, ++j) {
        bishopPos = 1ULL << (i * BOARD_WIDTH + j);
        attacks |= bishopPos;
        if (bishopPos & block) break;
    }
    for (int i = row - 1, j = col + 1; i >= 0 && j <= 7; --i, ++j) {
        bishopPos = 1ULL << (i * BOARD_WIDTH + j);
        attacks |= bishopPos;
        if (bishopPos & block) break;
    }
    for (int i = row + 1, j = col - 1; i <= 7 && j >= 0; ++i, --j) {
        bishopPos = 1ULL << (i * BOARD_WIDTH + j);
        attacks |= bishopPos;
        if (bishopPos & block) break;
    }
    for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; --i, --j) {
        bishopPos = 1ULL << (i * BOARD_WIDTH + j);
        attacks |= bishopPos;
        if (bishopPos & block) break;
    }

    return attacks;
}
uint64_t helpers::maskRookAttacksWithBlocks(int square, uint64_t block) {
    uint64_t attacks = 0ULL, rookPos;
    int row = square / BOARD_WIDTH;
    int col = square % BOARD_WIDTH;

    for (int i = row + 1; i <= 7; ++i) {
        rookPos = 1ULL << (i * BOARD_WIDTH + col);
        attacks |= rookPos;
        // encounters obstacle
        if (rookPos & block) break;
    }
    for (int i = row - 1; i >= 0; --i) {
        rookPos = 1ULL << (i * BOARD_WIDTH + col);
        attacks |= rookPos;
        // encounters obstacle
        if (rookPos & block) break;
    }
    for (int j = col + 1; j <= 7; ++j) {
        rookPos = 1ULL << (row * BOARD_WIDTH + j);
        attacks |= rookPos;
        // encounters obstacle
        if (rookPos & block) break;
    }
    for (int j = col - 1; j >= 0; --j) {
        rookPos = 1ULL << (row * BOARD_WIDTH + j);
        attacks |= rookPos;
        // encounters obstacle
        if (rookPos & block) break;
    }

    return attacks;
}
void helpers::prettyPrintBB(uint64_t bb) {
    for (int i = 0; i < BOARD_WIDTH; ++i) {
        std::string tmp;
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            tmp += (bb & (1ULL << j)) ? "x " : ". ";
        }
        std::cout << tmp << std::endl;
        bb >>= BOARD_WIDTH;
    }
    std::cout << std::endl;
}
uint64_t helpers::setOccupancy(int index, int bitsCount, uint64_t mask) {
    // occupancy map
    uint64_t occupancy = 0ULL;
    
    // loop over the range of bits within attack mask
    for (int i = 0; i < bitsCount; i++)
    {
        int square = getLSBIndex(mask);
        popBit(mask, square);
        
        if (index & (1 << i))
            occupancy |= (1ULL << square);
    }
    
    // return occupancy map
    return occupancy;
}
