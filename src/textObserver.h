#ifndef __TEXTOBSERVER_H__
#define __TEXTOBSERVER_H__
#include <iostream>
#include "observer.h"

class Board;

class TextObserver : public Observer {
  Board *chessBoard;
  std::ostream &out = std::cout;

 public:
  TextObserver(ChessBoard *chessbOARD);
  ~TextObserver();
  virtual void update() override;
};

#endif