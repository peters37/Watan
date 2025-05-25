#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <vector>
#include <iostream>
#include "criteria.h"
using namespace std;

const int TILE_WIDTH = 15; // width of each tile
const int NUM_CRITERIA = 54;
const int NUM_GOALS = 72;
const int NUM_TILES = 19;
const int GEESE_ROLL = 7;
const int LOSE_RESOURCES = 10;

string convert_str(int num);

enum Resource {CAFFEINE, LAB, LECTURE, STUDY, TUTORIAL, NETFLIX};

class Board {
    std::vector<int> resources;
    std::vector<int> tile_nums;
    string getRow(int row, int &goal, int &criteria, int tile, int &numResources);
    friend ostream &operator<<(ostream &out, Board *b);
  public:
    Board(Board *b);
    Board(std::vector<int> resources, std::vector<int> tile_nums);
    virtual ~Board();
    virtual string getAssessment(int criteria) const = 0;
    virtual string getGoal(int goal) const = 0;
    virtual bool getGeese(int tile) const = 0;
};

#endif
