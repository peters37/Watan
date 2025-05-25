#include "../headers/board.h"

string convert_resource(int resource);

Board::Board(Board *b): resources {b->resources}, tile_nums {b->tile_nums} {}

Board::Board(std::vector<int> resources, std::vector<int> tile_nums):
    resources {resources}, tile_nums {tile_nums} {}

Board::~Board() {}

// Converts int to str, adds space to num if 1 digit
string convert_str(int num) {
    if (num < 10) {
        return " " + to_string(num);
    } else {
        return to_string(num);
    }
}

string toUpper(string str) {
    for (int i = 0; i < str.length(); ++i) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] -= 'a' - 'A'; // difference between ascii values of upper and lower
        }
    }
    return str;
}

int string_to_resource(string resource) {
    string upper_res = toUpper(resource);
    if (upper_res == "CAFFEINE") {
        return CAFFEINE;
    } else if (upper_res == "LAB") {
        return LAB;
    } else if (upper_res == "LECTURE") {
        return LECTURE;
    } else if (upper_res == "STUDY") {
        return STUDY;
    } else if (upper_res == "TUTORIAL") {
        return TUTORIAL;
    } else {
        return -1;
    }
}

ostream &operator<<(ostream &out, Board *b) {
    int goal_counter = 0;
    int criteria_counter = 0;
    int tile = 0;
    int numResource = 0;
    int tier = 1;
    int num_spaces = 2 * TILE_WIDTH + 5; // Required number of spaces for first tile
    while (tier <= 3) { // prints first third of the board
        for (int row = 1; row <= 4; ++row) {
            if (row == 3) { 
                num_spaces -= 2;
            }
            for (int i = 0; i < num_spaces; ++i) { // Adjust for spacing
                out << " ";
            }
            if (row == 3 && tier > 1) {
                tile += tier - 1;
                numResource += tier - 1;
            }
            out << b->getRow(row, goal_counter, criteria_counter, tile, numResource);
            if (tier >= 2) {
                out << b->getRow(row + 4, goal_counter, criteria_counter, tile, numResource);
                ++numResource;
                out << b->getRow(row, goal_counter, criteria_counter, tile + 1, numResource);
                --numResource;
            }
            if (tier >= 3) {
                if (row <= 3) {
                    ++numResource;
                    out << b->getRow(row + 4, goal_counter, criteria_counter, tile + 1, numResource);
                    out << b->getRow(row, goal_counter, criteria_counter, tile + 2, numResource);
                    --numResource;
                } else {
                    numResource += 2;
                    out << b->getRow(row + 4, goal_counter, criteria_counter, tile + 2, numResource);
                    out << b->getRow(row, goal_counter, criteria_counter, tile + 2, numResource);
                    numResource -= 2;
                }
            }
            out << endl;
        }
        tier += 1;
        num_spaces -= TILE_WIDTH - 2; // Going down a tier, less spaces needed
    }
    num_spaces = 0;
    int counter = 0;
    while (counter < 3) { // prints second portion of board
        for (int row = 1; row <= 4; ++row) {
            for (int i = 0; i < num_spaces; ++i) {
                out << " ";
            }
            if (row == 3) {
                tile += tier - 1;
                numResource += tier - 1;
            }
            if (tier == 4) {
                if (row == 1) {
                    out << "|" + b->getAssessment(criteria_counter) + "|";
                    ++criteria_counter;
                    num_spaces += 3;
                } else if (row == 2 || row == 4) {
                    out << "\\";
                } else {
                    out << b->getGoal(goal_counter);
                    ++goal_counter;
                    num_spaces += 2;
                }
                out << b->getRow(row + 4, goal_counter, criteria_counter, tile, numResource);
                out << b->getRow(row, goal_counter, criteria_counter, tile, numResource);
                ++numResource;
                out << b->getRow(row + 4, goal_counter, criteria_counter, tile + 1, numResource);
                out << b->getRow(row, goal_counter, criteria_counter, tile + 1, numResource);
                ++numResource;
                out << b->getRow(row + 4, goal_counter, criteria_counter, tile + 2, numResource);
                numResource -= 2;
                if (row == 1) {
                    out << "|" + b->getAssessment(criteria_counter) + "|";
                    ++criteria_counter;
                } else if (row == 2 || row == 4) {
                    out << "/";
                } else {
                    out << b->getGoal(goal_counter);
                    ++goal_counter;
                }
            } else if (tier == 3) {
                if (row == 2) {
                    num_spaces -= 2;
                }
                out << b->getRow(row, goal_counter, criteria_counter, tile, numResource);
                out << b->getRow(row + 4, goal_counter, criteria_counter, tile, numResource);
                ++numResource;
                out << b->getRow(row, goal_counter, criteria_counter, tile + 1, numResource);
                out << b->getRow(row + 4, goal_counter, criteria_counter, tile + 1, numResource);
                ++numResource;
                out << b->getRow(row, goal_counter, criteria_counter, tile + 2, numResource);
                numResource -= 2;
            }
            out << endl;
            if (row == 4) { // swap tier
                if (tier == 3) {
                    tier = 4;
                    num_spaces -= 3;
                } else {
                    tier = 3;
                    counter += 1;
                }
            }
        }
    }
    num_spaces = 5;
    tier = 3;
    counter = 0;
    for (int row = 1; row <= 4; ++row) {
        for (int i = 0; i < num_spaces; ++i) {
            out << " ";
        }
        if (row == 1) {
            out << b->getRow(row, goal_counter, criteria_counter, tile, numResource);
            num_spaces += 13;
        } else if (row == 2 || row == 4) {
            out << "\\";
        } else {
            out << b->getGoal(goal_counter);
            ++goal_counter;
            tile += 2;
            numResource += 2;
        }
        out << b->getRow(row + 4, goal_counter, criteria_counter, tile, numResource);
        out << b->getRow(row, goal_counter, criteria_counter, tile, numResource);
        ++numResource;
        out << b->getRow(row + 4, goal_counter, criteria_counter, tile + 1, numResource);
        --numResource;
        if (row == 1) {
            out << b->getRow(row, goal_counter, criteria_counter, tile, numResource);
        } else if (row == 2 || row == 4) {
            out << "/";
        } else {
            out << b->getGoal(goal_counter);
            ++goal_counter;
            ++counter;
            num_spaces += 2;
        }
        out << endl;
    }
    for (int row = 1; row <= 4; ++row) {
        for(int i = 0; i < num_spaces; ++i) {
            out << " ";
        }
        if (row == 1) {
            out << b->getRow(row, goal_counter, criteria_counter, tile, numResource);
        } else if (row == 2 || row == 4) {
            out << "\\";
        } else {
            out << b->getGoal(goal_counter);
            ++goal_counter;
        }
        out << b->getRow(row + 4, goal_counter, criteria_counter, tile, numResource);
        if (row == 1) {
            out << b->getRow(row, goal_counter, criteria_counter, tile, numResource);
            num_spaces += 13;
        } else if (row == 2 || row == 4) {
            out << "/";
        } else {
            out << b->getGoal(goal_counter);
            ++goal_counter;
            num_spaces += 2;
        }
        out << endl;
    }
    for (int i = 0; i < num_spaces; ++i) {
        out << " ";
    }
    out << b->getRow(1, goal_counter, criteria_counter, tile, numResource) << endl;
    return out;
}

// Convert resources to strings
string convert_resource(int r) {
    switch (r) {
        case CAFFEINE:
            return "CAFFEINE";
        case LAB:
            return "LAB";
        case LECTURE:
            return "LECTURE";
        case STUDY:
            return "STUDY";
        case TUTORIAL:
            return "TUTORIAL";
        default:
            return "NETFLIX";
    }
}

// Returns a row of a tile
string Board::getRow(int row, int &goal, int &criteria, int tile, int &numResources) {
    string res;
    string tile_str = convert_str(tile);
    string spaces = "";
    string ret_str;
    if (row == 1) {
        ret_str = "|" + getAssessment(criteria) + "|--" + getGoal(goal) + "--|" \
                + getAssessment(criteria + 1) + "|";
        goal += 1;
        criteria += 2;
        return ret_str;
    } else if (row == 2) {
        return "/            \\";
    } else if (row == 3) {
        ret_str = getGoal(goal) + "      " + tile_str + "     " + getGoal(goal + 1);
        goal += 2;
        return ret_str;
    } else if (row == 4) {
        if (tile_nums[tile] == GEESE_ROLL) { // If tile has GEESE_ROLL, it must have NETFLIX on it
            res = "NETFLIX";
            --numResources;
        } else {
            res = convert_resource(resources[numResources]);
        }
        for (int i = 0; i < TILE_WIDTH - 4 - res.length(); ++i) { // Account for proper spacing
            spaces += " ";
        }
        return "/     " + res + spaces + "\\";
    } else if (row == 5) {
        tile_str = convert_str(tile_nums[tile]);
        if (tile_nums[tile] == 7) { // If number is 7, don't display a number
            tile_str = "  ";
        }
        for (int i = 0; i < TILE_WIDTH / 2; ++i) {
            spaces += " ";
        }
        return spaces + tile_str + spaces;
    } else if (row == 6) {
        if (getGeese(tile)) {
            res = "GEESE";
        } else {
            res = "     ";
        }
        for (int i = 0; i < TILE_WIDTH / 2 - 2; ++i) {
            spaces += " ";
        }
        return spaces + res + spaces + " ";
    } else if (row == 7) {
        for (int i = 0; i < TILE_WIDTH - 2; ++i) {
            spaces += " ";
        }
        return spaces;
    } else {
        for (int i = 0; i < TILE_WIDTH - 3; ++i) {
            spaces += " ";
        }
        return spaces;
    }
}

