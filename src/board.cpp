#include <iostream>
#include "board.h"
#include "bitutil.h"

using namespace std;
using namespace bitutil;

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

    // init occupancy maps
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

char Board::getSquare(int square) {
    for (auto& bb : pieceMaps) {
        if (getBit(bb.second, square)) {
            return bb.first;
        }
    }
    return '.';
}

vector<uint16_t> generateLegalMoves() {
    return {};
}

void makeMove(uint16_t move) {}

void makeMove(string from, string to) {}

void Board::undoMove(int num) {
    cout << "undo " << num << " moves" << endl;
}
void Board::render() {
    updateObservers();
}