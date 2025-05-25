#include "../headers/goaldecorator.h"

GoalDecorator::GoalDecorator(Board *b, int goal, string player):
    Decorator {b}, goal_number {goal}, player {player} {}

string GoalDecorator::getGoal(int goal) const {
    if (goal_number == goal) {
        return player;
    } else {
        return component->getGoal(goal);
    }
}

string GoalDecorator::getAssessment(int criteria) const {
    return component->getAssessment(criteria);
}

bool GoalDecorator::getGeese(int tile) const {
    return component->getGeese(tile);
}
