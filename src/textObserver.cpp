#include "board.h"
#include "observer.h"
#include "textObserver.h"

using namespace std;

TextObserver::TextObserver(Board *chessBoard): b{chessBoard} {
  b->attach( this );
}

TextObserver::~TextObserver() {
  b->detach( this );
}

void TextObserver::update() {
  for (int row = 0; row < 8; ++row) {
    out << 8 - row << " ";
    for (int col = 0; col < 8; ++col) {
      Pos p{row, col};
      Piece *piece = b->getPiece(p);
      if (piece != nullptr) {
        char name = piece->getPiece();
        if (piece->getOwner() == 'w') {
          name = toupper(name);
        }
        out << name;
      }
      else {
        if ((row + col) % 2 == 0) {
          out << ' ';
        }
        else {
          out << '-';
        }
      }
    }
    out << endl;
  }
  out << "  " << "abcdefgh" << endl << endl;
}