#include "../headers/criteria.h"
#include "../headers/tile.h"
#include <vector>
#include <string>

Criteria::Criteria(int number, std::vector<Criteria *> adjacent_criteria, 
                   std::vector<Goal *> adjacent_goals, std::vector<Tile *> tiles):
    number {number}, adjacent_criteria {adjacent_criteria}, adjacent_goals {adjacent_goals}, tiles {tiles}, 
    level {0}, player {nullptr} {}


int Criteria::getNumber() const {
    return this->number;
}

Goal *Criteria::getGoal(int j) const {
    return adjacent_goals[j];
}

int Criteria::getLevel() const {
    return level;
}

char Criteria::getLevelInitial() const {
    if (level == 1) {
        return 'A';
    } else if (level == 2) {
        return 'M';
    } else {
        return 'F';
    }
}

Player *Criteria::getPlayer() const {
    return player;
}

std::vector<Goal *>::iterator Criteria::beginGoalIterator() {
    return adjacent_goals.begin();
}

std::vector<Goal *>::iterator Criteria::endGoalIterator() {
    return adjacent_goals.end();
}

std::vector<Criteria *>::iterator Criteria::beginCriteriaIterator() {
    return adjacent_criteria.begin();
}

std::vector<Criteria *>::iterator Criteria::endCriteriaIterator() {
    return adjacent_criteria.end();
}

std::vector<Tile *>::iterator Criteria::beginTileIterator() {
    return tiles.begin();
}

std::vector<Tile *>::iterator Criteria::endTileIterator() {
    return tiles.end();
}

void Criteria::addAdjacentCriteria(Criteria *crt) {
    adjacent_criteria.emplace_back(crt);
}

void Criteria::addAdjacentGoal(Goal *g) {
    adjacent_goals.emplace_back(g);
}

void Criteria::addTile(Tile *t) {
    tiles.emplace_back(t);
}

void Criteria::increaseLevel(int inc) {
    level += inc;
}

void Criteria::setPlayer(Player *new_player) {
    player = new_player;
}
