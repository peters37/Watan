#ifndef PLAYER_H
#define PLAYER_H

#include "goal.h"
#include "criteria.h"
#include "game.h"
#include <iostream>
#include <random>

class Criteria;
class Game;

class Player: public Observer {
    Game *subject;
    string player;
    int victory_points;
    int caffeine;
    int lab;
    int lecture;
    int study;
    int tutorial;
    std::vector<Criteria *> criteria;
    std::vector<Goal *> goals;
    bool fair_dice;
    bool gained_resources;
  public:
    // Constructors and Destructors
    Player(Game *subject, string player, int victory_points, std::vector<Criteria *> criteria, std::vector<Goal *> goals);
    ~Player() override;
    // Accessors and Iterators
    string getName() const;
    char getInitial() const;
    string getCriteriaInitials(Criteria *new_criteria) const;
    bool getGainedResources() const;
    int getVictoryPoints() const;
    int getResource(int resource) const;
    std::vector<Criteria *>::iterator beginCriteriaIterator();
    std::vector<Criteria *>::iterator endCriteriaIterator();
    std::vector<Goal *>::iterator beginGoalIterator();
    std::vector<Goal *>::iterator endGoalIterator();
    // Mutators
    void setResource(int resource, int num);
    void addCriteria(Criteria *new_criteria);
    void addGoal(Goal *new_goal);
    void setFair();
    void setLoaded();
    void changeResource(int resource, int change);
    void upgradeCriteria(Criteria *improved_criteria, int upgrade);
    void resetPlayer();
    // Player Logic Methods
    void notify(ostream &out) override;
    void collectResources(Criteria *new_criteria);
    ostream &getStatus(ostream &out);
    bool roll_dice(istream &in, ostream& out);
    void collectResource(int resource, int level);
    ostream &outputGainedResources(std::vector<int> new_resources, ostream &out);
    void loseResources(ostream &out);
    ostream &outputLostResources(std::vector<int> new_resources, ostream &out);
    bool hasTile(int num);
    bool hasResources();
    ostream &printCriteria(ostream &out);
    bool adjacentGoal(int num);
    void buyGoal(Goal *goal);
    bool adjacentCriterion(Criteria *new_criteria);
    void buyCriteria(Criteria *new_criteria);
};

#endif
