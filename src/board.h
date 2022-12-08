#ifndef __BOARD_H__
#define __BOARD_H__

#include <string>
#include <unordered_map>
#include "subject.h"

class Board : public Subject {
private:
    std::unordered_map<char, uint64_t> pieceMaps; // bb for each of the 12 pieces
    uint64_t occupancyMaps[3]; // bb for white, black, and both sides
    
    void initializeBoard(std::string fen);
    void computeOccupancyMaps();
public:
    int turn, fiftyCount;

    Board();
    Board(std::string& fenString);

    // abstracted board state manipulation
    void setSquare(char piece, int square);
    void removeSquare(char piece, int square);
    char getSquare(int square) const;

    // useful getters
    uint64_t getOccupancyBySide() const;
    uint64_t getEmptySquares() const;
    uint64_t getPieceBB(char piece) const;

    int getKingSquare(int side) const;

    // generates a list of moves encoded in 16-bit representation
    std::vector<uint16_t> generateLegalMoves();
    // makes an engine generated move if deemed legal
    void makeMove(Move move);
    void makeMove(string& source, string& target); 
    void makeNullMove();
    void undoMove();
    void undoNullMove();
    // calls observer update function to render current board state
    void render();
};

#endif
