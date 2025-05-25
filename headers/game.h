#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "blankboard.h"
#include "subject.h"
#include "player.h"
#include "assessmentdecorator.h"
#include "geesedecorator.h"
#include "goaldecorator.h"
#include <iostream>
#include <random>
#include <fstream>
#include <sstream>

const int NUM_VICTORY_POINTS = 10;
const int TRADE_IN = 4;
class Player;

class Game: public Subject {
    Board *b;
    int turn;
    int dice_roll;
    Tile **tiles;
    Criteria **criteria;
    Goal **goals;
    int geese;
    bool enhanced;
  protected:
    void updateBoard(Board *new_board);
    void setTurn(int num_turn);
    void setGeese(int num);
  public:
    // Constructor and Destructor
    Game();
    ~Game();
    // Accessor Methods
    Criteria *getCriteria(int num_criteria) const;
    Goal *getGoal(int num_goal) const;
    Tile *getTile(int num_tile) const;
    Board *getBoard() const;
    int getTurn() const;
    int getState() const;
    // Mutator Methods
    void updateRoll(int roll, ostream &out);
    void resetGame(std::vector<Player *> players);
    void incTurn();
    bool moveGeese(int num);
    void enhance();
    // Initializing Methods
    void init_criteria();
    void init_tiles(std::vector<int> resources, std::vector<int> tile_nums);
    virtual void setUpGame(std::vector<Player *> players) = 0;
    // Game Logic Methods
    bool geeseRolled() const;
    bool addFirstAssignment(Player *player, int assignment, bool collect);
    bool gainedResources(std::vector<Player *> players) const;
    void loseResources(std::vector<Player *> players, ostream &out);
    bool adjacentStudents(int num, std::vector<Player *> players, Player *stealer, ostream &out);
    bool steal(Player *stealer, std::vector<Player *> players, string name, ostream &out, int num);
    ostream &getStatuses(std::vector<Player *> players, ostream &out);
    bool availableGoal(Player *player, int goal) const;
    bool buyGoal(Player *player, int num);
    bool availableCriterion(Player *player, int criterion_num) const;
    bool buyCriteria(Player *player, int criterion_num);
    bool improvableCriterion(Player *player, int criterion_num) const;
    bool improveCriterion(Player *player, int criterion_num);
    void save(std::vector<Player *> players, string save_file);
    bool winner(Player *player) const;
    bool tradeable(Player *player, string player2, string resource1, 
                   string resource2, std::vector<Player *> players) const;
    bool trade(Player *player, string player2, string resource1,
               string resource2, std::vector<Player *> players);
    bool exchangeable(Player *player, string res1, string res2) const;
    bool exchange(Player *player, string res1, string res2);
};

#endif
