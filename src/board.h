#ifndef __BOARD_H__
#define __BOARD_H__

#include <iostream>
#include <string>
#include <vector> 

class Board {
    std::vector<uint64_t> bitmaps;

    void setBit(uint64_t b, int i);
    void getBit(uint64_t b, int i);
    void clearBit(uint64_t b, int i);
    uint64_t
public:
    Board();
    Board(std::string& fenString);
    void undo(int num);
    void print();
};

#endif
