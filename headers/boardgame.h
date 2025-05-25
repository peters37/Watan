#ifndef BOARDGAME_H
#define BOARDGAME_H

#include "game.h"
#include "blankboard.h"
#include <iostream>

class BoardGame: public Game {
  string file;
  public:
    BoardGame(string file);
    void setUpGame(std::vector<Player *> players) override;
};

#endif
