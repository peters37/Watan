#ifndef BLANKBOARD_H
#define BLANKBOARD_H

#include <string>
#include <vector>
#include "board.h"

class BlankBoard: public Board {
    string getAssessment(int criteria) const override;
    bool getGeese(int tile) const override;
    string getGoal(int goal) const override;
  public:
    BlankBoard(std::vector<int> resources, std::vector<int> tile_nums);
    ~BlankBoard() override {}
};

#endif
