#include "seqEngine.h"
#include "board.h"
#include "nnue/nnue.h"
#include <climits>
#include <vector>
#include <algorithm>

using namespace std;
using namespace helpers;

SequentialEng::SequentialEng(Board *chessBoard, int depth): Engine(chessBoard, depth), pvTable{} {
    nnue_init("nn-04cf2b4ed1da.nnue");
}

int SequentialEng::getGamePhaseScore() {
    int gamePhaseScore = 0;

    for (int i = 0; i < PIECES; ++i) {
        char piece = pieces[i];
        BitBoard bb = chessBoard->getPieceBB(piece);
        if (isupper(piece)) {
            gamePhaseScore += countBits(bb) * materialScores[opening][i];
        } else {
            gamePhaseScore += countBits(bb) * -materialScores[opening][i];
        }
    }

    return gamePhaseScore;
}

// evaluates a side's board position using stockfish's nnue weights
int SequentialEng::evaluateBoard(int side) {
    int index = 2;

    int sfPieces[33];
    int sfSquares[33];

    for (int i = 0; i < PIECES; ++i) {
        char piece = pieces[i];
        // copy of the piece's bitboard
        BitBoard bb = chessBoard->getPieceBB(piece);

        // maps to stockfish's pieces and squares convention
        while (bb) {
            int square = getLSBIndex(bb);
            if (piece == 'K') {
                sfPieces[0] = nnuePieces[i];
                sfSquares[0] = nnueSquares[square];
            }
            else if (piece == 'k') {
                sfPieces[1] = nnuePieces[i];
                sfSquares[1] = nnueSquares[square];
            } else {
                sfPieces[index] = nnuePieces[i];
                sfSquares[index] = nnueSquares[square];
                index++;
            }
            popBit(bb, square);
        }
    }

    // zero terminating
    sfPieces[index] = 0;
    sfSquares[index] = 0;

    // use NNUE to evaluate positional score
    int nnueScore = nnue_evaluate(side, sfPieces, sfSquares);

    return nnueScore;
}

// defines custom heuristic to order principal variation moves
int SequentialEng::heuristicOrder(EncMove encMove, int depth, int side) {
    int score = INT_MIN;
    
    Move move{encMove};

    // castling is generally better
    if (move.isCastle()) {
        return score + 1;
    }

    // reward moves that avoid dangling a piece 
    char piece = chessBoard->getSquare(move.getSource());
    int pieceVal = weightMap.at(tolower(piece));
    bool isDanglingSource = chessBoard->isSquareAttacked(side, move.getSource()) &&
        !chessBoard->isSquareAttacked(side ^ 1, move.getSource());
    bool isDanglingTarget = chessBoard->isSquareAttacked(side, move.getTarget()) &&
        !chessBoard->isSquareAttacked(side ^ 1, move.getTarget());

    // non-capture normal moves
    if (move.getMoveType() == QUIET || move.getMoveType() == DOUBLE_MOVE) {
        // dangling piece avoiding danger is good
        if (isDanglingSource) {
            score += pieceVal;
        }
        // move resulting in dangling piece is bad
        if (isDanglingTarget) {
            return score - pieceVal;
        } else {
            return score;
        }
    }

    // captures or promotions (we do not bother to reorder enpassant moves as they rarely happen)
    if (move.isCapture()) {
        char capturedPiece = chessBoard->getSquare(move.getTarget());
        score += weightMap.at(capturedPiece);
    }

    if (move.isPromotion()) {
        char promoPiece = promoOptions[(move.getMoveType() - KNIGHT_PROMOTION) % 4];
        return score + weightMap.at(promoPiece);
    } else {
        if (isDanglingTarget) {
            return score - pieceVal;
        } 
        return score;
    }
}

vector<EncMove> SequentialEng::sortMoves(vector<EncMove> moves, int depth, bool &isFollowPV) {
    int side = chessBoard->getSide();
    int movesLen = moves.size();

    // checks for principal variation
    if (isFollowPV && ((depth + 1) >= (int)pvTable.size() || 
        pvTable[depth] != chessBoard->getLastMove(side))) {
        isFollowPV = false;
    }

    // generates pvMoves using moves with corresponding index
    vector<pair<int, int>> pvMoves(movesLen);
    for (int i = 0; i < movesLen; ++i) {
        if (isFollowPV && moves[i] == pvTable[depth + 1]) {
            pvMoves[i] = (isFollowPV && moves[i] == pvTable[depth + 1]) ?
                make_pair(INT_MAX, i ) : make_pair(heuristicOrder(moves[i], depth, side), i );
        }
    }

    // sorts using heuristic evaluation
    sort(pvMoves.rbegin(), pvMoves.rend());

    // extracts moves and return sort list of moves
    vector<EncMove> sorted(movesLen);
    for (int i = 0; i < movesLen; ++i) {
        sorted[pvMoves[i].second] = moves[i];
    }
    return sorted;
}

// performs Alpha Beta pruning on Minimax Tree
pair<EncMove, int> SequentialEng::alphabeta(int depth, int alpha, int beta,int maxSide, int side) {
    if (depth == 0) return make_pair(NULL, evaluateBoard(maxSide));
    
    vector<EncMove> moves = chessBoard->generateLegalMoves(side);
    // bool isFollowPV = true;
    // vector<EncMove> moves = sortMoves(chessBoard->generateLegalMoves(side), depth, isFollowPV);
   
    if (moves.empty()) {
        if (chessBoard->isKingInCheck(side)) {
            return {NULL, side == maxSide ? INT_MIN : INT_MAX };
        }     
        return {NULL, evaluateBoard(maxSide)};
    };
    
    // randomized ai move from possible move to examine
    time_t t;
    srand(time(&t));
    int randomInd = rand() % moves.size();
    EncMove bestMove = moves[randomInd];

    bool isMaxSide = maxSide == side;
    int eval = isMaxSide ? INT_MIN : INT_MAX;
    
    if (isMaxSide) {
        for (auto & move : moves) {
            chessBoard->makeMove(move);
            eval = max(eval, alphabeta(depth - 1, alpha, beta, maxSide, side ^ 1).second);
            chessBoard->undoMove();
            
            if (eval > alpha) {
                alpha = eval;
                bestMove = move;
            }
            if (eval >= beta) {
                break;
            }
        }
        return {bestMove, eval};
    } else {
        for (auto & move : moves) {
            chessBoard->makeMove(move);
            eval = min(eval, alphabeta(depth - 1, alpha, beta, maxSide, side ^ 1).second);
            chessBoard->undoMove();
            
            if (eval < beta) {
                beta = eval;
                bestMove = move;
            }
            if (eval <= alpha) {
                break;
            }
        }
        return {bestMove, eval};
    }
}

// returns the engine best move
EncMove SequentialEng::getEngineMove() {
    EncMove bestMove;
    int side = chessBoard->getSide();
    // int score = 0;
    int alpha = INT_MIN, beta = INT_MAX;

    // uint64_t startTime = getCurrentTimeInMs();
    // uint64_t stopTime = startTime + 10000; // limit search time to 10 seconds

    // iteratively deepen the search depth until max depth reached
    // for (int itDepth = depth; itDepth <= depth; ++itDepth) {
    //     if (getCurrentTimeInMs() >= stopTime) break; // search period expired
        pair<EncMove, int> result = alphabeta(depth, alpha, beta, side, side);
        // score = result.second;

        // // reinitializes aspiration window if search out of bound
        // if ((score <= alpha) || (score >= beta)) {
        //     alpha = INT_MIN;    
        //     beta = INT_MAX;      
        //     continue;
        // }
        bestMove = result.first;
        
        // decrease aspiration window size
        // alpha = score - 50;
        // beta = score + 50;
    // }
    return bestMove;
}
