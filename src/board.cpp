#include <iostream>
#include "board.h"

using namespace std;

#define FEN_STRING "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"

inline void Board::setBit(uint64_t& bitboard, int square) {
    bitboard |= (1ULL << square);
}
inline void Board::popBit(uint64_t& bitboard, int square) {
    bitboard &= ~(1ULL << square);
}
inline int Board::getBit(uint64_t bitboard, int square) {
    return (bitboard & (1ULL << square)) ? 1 : 0;
}

void Board::initializeBoard(string fen) {
    int row = 0, col = 0;
    for (char c : fen) {
        // Goes to new row
        if (c == '/') {
            col = 0;
            ++row;
        } else if ((c >= '0') && (c <= '9')) {
            for (int i = 0; i < c - '0'; ++i) {
                ++col;
            }
        } else {
            setSquare(c, row * 8 + col);
            ++col;
        }
    }
}

Board::Board() { initializeBoard(FEN_STRING); }
Board::Board(string& customFen) { initializeBoard(customFen); }

unordered_map<char, uint64_t>& Board::getBitMaps() {
    return bitmaps;
}
void Board::setSquare(char piece, int square) {
    setBit(bitmaps[piece], square);
}
char Board::getSquare(int square) {
    for (auto& bb : bitmaps) {
        if (getBit(bb.second, square)) {
            return bb.first;
        }
    }
    return '.';
}

void Board::undo(int num) {
    cout << "undo " << num << " moves" << endl;
}
void Board::render() {
    updateObservers();
}