#include "board.h"
#include "textObserver.h"

using namespace std;

TextObserver::TextObserver(Board *chessBoard): chessBoard{chessBoard} {
  chessBoard->attach( this );
}

TextObserver::~TextObserver() {
  chessBoard->detach( this );
}

void TextObserver::update() {
  out << endl;
  for (int row = 0; row < 8; ++row) {
    for (int col = 0; col < 8; ++col) {
      int square = row * 8 + col;
      if (col == 0) out << 8 - row << " "; // prints row num
      char piece = chessBoard->getSquare(square);
      out << (piece == '.' ? "." : unicodePieces[piece]) << " ";  
    }
    out << endl;
  }
  out << "  " << "a b c d e f g h" << endl << endl;
}