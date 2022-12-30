#ifndef __BOARD_H__
#define __BOARD_H__

#include <vector>
#include <string>
#include <unordered_map>
#include "subject.h"
#include "move.h"
#include "movegen.h"

class Board : public Subject {
private:
    std::unordered_map<char, uint64_t> pieceMaps; // bb for each of the 12 pieces
    uint64_t occupancyMaps[3]; // bb for white, black, and both sides
    vector<vector<char>> playerPieces = {{'P', 'N', 'B', 'R', 'Q', 'K'}, {'p', 'n', 'b', 'r', 'q', 'k'}};
    vector<Move> moveStack;

    void initializeBoard(std::string fen);
    void computeOccupancyMaps();

    std::vector<uint16_t> generatePawnMoves(int side, std::vector<uint16_t>& moveslist);
    std::vector<uint16_t> generateKnightMoves(int side, std::vector<uint16_t>& moveslist);
    std::vector<uint16_t> generateBishopMoves(int side, std::vector<uint16_t>& moveslist);
    std::vector<uint16_t> generateRookMoves(int side, std::vector<uint16_t>& moveslist);
    std::vector<uint16_t> generateQueenMoves(int side, std::vector<uint16_t>& moveslist);
    std::vector<uint16_t> generateKingMoves(int side, std::vector<uint16_t>& moveslist);
public:
    int ply, fiftyCount;

    Board();
    Board(std::string& fenString);

    // abstracted board state manipulation
    void setSquare(char piece, int square);
    void removeSquare(char piece, int square);
    char getSquare(int square) const;

    // useful getters
    uint64_t getOccupancyBySide(int side) const;
    uint64_t getEmptySquares() const;
    uint64_t getPieceBB(char piece) const;

    int getKingSquare(int side) const;

    // generates a list of moves encoded in 16-bit representation
    std::vector<uint16_t> generateLegalMoves();
    // makes an engine generated move if deemed legal
    void makeMove(uint64_t move);
    void makeMove(string& source, string& target); 
    void makeNullMove();
    void undoMove();
    void undoNullMove();
    // calls observer update function to render current board state
    void render();
};

#endif
