#ifndef __TEXTOBSERVER_H__
#define __TEXTOBSERVER_H__
#include <iostream>
#include <unordered_map>
#include <string>
#include "observer.h"

class Board;

class TextObserver : public Observer {
	Board *chessBoard;
	std::ostream &out = std::cout;
	std::unordered_map<char, std::string> unicodePieces = {
		{'p', "♙"}, {'n',"♘"}, {'b',"♗"}, {'r',"♖"}, {'q',"♕"}, {'k',"♔"}, 
		{'P',"♟︎"}, {'N',"♞"}, {'B',"♝"}, {'R',"♜"}, {'Q',"♛"}, {'K',"♚"}};
public:
	TextObserver(Board *chessBoard);
	~TextObserver();
	virtual void update() override;
};

#endif