#ifndef GEESEDECORATOR_H
#define GEESEDECORATOR_H

#include "decorator.h"

class GeeseDecorator: public Decorator {
    int geese_tile;
    bool getGeese(int tile) const override;
    string getAssessment(int criteria) const;
    string getGoal(int goal) const;
  public:
    GeeseDecorator(Board *b, int tile);
};

#endif
