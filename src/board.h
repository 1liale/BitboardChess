#ifndef __BOARD_H__
#define __BOARD_H__

#include <string>
#include <unordered_map>
#include "subject.h"

class Board : public Subject {
private:
    std::unordered_map<char, uint64_t> bitmaps = {
        {'p', 0ULL}, {'r', 0ULL},{'n', 0ULL}, {'b', 0ULL}, {'q', 0ULL}, {'k', 0ULL}, 
        {'P', 0ULL}, {'R', 0ULL},{'N', 0ULL}, {'B', 0ULL}, {'Q', 0ULL}, {'K', 0ULL}
    };
    void initializeBoard(std::string fen);
protected:
    inline void setBit(uint64_t& bitboard, int square);
    inline void popBit(uint64_t& bitboard, int square);
    inline int getBit(uint64_t bitboard, int square);
public:
    Board();
    Board(std::string& fenString);
    std::unordered_map<char, uint64_t>& getBitMaps();
    void undo(int num);
    void render();
    void setSquare(char piece, int square);
    char getSquare(int square);
};

#endif
