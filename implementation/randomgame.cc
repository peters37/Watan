#include "../headers/randomgame.h"

void RandomGame::setUpGame(std::vector<Player *> players) {
    std::vector<int> tile_nums = {2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12};
    std::vector<int> resources = {CAFFEINE, CAFFEINE, CAFFEINE, CAFFEINE, LAB, LAB, LAB, LAB, LECTURE, 
                                  LECTURE, LECTURE, LECTURE, STUDY, STUDY, STUDY, TUTORIAL, TUTORIAL,
                                  TUTORIAL};
    std::random_device seed;
    std::mt19937 gen(seed());
    std::shuffle(tile_nums.begin(), tile_nums.end(), gen);
    std::shuffle(resources.begin(), resources.end(), gen);
    Board *board = new BlankBoard {resources, tile_nums};
    updateBoard(board);
    init_tiles(resources, tile_nums);
}
