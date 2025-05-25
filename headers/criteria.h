#ifndef CRITERIA_H
#define CRITERIA_H

#include "goal.h"
#include "tile.h"
#include <vector>
#include <string>

class Player;

class Criteria {
    int number;
    std::vector<Criteria *> adjacent_criteria;
    std::vector<Goal *> adjacent_goals;
    std::vector<Tile *> tiles;
    int level;
    Player *player;
  public:
    // Constructor
    Criteria(int number, std::vector<Criteria *> adjacent_criteria, 
             std::vector<Goal *> adjacent_goals, std::vector<Tile *> tiles);
    // Accessors and Iterators
    int getNumber() const;
    Goal *getGoal(int j) const;
    int getLevel() const;
    char getLevelInitial() const;
    Player *getPlayer() const;
    std::vector<Goal *>::iterator beginGoalIterator();
    std::vector<Goal *>::iterator endGoalIterator();
    std::vector<Criteria *>::iterator beginCriteriaIterator();
    std::vector<Criteria *>::iterator endCriteriaIterator();
    std::vector<Tile *>::iterator beginTileIterator();
    std::vector<Tile *>::iterator endTileIterator();
    // Mutators
    void addAdjacentCriteria(Criteria *crt);
    void addAdjacentGoal(Goal *g);
    void addTile(Tile *t);
    void increaseLevel(int inc);
    void setPlayer(Player *new_player);
};

#endif
