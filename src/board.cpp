#include <iostream>
#include "board.h"
#include "util.h"

using namespace std;
using namespace bitutil;
using namespace definitions;

#define FEN_STRING "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"

void Board::initializeBoard(string fen) {
    pieceMaps = {
        {'p', 0ULL}, {'r', 0ULL},{'n', 0ULL}, {'b', 0ULL}, {'q', 0ULL}, {'k', 0ULL}, 
        {'P', 0ULL}, {'R', 0ULL},{'N', 0ULL}, {'B', 0ULL}, {'Q', 0ULL}, {'K', 0ULL}
    };
    memset(occupancyMaps, 0ULL, sizeof(occupancyMaps));
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
    computeOccupancyMaps()
}
Board::computeOccupancyMaps() {
    for (auto& bb : pieceMaps) {
        int side = isupper(bb.first) ? 0 : 1;
        occupancyMaps[side] |= pieceMaps[bb.first];
    }
    occupancyMaps[2] = occupancyMaps[0] | occupancyMaps[1];
}

Board::Board() { initializeBoard(FEN_STRING); }
Board::Board(string& customFen) { initializeBoard(customFen); }

void Board::setSquare(char piece, int square) {
    setBit(pieceMaps[piece], square);
}

void Board::removeSquare(char piece, int square) {
    popBit(pieceMaps[piece], square);
}

char Board::getSquare(int square) const {
    for (auto& bb : pieceMaps) {
        if (getBit(bb.second, square)) {
            return bb.first;
        }
    }
    return '.';
}

uint64_t Board::getOccupancyBySide(int side) const {
    return occupancyMaps[side];
}
uint64_t Board::getEmptySquares() const {
    return occupancyMaps[2] ^ (~0ULL);
}
uint64_t Board::getPieceBB(char piece) const {
    return pieceMaps[piece];
}

int Board::getKingSquare(int side) const {
    char king = side ? 'K' : 'k';
    return getLSBIndex(pieceMaps[king]);
}

vector<uint16_t> Board::generatePawnMoves(int side, vector<uint16_t>& moveslist) {
    int source, target;
    MoveType moveType;
    char piece = side ? 'P' : 'p';
    uint64_t bitboard = pieceMaps[piece], attacks;
    const int SQ_SHIFT = 8;

    while (bitboard) {
        source = getLSBIndex(bitboard);
        // set target to square in front 
        target = source + SQ_SHIFT * side == 0 ? -1 : 1;

        // generate pawn quiet moves
        if (!(target < a8 || target > h1) && !getBit(occupancyMaps[2], target)) {
            bool isWhitePromo = side == 0 && source >= a7 && source <= h7;
            bool isBlackPromo = side == 1 && source >= a2 && source <= h2;
            // promotion
            if (isWhitePromo || isBlackPromo) {
                Move promotePawn{source, target, QUEEN_PROMOTION}
                moveslist.push_back(promotePawn.move);
            } 
            // move or double move
            else {
                Move singleMovePawn{source, target, DOUBLE_MOVE};
                moveslist.push_back(singleMovePawn.move);
                // double move
                bool isWhiteDouble = side == 0 && source >= a2 && source <= h2;
                bool isBlackDouble = side == 1 && source >= a7 && source <= h7;
                int nextTarget = target + SQ_SHIFT * side == 0 ? -1 : 1;
                if ( (isWhiteDouble || isBlackDouble) && !getBit(occupancyMaps[2], nextTarget)) {
                    Move doubleMovePawn{source, nextTarget, DOUBLE_MOVE};
                    moveslist.push_back(doubleMovePawn.move);
                }
            }
        } 

        // generate pawn capture moves
        attacks = pawnAttacks[side][source] & occupancyMaps[abs(side - 1)];
        while () {
            
        }
    }
}

vector<uint16_t> Board::generateKnightMoves(int side, vector<uint16_t>& moveslist) {

}

vector<uint16_t> Board::generateBishopMoves(int side, vector<uint16_t>& moveslist) {
    
}

vector<uint16_t> Board::generateRookMoves(int side, vector<uint16_t>& moveslist) {
    
}

vector<uint16_t> Board::generateQueenMoves(int side, vector<uint16_t>& moveslist) {
    
}

vector<uint16_t> Board::generateKingMoves(int side, vector<uint16_t>& moveslist) {
    
}

vector<uint16_t> Board::generateLegalMoves(int side, uint64_t board) {
    vector<uint16_t> moveslist;

    generatePawnMoves(side, moveslist);
    generateKnightMoves(side, moveslist);
    generateBishopMoves(side, moveslist);
    generateRookMoves(side, moveslist);
    generateQueenMoves(side, moveslist);
    generateKingMoves(side, moveslist);

    return moveslist;
}

void Board::makeMove(uint64_t move) {}
void Board::makeMove(string& source, string& target) {}
void Board::makeNullMove() {}
void Board::undoMove() {}
void Board::undoNullMove() {}

void Board::render() {
    updateObservers();
}