#include <iostream>
#include "board.h"
#include "util.h"
#include <cstring>


using namespace std;
using namespace bitutil;
using namespace helpers;


#ifdef DEBUG
    #define DEFAULT_FEN "6k1/PP3ppp/p7/P7/8/7P/1p3PPP/3R2K1"
#else
    #define DEFAULT_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"
#endif

///////////////////////  INITIALIZATION  /////////////////////// 

void Board::initializeBoard(string fen) {
    ply = 0;
    fifty = 0;
    castlingRight = 15;
    pieceMaps = {
        {'p', 0ULL}, {'r', 0ULL},{'n', 0ULL}, {'b', 0ULL}, {'q', 0ULL}, {'k', 0ULL}, 
        {'P', 0ULL}, {'R', 0ULL},{'N', 0ULL}, {'B', 0ULL}, {'Q', 0ULL}, {'K', 0ULL}
    };
    playerPieces = {{'P', 'N', 'B', 'R', 'Q', 'K'}, {'p', 'n', 'b', 'r', 'q', 'k'}};
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
            setSquare(c, row * BOARD_WIDTH + col);
            ++col;
        }
    }
    computeOccupancyMaps();
    computeAttackBoards();
}

void Board::computeOccupancyMaps() {
    memset(occupancyMaps, 0ULL, sizeof(occupancyMaps));
    for (auto& bb : pieceMaps) {
        int side = isupper(bb.first) ? 0 : 1;
        occupancyMaps[side] |= pieceMaps[bb.first];
    }
    occupancyMaps[BOTH_SIDE] = occupancyMaps[WHITE_SIDE] | occupancyMaps[BLACK_SIDE];
}

void Board::computeAttackBoards() {
    // sets up attack boards for leaping pieces (PAWN, KNIGHT, KING)
    for (int square = 0; square < BOARD_SIZE; ++square) {
        pawnAttacks[WHITE_SIDE][square] = maskPawnAttacks(WHITE_SIDE, square);
        pawnAttacks[BLACK_SIDE][square] = maskPawnAttacks(BLACK_SIDE, square);

        knightAttacks[square] = maskKnightAttacks(square);
        kingAttacks[square] = maskKingAttacks(square);

        bishopMasks[square] = maskBishopAttacks(square);
        rookMasks[square] = maskRookAttacks(square);

        computeSliderAttacks(bishopMasks[square], true, square);
        computeSliderAttacks(rookMasks[square], false, square);
    }
}

void Board::computeSliderAttacks(uint64_t mask, bool isBishop, int square) {
    int relevantBitsCount = countBits(mask);
    int occupancyIndices = (1 << relevantBitsCount);
    
    for (int i = 0; i < occupancyIndices; i++) {
        uint64_t occupancy = setOccupancy(i, relevantBitsCount, mask);
        if (isBishop) {
            int magicInd = (occupancy * bishopMagics[square]) >> (64 - bishopIndexBits[square]);
            bishopAttacks[square][magicInd] = maskBishopAttacksWithBlocks(square, occupancy);
        } else {
            int magicInd = (occupancy * rookMagics[square]) >> (64 - rookIndexBits[square]);
            rookAttacks[square][magicInd] = maskRookAttacksWithBlocks(square, occupancy);
        }
    }
}

Board::Board() { initializeBoard(DEFAULT_FEN); }
Board::Board(string& customFen) { initializeBoard(customFen); }

///////////////////  ESSENTIAL BOARD UTILS  //////////////////// 

// board state manipulation
void Board::setSquare(char piece, int square) {
    setBit(pieceMaps[piece], square);
}
void Board::removeSquare(char piece, int square) {
    popBit(pieceMaps[piece], square);
}
char Board::getSquare(int square) const {
    for (const auto& bb : pieceMaps) {
        if (getBit(bb.second, square)) {
            return bb.first;
        }
    }
    return '.';
}

// useful functionalities
int Board::getSide() const {
    return ply % 2;
} 
uint64_t Board::getOccupancyBySide(int side) const {
    return occupancyMaps[side];
}
uint64_t Board::getEmptySquares() const {
    return occupancyMaps[BOTH_SIDE] ^ (~0ULL);
}
uint64_t Board::getPieceBB(char piece) {
    return pieceMaps[piece];
}
int Board::getKingSquare(int side) {
    char king = side == WHITE_SIDE ? 'K' : 'k';
    return getLSBIndex(getPieceBB(king));
}
bool Board::isKingInCheck(int side) {
    int kingSquare = getKingSquare(side);
    return isSquareAttacked(side, kingSquare);
}
bool Board::isSquareAttacked(int side, int square) {
    uint64_t pawnBB = (side == WHITE_SIDE ? pieceMaps['p'] : pieceMaps['P']);
    uint64_t knightBB = (side == WHITE_SIDE ? pieceMaps['n'] : pieceMaps['N']);
    uint64_t kingBB = (side == WHITE_SIDE ? pieceMaps['k'] : pieceMaps['K']);
    uint64_t bishopBB = (side == WHITE_SIDE ? pieceMaps['b'] : pieceMaps['B']);
    uint64_t rookBB = (side == WHITE_SIDE ? pieceMaps['r'] : pieceMaps['R']);
    uint64_t queenBB = (side == WHITE_SIDE ? pieceMaps['q'] : pieceMaps['Q']);

    // check if attacked by pawn
    if (pawnAttacks[side ^ 1][square] & pawnBB) return true;
    // check if attacked by knight
    if (knightAttacks[square] & knightBB) return true;
    // check if attacked by king
    if (kingAttacks[square] & kingBB) return true;
    // check if attacked by bishop
    if (getBishopAttacks(square, occupancyMaps[BOTH_SIDE]) & bishopBB) return true;
    // check if attacked by rook
    if (getRookAttacks(square, occupancyMaps[BOTH_SIDE]) & rookBB) return true;
    // check if attacked by queen
    if (getQueenAttacks(square, occupancyMaps[BOTH_SIDE]) & queenBB) return true;
    
    return false;
}

uint64_t Board::getBishopAttacks(int square, uint64_t occupancy) {
    occupancy &= bishopMasks[square];
    occupancy *= bishopMagics[square];
    occupancy >>= 64 - bishopIndexBits[square];

    return bishopAttacks[square][occupancy];
}
uint64_t Board::getRookAttacks(int square, uint64_t occupancy) {
    occupancy &= rookMasks[square];
    occupancy *= rookMagics[square];
    occupancy >>= 64 - rookIndexBits[square];
    
    return rookAttacks[square][occupancy];
}
uint64_t Board::getQueenAttacks(int square, uint64_t occupancy) {
    uint64_t attacks = 0ULL;
    
    // merge bishop and rook attacks to get queen attacks
    attacks = getBishopAttacks(square, occupancy);
    attacks |= getRookAttacks(square, occupancy);
    
    // return queen attacks
    return attacks;
}

///////////////////////  MOVE GENERATION  /////////////////////// 

void Board::generatePawnMoves(int side, vector<uint16_t>& moveslist) {
    int source, target;
    char piece = side == WHITE_SIDE ? 'P' : 'p';
    uint64_t bitboard = pieceMaps[piece], attacks;

    // checks each pawn square
    while (bitboard) {
        source = getLSBIndex(bitboard);

        // set target to square in front 
        target = source + 8 * (side == WHITE_SIDE ? -1 : 1);

        // generate pawn quiet moves
        if (target >= a8 && target <= h1 && !getBit(occupancyMaps[BOTH_SIDE], target)) {
            bool isWhitePromo = side == WHITE_SIDE && source >= a7 && source <= h7;
            bool isBlackPromo = side == BLACK_SIDE && source >= a2 && source <= h2;
            // promotion
            if (isWhitePromo || isBlackPromo) {
                Move promoteToQueen{source, target, QUEEN_PROMOTION};
                Move promoteToRook{source, target, ROOK_PROMOTION};
                Move promoteToKnight{source, target, KNIGHT_PROMOTION};
                Move promoteToBishop{source, target, BISHOP_PROMOTION};

                moveslist.push_back(promoteToQueen.move);
                moveslist.push_back(promoteToRook.move);
                moveslist.push_back(promoteToKnight.move);
                moveslist.push_back(promoteToBishop.move);
            } 
            // move or double move
            else {
                Move singleMovePawn{source, target, QUIET};
                moveslist.push_back(singleMovePawn.move);
                // double move
                bool isWhiteDouble = side == WHITE_SIDE && source >= a2 && source <= h2;
                bool isBlackDouble = side == BLACK_SIDE && source >= a7 && source <= h7;
                int nextTarget = target + BOARD_WIDTH * (side == WHITE_SIDE ? -1 : 1);
                if ( (isWhiteDouble || isBlackDouble) && !getBit(occupancyMaps[BOTH_SIDE], nextTarget)) {
                    Move doubleMovePawn{source, nextTarget, DOUBLE_MOVE};
                    moveslist.push_back(doubleMovePawn.move);
                }
            }
        } 

        // generate pawn capture moves
        attacks = pawnAttacks[side][source] & occupancyMaps[side ^ 1];

        while (attacks) {
            // init target square
            target = getLSBIndex(attacks);
            
            bool canPromoteWhite = side == WHITE_SIDE && source >= a7 && source <= h7;
            bool canPromoteBlack = side == BLACK_SIDE && source >= a2 && source <= h2;
            // promotion after capture
            if (canPromoteWhite || canPromoteBlack) {
                Move promoteToQueen{source, target, QUEEN_PROMOTION_CAPTURE};
                Move promoteToRook{source, target, ROOK_PROMOTION_CAPTURE};
                Move promoteToKnight{source, target, KNIGHT_PROMOTION_CAPTURE};
                Move promoteToBishop{source, target, BISHOP_PROMOTION_CAPTURE};

                moveslist.push_back(promoteToQueen.move);
                moveslist.push_back(promoteToRook.move);
                moveslist.push_back(promoteToKnight.move);
                moveslist.push_back(promoteToBishop.move);
            } else {
                Move pawnCapture{source, target, CAPTURE};
                moveslist.push_back(pawnCapture.move);
            }

            popBit(attacks, target);
        }
        popBit(bitboard, source);
    }
}

void Board::generateKnightMoves(int side, vector<uint16_t>& moveslist) {
    int source, target;
    char piece = side == WHITE_SIDE ? 'N' : 'n';
    uint64_t bitboard = pieceMaps[piece], attacks;
    Move knightMove;

    // checks each knight square
    while (bitboard) {
        source = getLSBIndex(bitboard);

        // generate knight moves
        attacks = knightAttacks[source] & ~occupancyMaps[side];

        while (attacks) {
            target = getLSBIndex(attacks);
            char targetPiece = getSquare(target);
            // quiet move
            if (targetPiece == '.') {
                knightMove = Move{source, target, QUIET};
            } 
            // capture move
            else {
                knightMove = Move{source, target, CAPTURE};
            }
            moveslist.push_back(knightMove.move);
            popBit(attacks, target);
        }
        popBit(bitboard, source);
    }
}

void Board::generateKingMoves(int side, vector<uint16_t>& moveslist) {
    int source, target;
    char piece = side == WHITE_SIDE ? 'K' : 'k';
    uint64_t bitboard = pieceMaps[piece], attacks;
    Move kingMove;

    while(bitboard) {
        source = getLSBIndex(bitboard);
        attacks = kingAttacks[source] & ~occupancyMaps[side];

        while (attacks) {
            target = getLSBIndex(attacks);
            // quiet
            if (!getBit(occupancyMaps[side ^ 1], target)) {
                kingMove = Move{source, target, QUIET};
            } 
            // capture
            else {
                kingMove = Move{source, target, CAPTURE};
            }
            moveslist.push_back(kingMove.move);
            popBit(attacks, target);
        }

        popBit(bitboard, source);
    }
}

void Board::generateBishopMoves(int side, vector<uint16_t>& moveslist) {
    int source, target;
    char piece = side == WHITE_SIDE ? 'B' : 'b';
    uint64_t bitboard = pieceMaps[piece], attacks;
    Move bishopMove;

    while (bitboard) {
        source = getLSBIndex(bitboard);
        attacks = getBishopAttacks(source, occupancyMaps[BOTH_SIDE]) & ~occupancyMaps[side];

        while (attacks) {
            target = getLSBIndex(attacks);
            // quiet
            if (!getBit(occupancyMaps[side ^ 1], target)) {
                bishopMove = Move{source, target, QUIET};
            } 
            // capture
            else {
                bishopMove = Move{source, target, CAPTURE};
            }
            moveslist.push_back(bishopMove.move);
            popBit(attacks, target);
        }
        popBit(bitboard, source);
    }
}

void Board::generateRookMoves(int side, vector<uint16_t>& moveslist) {
    int source, target;
    char piece = side == WHITE_SIDE ? 'R' : 'r';
    uint64_t bitboard = pieceMaps[piece], attacks;
    Move rookMove;

    while (bitboard) {
        source = getLSBIndex(bitboard);
        attacks = getRookAttacks(source, occupancyMaps[BOTH_SIDE]) & ~occupancyMaps[side];

        while (attacks) {
            target = getLSBIndex(attacks);
            // quiet
            if (!getBit(occupancyMaps[side ^ 1], target)) {
                rookMove = Move{source, target, QUIET};
            } 
            // capture
            else {
                rookMove = Move{source, target, CAPTURE};
            }
            moveslist.push_back(rookMove.move);
            popBit(attacks, target);
        }
        popBit(bitboard, source);
    }
}

void Board::generateQueenMoves(int side, vector<uint16_t>& moveslist) {
    int source, target;
    char piece = side == WHITE_SIDE ? 'Q' : 'q';
    uint64_t bitboard = pieceMaps[piece], attacks;
    Move queenMove;

     while (bitboard) {
        source = getLSBIndex(bitboard);
        attacks = getQueenAttacks(source, occupancyMaps[BOTH_SIDE]) & ~occupancyMaps[side];

        while (attacks) {
            target = getLSBIndex(attacks);
            // quiet
            if (!getBit(occupancyMaps[side ^ 1], target)) {
                queenMove = Move{source, target, QUIET};
            } 
            // capture
            else {
                queenMove = Move{source, target, CAPTURE};
            }
            moveslist.push_back(queenMove.move);
            popBit(attacks, target);
        }
        popBit(bitboard, source);
    }
}

void Board::generateSpecialMoves(int side, vector<uint16_t>& moveslist) {
    Move specialMove;

    // generates en-passant
    if (!moveHistory.empty()) {
        MadeMove &lastMove = moveHistory.back();
        Move move{lastMove.move};

        int target = move.getTarget();
        int source = move.getSource();

        char pawn = side == WHITE_SIDE ? 'P' : 'p';
        char oppPawn = side == WHITE_SIDE ? 'p' : 'P';
        if (lastMove.sourcePiece == oppPawn && move.getMoveType() == DOUBLE_MOVE) {
            int nextTarget = source + BOARD_WIDTH * (side == WHITE_SIDE ? 1 : -1);
            
            if (target != a5 && target != a4 && getBit(pieceMaps[pawn], target - 1)) {
                specialMove = Move{target - 1, nextTarget, EN_PASSANT};
                moveslist.push_back(specialMove.move);
            } 
            if (target != h5 && target != h4 && getBit(pieceMaps[pawn], target + 1)) {
                specialMove = Move{target + 1, nextTarget, EN_PASSANT};
                moveslist.push_back(specialMove.move);
            }
        }
    }
    
    int kingSquare = getKingSquare(side);
    #ifdef DEBUG
        // cout << "king square: " << positions[kingSquare] << endl;
        // cout << bitset<4>(castlingRight) << endl;
        // cout << bitset<4>(castlingSideMask[side][0]) << endl;
    #endif 
    // handles king-side castle
    if (castlingRight & castlingSideMask[side][0]) {
        // checks that no obstacle on castling path
        if (!(getBit(occupancyMaps[BOTH_SIDE], kingSquare + 1) || 
              getBit(occupancyMaps[BOTH_SIDE], kingSquare + 2))) {
            if (!(isSquareAttacked(side, kingSquare + 1) ||
                  isSquareAttacked(side, kingSquare + 2))) {
                specialMove = Move{kingSquare, kingSquare + 2, K_CASTLE};
                moveslist.push_back(specialMove.move);
            }
        }
    }
    // checks right-castle
    if (castlingRight & castlingSideMask[side][1]) {
        // checks that no obstacle on castling path
        if (!(getBit(occupancyMaps[BOTH_SIDE], kingSquare - 1) || 
              getBit(occupancyMaps[BOTH_SIDE], kingSquare - 2) ||
              getBit(occupancyMaps[BOTH_SIDE], kingSquare - 3))) {
            if (!(isSquareAttacked(side, kingSquare - 1) ||
                  isSquareAttacked(side, kingSquare - 2))) {
                specialMove = Move{kingSquare, kingSquare - 2, Q_CASTLE};
                moveslist.push_back(specialMove.move);
            }
        }
    }
}

vector<uint16_t> Board::generatePseudoMoves(int side) {
    vector<uint16_t> moveslist;

    generatePawnMoves(side, moveslist);
    generateKnightMoves(side, moveslist);
    generateKingMoves(side, moveslist);
    generateBishopMoves(side, moveslist);
    generateRookMoves(side, moveslist);
    generateQueenMoves(side, moveslist);
    generateSpecialMoves(side, moveslist);

    return moveslist;
}

vector<uint16_t> Board::generateLegalMoves(int side) {
    vector<uint16_t> moveslist = generatePseudoMoves(side), legalMoves;
    for (auto move : moveslist) {
        int moveFlag = makeMove(move);
        if (moveFlag == ILLEGAL_MOVE) continue; // skip illegal move
        legalMoves.push_back(move);
        undoMove();
    }
    return legalMoves;
}

///////////////////////  MAKE AND UNDO MOVE /////////////////////// 

// handles pseudo-legal move from human and computer
int Board::makeMove(uint16_t pseudoMove) {
    int side = getSide();
    ++ply;
    ++fifty;

    Move move{pseudoMove};
    int source = move.getSource();
    int target = move.getTarget();
    MoveType moveType = move.getMoveType();
    char sourcePiece = getSquare(source);
    char targetPiece = getSquare(target);
    
    // moves sourcePiece to designated square
    removeSquare(sourcePiece, source);
    setSquare(sourcePiece, target);

    if (tolower(sourcePiece) == 'p') fifty = 0;
    // removes the en-passented pawn
    if (moveType == EN_PASSANT) {
        char oppPawn = side == WHITE_SIDE ? 'p' : 'P';
        int oppPawnSquare = target + BOARD_WIDTH * (1 - 2 * side);
        targetPiece = oppPawn;
        removeSquare(targetPiece, oppPawnSquare);
    } 
    // moves the king side rook for castling
    else if (moveType == K_CASTLE) {
        char castlingRook = side == WHITE_SIDE ? 'R' : 'r';
        targetPiece = castlingRook;
        removeSquare(castlingRook, target + 1);
        setSquare(castlingRook, target - 1);
    } 
    // moves the queen side rook for castling
    else if (moveType == Q_CASTLE) {
        char castlingRook = side == WHITE_SIDE ? 'R' : 'r'; 
        targetPiece = castlingRook;
        removeSquare(castlingRook, target - 2);
        setSquare(castlingRook, target + 1);
    } 
    // removes captured piece
    else if (moveType == CAPTURE) {
        fifty = 0;
        if (targetPiece != '.')  removeSquare(targetPiece, target);
    } 
    // replaces pawn by promoted piece
    else if (moveType >= KNIGHT_PROMOTION) {
        removeSquare(sourcePiece, target);

        if (moveType >= KNIGHT_PROMOTION_CAPTURE) {
            if (targetPiece != '.') removeSquare(targetPiece, target);
        }
        // sets sourcePiece to the intended promotion piece
        switch((moveType - 8) % 4) {
            case 0:
                sourcePiece = side == WHITE_SIDE ? 'N' : 'n';
                break;
            case 1:
                sourcePiece = side == WHITE_SIDE ? 'B' : 'b';
                break;
            case 2:
                sourcePiece = side == WHITE_SIDE ? 'R' : 'r';
                break;
            case 3:
                sourcePiece = side == WHITE_SIDE ? 'Q' : 'q';
                break;
        }
        setSquare(sourcePiece, target);
    }

    // stores move history
    moveHistory.push_back({ pseudoMove, sourcePiece, targetPiece, castlingRight });
    
    #ifdef DEBUG
    // cout << "updating castlingRight:" << bitset<4>(castlingRight) << endl;
    // cout << positions[source] << ": " << bitset<4>(castlingRightsTable[source]) << endl;
    // cout << positions[target] << ": " << bitset<4>(castlingRightsTable[target]) << endl;
    #endif
    castlingRight &= castlingRightsTable[source];
    castlingRight &= castlingRightsTable[target];

    // re-computes occupancies
    computeOccupancyMaps();

    // returns illegal move if king is left in check
    if(isKingInCheck(side)) {
        undoMove();
        return ILLEGAL_MOVE;
    }
    return LEGAL_MOVE;
}

// handles human player move
int Board::makeMove(string& sourceStr, string& targetStr, char promote = 'x') {
    int side = getSide();
    vector<uint16_t> moveslist = generateLegalMoves(side);
    // vector<uint16_t> moveslist = generatePseudoMoves(side);
#ifdef DEBUG
    cout << "legal moves: " << moveslist.size() << endl;
#endif
    int source = getSquareFromStr(sourceStr);
    int target = getSquareFromStr(targetStr);

    // validate move
    for (auto move : moveslist) {
        Move legalMove{move};
        int legalSource = legalMove.getSource();
        int legalTarget = legalMove.getTarget();

        if (legalSource == source && legalTarget == target) {
            // handles promotion
            MoveType moveType = legalMove.getMoveType();
            if(moveType >= KNIGHT_PROMOTION) {
                bool validPromotion = false;
                // promotion to knight
                if (tolower(promote) == 'n' && (moveType == KNIGHT_PROMOTION || moveType == KNIGHT_PROMOTION_CAPTURE)) {
                    validPromotion = true;
                }
                // promotion to bishop
                else if (tolower(promote) == 'b' && (moveType == BISHOP_PROMOTION || moveType == BISHOP_PROMOTION_CAPTURE)) {
                    validPromotion = true;
                }
                // promotion to rook
                else if (tolower(promote) == 'r' && (moveType == ROOK_PROMOTION || moveType == ROOK_PROMOTION_CAPTURE)) {
                    validPromotion = true;
                }
                // promotion to queen by default
                else if ((tolower(promote) == 'q' || promote == 'x') && (moveType == QUEEN_PROMOTION || moveType == QUEEN_PROMOTION_CAPTURE)) {
                    validPromotion = true;
                }
                
                // makes promotions if valid
                if (validPromotion) { 
                    #ifdef DEBUG
                        cout << "promote: " << promote << " " << legalMove << endl;
                    #endif
                    return makeMove(legalMove.move);
                } 
            } else {
                if (promote != 'x') break;
                return makeMove(legalMove.move); 
            }
        } 
    }
    
    // otherwise return illegal move flag
    return ILLEGAL_MOVE;
}
// takes back a previous move
void Board::undoMove() {
    if (ply == 0 || moveHistory.empty()) {
        throw runtime_error("Make a move first to undo move!");
    } // no moves to undo
    int side = getSide();
    --ply;
    MadeMove madeMove = moveHistory.back();
    moveHistory.pop_back();
    Move move {madeMove.move};
    int source = move.getSource();
    int target = move.getTarget();
    MoveType moveType = move.getMoveType(); 
    char sourcePiece = madeMove.sourcePiece;
    char targetPiece = madeMove.targetPiece;
    castlingRight = madeMove.castlingRight;
    
    removeSquare(sourcePiece, target);
    setSquare(sourcePiece, source);

    // restores en-passanted piece
    if (moveType == EN_PASSANT) {
        int enpassantSquare = target - BOARD_WIDTH * (1 - 2 * side);
        setSquare(targetPiece, enpassantSquare);
    } else if (moveType == K_CASTLE) {
        removeSquare(targetPiece, target - 1);
        setSquare(targetPiece, target + 1);
    } else if (moveType == Q_CASTLE) {
        removeSquare(targetPiece, target + 1);
        setSquare(targetPiece, target - 2);
    }
    // restores captured piece
    else if (moveType == CAPTURE) {
        setBit(pieceMaps[targetPiece], target);
    } 
    // retores promoted pawn
    else if (moveType >= KNIGHT_PROMOTION) {
        removeSquare(sourcePiece, source);
        setSquare(side == WHITE_SIDE ? 'p' : 'P', source);
        if (moveType >= KNIGHT_PROMOTION_CAPTURE) {
            setSquare(targetPiece, target);
        }
        #ifdef DEBUG
            cout << "source: " << positions[source] << endl;
            cout << "target: " << positions[target] << endl;
            cout << sourcePiece << ", " << targetPiece << endl;
            cout << "undo move: " << move << endl;
        #endif
        removeSquare(sourcePiece, target);
    }

    // recomputes occupancy maps
    computeOccupancyMaps();
}

// TODO: IMPLEMENT FOR NEGAMAX OPTIMIZATION (LOW PRIORITY)
void Board::makeNullMove() {}
void Board::undoNullMove() {}

// evalutates current game state
int Board::checkGameState(int side) {
    #ifdef DEBUG
    cout << "checking game state" << endl;
    #endif
    if (fifty == 100) return DRAW;
    vector<uint16_t> legalMoves = generateLegalMoves(side);
    #ifdef DEBUG
    // cout << "moves available: " << legalMoves.size() << endl;
    // for (auto & move: legalMoves) {
    //     cout << Move{move} << endl;
    // }    
    #endif
    if (legalMoves.empty()) {
        if (isKingInCheck(side)) return GAME_OVER;
        else return DRAW;
    } 
    
    if (isKingInCheck(side)) {
        cout << "Check!" << endl;
    }
    return LEGAL_MOVE;
}

///////////////////////  OBSERVER UPDATE /////////////////////// 

void Board::render() {
    updateObservers();
}
