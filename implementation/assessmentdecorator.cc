#include "../headers/assessmentdecorator.h"

AssessmentDecorator::AssessmentDecorator(Board *b, int criteria, string assessment):
    Decorator {b}, assessment_criteria {criteria}, assessment {assessment} {}

string AssessmentDecorator::getAssessment(int criteria) const {
    if (assessment_criteria == criteria) {
        return assessment;
    } else {
        return component->getAssessment(criteria);
    }
}

string AssessmentDecorator::getGoal(int goal) const {
    return component->getGoal(goal);
}

bool AssessmentDecorator::getGeese(int criteria) const {
    return component->getGeese(criteria);
}

