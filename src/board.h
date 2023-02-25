#ifndef __BOARD_H__
#define __BOARD_H__

#include <vector>
#include <string>
#include <unordered_map>
#include "subject.h"
#include "move.h"

class Board : public Subject {
    struct MadeMove {
        uint16_t move;
        char sourcePiece, targetPiece;
        int castlingRight;
    };
private:
    int ply, fifty, castlingRight;
    // uint64_t hashKey; // TODO: Implement Zobrist hashing
    std::unordered_map<char, uint64_t> pieceMaps; // bb for each of the 12 pieces
    uint64_t occupancyMaps[3]; // bb for white, black, and both sides
    std::vector<std::vector<char>> playerPieces;
    std::vector<MadeMove> moveHistory;

    // leaper attack maps
    uint64_t pawnAttacks[2][BOARD_SIZE];   // pawn attacks table
    uint64_t knightAttacks[BOARD_SIZE];    // knight attacks table
    uint64_t kingAttacks[BOARD_SIZE];      // king attacks table
    
    // slider attack maps
    // tables in [square][occupancies] format
    uint64_t bishopAttacks[BOARD_SIZE][512];    // bishop attack pattern masks
    uint64_t rookAttacks[BOARD_SIZE][4096];      // rook attack pattern masks

    uint64_t bishopMasks[BOARD_SIZE]; // mask to calculate bishop occupancy
    uint64_t rookMasks[BOARD_SIZE]; // mask to calculate rook occupancy

    // board initialization
    void initializeBoard(std::string fen);
    void computeOccupancyMaps();
    void computeAttackBoards();
    void computeSliderAttacks(uint64_t mask, bool isBishop, int square);

    // move generation
    void generatePawnMoves(int side, std::vector<uint16_t>& moveslist);
    void generateKnightMoves(int side, std::vector<uint16_t>& moveslist);
    void generateBishopMoves(int side, std::vector<uint16_t>& moveslist);
    void generateRookMoves(int side, std::vector<uint16_t>& moveslist);
    void generateQueenMoves(int side, std::vector<uint16_t>& moveslist);
    void generateKingMoves(int side, std::vector<uint16_t>& moveslist);
    void generateSpecialMoves(int side, std::vector<uint16_t>& moveslist);

    // get slider attacks at a square
    uint64_t getBishopAttacks(int square, uint64_t occupancy);
    uint64_t getRookAttacks(int square, uint64_t occupancy);
    uint64_t getQueenAttacks(int square, uint64_t occupancy);
public:
    Board();
    Board(std::string& fenString);

    // board state manipulation
    void setSquare(char piece, int square); // sets a piece on a square
    void removeSquare(char piece, int square); // removes a piece from a 
    char getSquare(int square) const; // gets char of piece at square

    // useful functionalities
    int getSide() const;
    uint64_t getOccupancyBySide(int side) const;
    uint64_t getEmptySquares() const;
    uint64_t getPieceBB(char piece);
    int getKingSquare(int side);
    bool isKingInCheck(int side);
    bool isSquareAttacked(int side, int square); // checks if square is attacked

    // generates a list of pseudo-legal moves
    std::vector<uint16_t> generatePseudoMoves(int side);
    // generates a list of legal moves
    std::vector<uint16_t> generateLegalMoves(int side);
    
    // makes an engine generated move if deemed legal
    int makeMove(uint16_t move);
    // makes move based on user input
    int makeMove(std::string& source, std::string& target, char promote); 
    void undoMove();

    // for search optimatization
    void makeNullMove();
    void undoNullMove();

    // evaluates current game state
    int checkGameState(int side);
    
    // calls observer update function to render current board state
    void render();
};

#endif
