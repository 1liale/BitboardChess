#include "board.h"

using namespace std;

Board::Board() {}
Board::Board(string& fenString) {}
void Board::undo(int num) {
    cout << "undo " << num << " moves" << endl;
}
void Board::print() {
    cout << "print" << endl;
}