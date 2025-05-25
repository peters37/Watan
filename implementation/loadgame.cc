#include "../headers/loadgame.h"
#include <iostream>

LoadGame::LoadGame(string file): file {file} {}

void LoadGame::setUpGame(std::vector<Player *> players) {
    std::vector<int> resources = {};
    std::vector<int> tile_nums = {};
    int cur_turn;
    ifstream read_file {file};
    read_file >> cur_turn;
    setTurn(cur_turn);
    int current_resource;
    string line;
    int num1;
    int num2;
    getline(read_file, line); // skip empty line
    for (int i = 0; i < players.size(); ++i) {
        getline(read_file, line);
        istringstream current_line {line};
        current_line >> current_resource;
        players[i]->setResource(CAFFEINE, current_resource);
        current_line >> current_resource;
        players[i]->setResource(LAB, current_resource);
        current_line >> current_resource;
        players[i]->setResource(LECTURE, current_resource);
        current_line >> current_resource;
        players[i]->setResource(STUDY, current_resource);
        current_line >> current_resource;
        players[i]->setResource(TUTORIAL, current_resource);
        current_line >> line;
        while (current_line >> num1) {
            players[i]->addGoal(getGoal(num1));
        }
        current_line.clear();
        current_line.ignore();
        while (current_line >> num1) {
            current_line >> num2;
            players[i]->addCriteria(getCriteria(num1));
            players[i]->upgradeCriteria(getCriteria(num1), num2 - 1);
        }
    }
    getline(read_file, line);
    istringstream current_line {line};
    while (current_line >> num1) {
        current_line >> num2;
        tile_nums.emplace_back(num2);
        if (num2 == GEESE_ROLL) {
            continue;
        }
        resources.emplace_back(num1);
    }
    Board *board = new BlankBoard {resources, tile_nums};
    for (int i = 0; i < players.size(); ++i) {
        for (auto it = players[i]->beginCriteriaIterator(); it != players[i]->endCriteriaIterator(); ++it) {
            ostringstream s;
            s << players[i]->getInitial() << (*it)->getLevelInitial();
            board = new AssessmentDecorator {board, (*it)->getNumber(), s.str()};
        }
        for (auto it = players[i]->beginGoalIterator(); it != players[i]->endGoalIterator(); ++it) {
            ostringstream s;
            s << players[i]->getInitial() << "A";
            board = new GoalDecorator {board, (*it)->getNumber(), s.str()};
        }
    }
    init_tiles(resources, tile_nums);
    int geese_num;
    read_file >> geese_num;
    // Check if geese is a valid number
    if (geese_num >= 0 && geese_num < NUM_TILES) {
        getTile(geese_num)->addGeese();
        board = new GeeseDecorator {board, geese_num};
        setGeese(geese_num);
    } else {
        setGeese(-1); // set geese -1 if geese is invalid
    }
    updateBoard(board);
}
