#ifndef RANDOMGAME_H
#define RANDOMGAME_H

#include "game.h"
#include "blankboard.h"
#include <random>
#include <algorithm>

class RandomGame: public Game {
  public:
    void setUpGame(std::vector<Player *> players) override;
};

#endif
