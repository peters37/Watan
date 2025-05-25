#include "../headers/goal.h"

Goal::Goal(int number, std::vector<Goal *> adjacent_goals):
    number {number}, adjacent_goals {adjacent_goals}, player {nullptr} {}

int Goal::getNumber() const {
    return number;
}

Player *Goal::getPlayer() const {
    return player;
}

std::vector<Goal *>::iterator Goal::beginIterator() {
    return adjacent_goals.begin();
}

std::vector<Goal *>::iterator Goal::endIterator() {
    return adjacent_goals.end();
}

void Goal::addAdjacentGoal(Goal *g) {
    if (this != g) {
        adjacent_goals.emplace_back(g);
    }
}

void Goal::setPlayer(Player *new_player) {
    player = new_player;
}
