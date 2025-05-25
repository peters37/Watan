#include "../headers/geesedecorator.h"

GeeseDecorator::GeeseDecorator(Board *b, int tile): Decorator {b}, geese_tile {tile} {}

bool GeeseDecorator::getGeese(int tile) const {
    if (geese_tile == tile) {
        return true;
    }
    return false;
}

string GeeseDecorator::getAssessment(int criteria) const {
    return component->getAssessment(criteria);
}
   
string GeeseDecorator::getGoal(int goal) const {
    return component->getGoal(goal);
}
