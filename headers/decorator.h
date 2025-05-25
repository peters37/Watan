#ifndef DECORATOR_H
#define DECORATOR_H

#include "board.h"

class Decorator: public Board {
  protected:
    Board *component;
  public:
    Decorator(Board *b);
    virtual ~Decorator();
};

#endif
