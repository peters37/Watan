#ifndef LOADGAME_H
#define LOADGAME_H

#include "game.h"
#include "blankboard.h"
#include "assessmentdecorator.h"
#include "goaldecorator.h"
#include "geesedecorator.h"

class LoadGame: public Game {
  string file;
  public:
    LoadGame(string file);
    void setUpGame(std::vector<Player *> players) override;
};

#endif
