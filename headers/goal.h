#ifndef GOAL_H
#define GOAL_H

#include <vector>

class Player;

class Goal {
    int number;
    std::vector<Goal *> adjacent_goals;
    Player *player;
  public:
    // Constructor
    Goal(int number, std::vector<Goal *> adjacent_goals);
    // Accessors and Iterators
    int getNumber() const;
    Player *getPlayer() const;
    std::vector<Goal *>::iterator beginIterator();
    std::vector<Goal *>::iterator endIterator();
    // Mutators
    void addAdjacentGoal(Goal *g);
    void setPlayer(Player *new_player);
};

#endif
