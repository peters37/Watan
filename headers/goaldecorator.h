#ifndef GOALDECORATOR_H
#define GOALDECORATOR_H

#include "decorator.h"

class GoalDecorator: public Decorator {
    int goal_number;
    string player;
    string getGoal(int goal) const override;
    string getAssessment(int criteria) const override;
    bool getGeese(int tile) const override;
  public:
    GoalDecorator(Board *b, int goal, string player);
};

#endif
