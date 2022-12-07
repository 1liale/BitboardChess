#ifndef __BOARD_H__
#define __BOARD_H__

#include <string>
#include <unordered_map>
#include "subject.h"

class Board : public Subject {
private:
    std::unordered_map<char, uint64_t> pieceMaps;
    uint64_t occupancyMaps[3];

    void initializeBoard(std::string fen);
public:
    Board();
    Board(std::string& fenString);

    // abstracted board state manipulation
    void setSquare(char piece, int square);
    void removeSquare(char piece, int square);
    char getSquare(int square);

    // generates a list of moves encoded in 16-bit representation
    std::vector<uint16_t> generateLegalMoves();
    // makes an engine generated move if deemed legal
    void makeMove(uint16_t move);
    // makes a player generated move if deemed legal
    void makeMove(std::string& from, std::string& to);
    // undo num many moves
    void undoMove(int num);
    // calls observer update function to render current board state
    void render();
};

#endif
