#ifndef __BOARD_H__
#define __BOARD_H__

#include <iostream>
#include <string>
#include <vector> 

class Board {
    std::vector<uint64_t> bitmaps;
public:
    Board();
    Board(std::string& fenString);
    void undo(int num);
    void print();
};

#endif
