#ifndef SEEDGAME_H
#define SEEDGAME_H

#include "game.h"
#include "blankboard.h"
#include <random>
#include <algorithm>

class SeedGame: public Game {
    int seed;
  public:
    SeedGame(int seed);
    void setUpGame(std::vector<Player *> players) override;
};

#endif
