#include "../headers/boardgame.h"

BoardGame::BoardGame(string file): file {file} {}

void BoardGame::setUpGame(std::vector<Player *> players) {
    std::vector<int> resources = {};
    std::vector<int> tile_nums = {};
    ifstream read_file {file};
    int num1;
    int num2;
    while (read_file >> num1) {
        read_file >> num2;
        tile_nums.emplace_back(num2);
        if (num2 == GEESE_ROLL) {
            continue;
        }
        resources.emplace_back(num1);
    }
    Board *board = new BlankBoard {resources, tile_nums};
    init_tiles(resources, tile_nums);
    updateBoard(board);
}
