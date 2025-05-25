#include "../headers/blankboard.h"

BlankBoard::BlankBoard(std::vector<int> resources, std::vector<int> tile_nums): 
             Board {resources, tile_nums} {}

string BlankBoard::getAssessment(int criteria) const {
    return convert_str(criteria);
}

bool BlankBoard::getGeese(int tile) const {
    return false;
}

string BlankBoard::getGoal(int goal) const {
    return convert_str(goal);
}
