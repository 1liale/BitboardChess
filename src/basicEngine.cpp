#include "basicEngine.h"
#include "board.h"
#include <climits>
#include "util.h"
#include <vector>

using namespace std;

BasicEng::BasicEng(Board *chessBoard, int depth): Engine(chessBoard, depth), 
    weightMap {{'k',9000}, {'q',900}, {'r',500}, {'b',350}, {'n',320}, {'p',100}}, // K, Q, R, B, K, P
    pawnTable {
        0,  0,  0,  0,  0,  0,  0,  0,
        50, 50, 50, 50, 50, 50, 50, 50,
        10, 10, 20, 30, 30, 20, 10, 10,
        5,  5, 10, 27, 27, 10,  5,  5,
        0,  0,  0, 25, 25,  0,  0,  0,
        5, -5,-10,  0,  0,-10, -5,  5,
        5, 10, 10,-25,-25, 10, 10,  5,
        0,  0,  0,  0,  0,  0,  0,  0
    },
    knightTable {
        -50,-40,-30,-30,-30,-30,-40,-50,
        -40,-20,  0,  0,  0,  0,-20,-40,
        -30,  0, 10, 15, 15, 10,  0,-30,
        -30,  5, 15, 20, 20, 15,  5,-30,
        -30,  0, 15, 20, 20, 15,  0,-30,
        -30,  5, 10, 15, 15, 10,  5,-30,
        -40,-20,  0,  5,  5,  0,-20,-40,
        -50,-40,-20,-30,-30,-20,-40,-50,
    },
    bishopTable {
        -20,-10,-10,-10,-10,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5, 10, 10,  5,  0,-10,
        -10,  5,  5, 10, 10,  5,  5,-10,
        -10,  0, 10, 10, 10, 10,  0,-10,
        -10, 10, 10, 10, 10, 10, 10,-10,
        -10,  5,  0,  0,  0,  0,  5,-10,
        -20,-10,-40,-10,-10,-40,-10,-20,
    },
    kingTable {
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -20, -30, -30, -40, -40, -30, -30, -20,
        -10, -20, -20, -20, -20, -20, -20, -10, 
         20,  20,   0,   0,   0,   0,  20,  20,
         20,  30,  10,   0,   0,  10,  30,  20
    } {}

uint16_t BasicEng::getEngineMove() {
    return minimax(depth, true, chessBoard->getSide(), chessBoard->getSide()).first;
}

int BasicEng::evaluateBoard(int maxSide) {
    int whiteCurState = 0;
    int blackCurState = 0;

    for (int square = 0; square < BOARD_SIZE; ++square) {
        char piece = chessBoard->getSquare(square);
        int pieceSide = isupper(piece) != 0 ? WHITE_SIDE : BLACK_SIDE;

        // white side
        if (pieceSide == WHITE_SIDE) {
            whiteCurState += evaluatePiece(piece, square, pieceSide);
        } else {
            blackCurState += evaluatePiece(piece, square, pieceSide);
        }
    }

    return maxSide == WHITE_SIDE ? (whiteCurState - blackCurState) : (blackCurState - whiteCurState);
}

int BasicEng::evaluatePiece(char piece, int square, int side) {
    char pieceName = tolower(piece);

    if (side != WHITE_SIDE) {
        square = 63 - square;
    }

    int pieceScore = weightMap[pieceName];
    switch(pieceName) {
        case 'p':
            pieceScore += pawnTable[square];
            break;
        case 'n':
            pieceScore += knightTable[square];
            break;
        case 'b':
            pieceScore += bishopTable[square];
            break;
        case 'k':
            pieceScore += kingTable[square];
            break;
    }

    return pieceScore;
}

pair<uint16_t, int> BasicEng::minimax(int depth, bool maxPlayer, int maxSide, int side) {
    if (depth < 1) {
        return make_pair(NULL, evaluateBoard(maxSide));
    }

    vector<uint16_t> possibleMoves = chessBoard->generateLegalMoves(side);
    if (possibleMoves.size() == 0) {
        return {NULL, evaluateBoard(maxSide)};
    }
    // randomized ai move from possible move to examine
    time_t t;
    srand(time(&t));
    int randomInd = rand() % possibleMoves.size();
    uint16_t aiMove = possibleMoves[randomInd];

    int currentEval;
    int minMaxEval = maxPlayer ? INT_MIN : INT_MAX;
    
    for (auto & move : possibleMoves) {
        chessBoard->makeMove(move);
        currentEval = minimax(depth - 1, !maxPlayer, maxSide, side ^ 1).second;
        chessBoard->undoMove();
        if (maxPlayer) {
            if (currentEval > minMaxEval) {
                minMaxEval = currentEval;
                aiMove = move;
            }
        } else {
            if (currentEval < minMaxEval) {
                minMaxEval = currentEval;
                aiMove = move;
            }
        }
        
    }
    return make_pair(aiMove, minMaxEval);
}
