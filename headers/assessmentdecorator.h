#ifndef ASSESSMENTDECORATOR_H
#define ASSESSMENTDECORATOR_H

#include "decorator.h"

class AssessmentDecorator: public Decorator {
    int assessment_criteria;
    string assessment;
    string getAssessment(int criteria) const override;
    string getGoal(int goal) const override;
    bool getGeese(int criteria) const override;
  public:
    AssessmentDecorator(Board *b, int criteria, string assessment);
};

#endif
