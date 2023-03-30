#include "basicEngine.h"
#include "board.h"
#include <climits>
#include <vector>
#include "nnue/nnue.h"

using namespace std;

BasicEng::BasicEng(Board *chessBoard, int depth): Engine(chessBoard, depth) {
    nnue_init("nn-04cf2b4ed1da.nnue");
}

EncMove BasicEng::getEngineMove() {
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
    if (pieceName == '.') return 0;

    if (side != WHITE_SIDE) {
        square = 63 - square;
    }

    int pieceScore = weightMap.at(pieceName);

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

pair<EncMove, int> BasicEng::minimax(int depth, bool maxPlayer, int maxSide, int side) {
    if (depth < 1) {
        return make_pair(NULL, evaluateBoard(maxSide));
    }

    vector<EncMove> possibleMoves = chessBoard->generateLegalMoves(side);
    if (possibleMoves.empty()) return {NULL, evaluateBoard(maxSide)};
    
    // randomized ai move from possible move to examine
    time_t t;
    srand(time(&t));
    int randomInd = rand() % possibleMoves.size();
    EncMove aiMove = possibleMoves[randomInd];

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
