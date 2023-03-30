#ifndef __BOARD_H__
#define __BOARD_H__

#include <vector>
#include <string>
#include <unordered_map>
#include "subject.h"
#include "move.h"

class Board : public Subject {
    struct MadeMove {
        EncMove move;
        char sourcePiece, targetPiece;
        int castlingRight;
    };
private:
    int ply, fifty, castlingRight;
    // BitBoard hashKey; // TODO: Implement Zobrist hashing
    std::unordered_map<char, BitBoard> pieceMaps; // bb for each of the 12 pieces
    BitBoard occupancyMaps[3]; // bb for white, black, and both sides
    std::vector<std::vector<char>> playerPieces;
    std::vector<MadeMove> moveHistory;

    // leaper attack maps
    BitBoard pawnAttacks[2][BOARD_SIZE];   // pawn attacks table
    BitBoard knightAttacks[BOARD_SIZE];    // knight attacks table
    BitBoard kingAttacks[BOARD_SIZE];      // king attacks table
    
    // slider attack maps
    // tables in [square][occupancies] format
    BitBoard bishopAttacks[BOARD_SIZE][512];    // bishop attack pattern masks
    BitBoard rookAttacks[BOARD_SIZE][4096];      // rook attack pattern masks

    BitBoard bishopMasks[BOARD_SIZE]; // mask to calculate bishop occupancy
    BitBoard rookMasks[BOARD_SIZE]; // mask to calculate rook occupancy

    // board initialization
    void initializeBoard(std::string fen);
    void computeOccupancyMaps();
    void computeAttackBoards();
    void computeSliderAttacks(BitBoard mask, bool isBishop, int square);

    // move generation
    void generatePawnMoves(int side, std::vector<EncMove>& moveslist);
    void generateKnightMoves(int side, std::vector<EncMove>& moveslist);
    void generateBishopMoves(int side, std::vector<EncMove>& moveslist);
    void generateRookMoves(int side, std::vector<EncMove>& moveslist);
    void generateQueenMoves(int side, std::vector<EncMove>& moveslist);
    void generateKingMoves(int side, std::vector<EncMove>& moveslist);
    void generateSpecialMoves(int side, std::vector<EncMove>& moveslist);

    // get slider attacks at a square
    BitBoard getBishopAttacks(int square, BitBoard occupancy);
    BitBoard getRookAttacks(int square, BitBoard occupancy);
    BitBoard getQueenAttacks(int square, BitBoard occupancy);
public:
    Board();
    Board(std::string& fenString);

    // board state manipulation
    void setSquare(char piece, int square); // sets a piece on a square
    void removeSquare(char piece, int square); // removes a piece from a 
    char getSquare(int square) const; // gets char of piece at square

    // useful functionalities
    int getSide() const;
    BitBoard getOccupancyBySide(int side) const;
    BitBoard getEmptySquares() const;
    BitBoard getPieceBB(char piece);
    int getKingSquare(int side);
    bool isKingInCheck(int side);
    bool isSquareAttacked(int side, int square); // checks if square is attacked
    EncMove getLastMove(int side) const; // gets the last move made

    // generates a list of pseudo-legal moves
    std::vector<EncMove> generatePseudoMoves(int side);
    // generates a list of legal moves
    std::vector<EncMove> generateLegalMoves(int side);
    
    // makes an engine generated move if deemed legal
    int makeMove(EncMove move);
    // makes move based on user input
    int makeMove(std::string& source, std::string& target, char promote); 
    void undoMove();

    // evaluates current game state
    int checkGameState(int side);
    
    // calls observer update function to render current board state
    void render();
};

#endif
