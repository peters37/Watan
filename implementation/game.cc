#include "../headers/game.h"

// Declarations
string convert_str(int num);
string convert_resource(int resource);
int string_to_resource(string str);

Game::Game(): turn {0}, geese {-1}, enhanced {false} {
    init_criteria();
}

Game::~Game() {
    if (tiles) {
        for (int i = 0; i < NUM_TILES; ++i) {
            delete tiles[i];
        }
    }
    delete []tiles;
    if (goals) {
        for (int i = 0; i < NUM_GOALS; ++i) {
            delete goals[i];
        }
    }
    delete []goals;
    if (criteria) {
        for (int i = 0; i < NUM_CRITERIA; ++i) {
            delete criteria[i];
        }
    }
    delete []criteria;
    delete b;
}

Criteria *Game::getCriteria(int num_criteria) const {
    return criteria[num_criteria];
}

Goal *Game::getGoal(int num_goal) const {
    return goals[num_goal];
}

Tile *Game::getTile(int num_tile) const {
    return tiles[num_tile];
}

Board *Game::getBoard() const {
    return b;
}

int Game::getTurn() const {
    return turn;
}

int Game::getState() const {
    return dice_roll;
}

void Game::updateRoll(int roll, ostream &out) {
    dice_roll = roll;
    notifyObservers(out);
}

void Game::updateBoard(Board *new_board) {
    b = new_board;
}

void Game::setTurn(int turn_num) {
    turn = turn_num;
}

void Game::resetGame(std::vector<Player *> players) { // Resets game to a random game
    std::vector<int> tile_nums = {2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12};
    std::vector<int> resources = {CAFFEINE, CAFFEINE, CAFFEINE, CAFFEINE, LAB, LAB, LAB, LAB, LECTURE, 
                                  LECTURE, LECTURE, LECTURE, STUDY, STUDY, STUDY, TUTORIAL, TUTORIAL, 
                                  TUTORIAL};
    std::random_device seed;
    std::mt19937 gen(seed());
    std::shuffle(tile_nums.begin(), tile_nums.end(), gen);
    std::shuffle(resources.begin(), resources.end(), gen);
    delete b;
    Board *board = new BlankBoard {resources, tile_nums};
    updateBoard(board);
    turn = 0;
    geese = -1;
    for (int i = 0; i < players.size(); ++i) {
        players[i]->resetPlayer();
    }
    for (int i = 0; i < NUM_CRITERIA; ++i) {
        delete criteria[i];
    }
    delete criteria;
    for (int i = 0; i < NUM_GOALS; ++i) {
        delete goals[i];
    }
    delete goals;
    for (int i = 0; i < NUM_TILES; ++i) {
        delete tiles[i];
    }
    delete tiles;
    init_criteria();
    init_tiles(resources, tile_nums);
}

void Game::incTurn() {
    ++turn;
}

bool Game::moveGeese(int num) {
    if (num < 0 || num >= NUM_TILES || tiles[num]->hasGeese()) {
        return false;
    }
    Board *board = new GeeseDecorator {b, num};
    updateBoard(board);
    if (geese >= 0) {
        tiles[geese]->removeGeese();
    }
    tiles[num]->addGeese();
    geese = num;
    return true;
}

void Game::setGeese(int num) {
    geese = num;
}

void Game::enhance() {
    enhanced = true;
}

void Game::init_criteria() {
    criteria = new Criteria *[NUM_CRITERIA];
    for (int i = 0; i < NUM_CRITERIA; ++i) {
        criteria[i] = new Criteria {i, {}, {}, {}};
    }
    goals = new Goal *[NUM_GOALS];
    for (int i = 0; i < NUM_GOALS; ++i) {
        goals[i] = new Goal {i, {}};
    }
    int current_criteria = 0;
    int factor = 3;
    int current_goal = 0;
    int goal_factor = 1;
    int row = 1;
    int row_tile = 1;
    // Traces through board starting from the first and last criteria, 
    // adding the goals and criteria adjacent to each criteria
    while (2 * current_criteria < NUM_CRITERIA - 1) {

        // Covers case of middle row of tiles where our 2 traces meet
        if (current_criteria + 4 > NUM_CRITERIA / 2 || current_criteria - 4 > NUM_CRITERIA / 2) {
            if (!(current_criteria % 12 == 0) && !(current_criteria % 12 == 5)) {
                criteria[current_criteria]->addAdjacentCriteria(criteria[current_criteria + 1]);
                criteria[current_criteria + 1]->addAdjacentCriteria(criteria[current_criteria]);
                criteria[NUM_CRITERIA - 1 - current_criteria]->addAdjacentCriteria(criteria[NUM_CRITERIA - 2 - current_criteria]);
                criteria[NUM_CRITERIA - 2 - current_criteria]->addAdjacentCriteria(criteria[NUM_CRITERIA - 1 - current_criteria]);
                criteria[current_criteria]->addAdjacentGoal(goals[current_goal]);
                criteria[current_criteria + 1]->addAdjacentGoal(goals[current_goal]);
                criteria[NUM_CRITERIA - 1 - current_criteria]->addAdjacentGoal(goals[NUM_GOALS - 1 - current_goal]);
                criteria[NUM_CRITERIA - 2 - current_criteria]->addAdjacentGoal(goals[NUM_GOALS - 1 - current_goal]);
            } else {
                ++current_criteria;
                continue;
            }
            current_criteria += 2;
            continue;
        }

        // Add criteria directly beside other criteria
        criteria[current_criteria]->addAdjacentCriteria(criteria[current_criteria + factor]);
        criteria[current_criteria + factor]->addAdjacentCriteria(criteria[current_criteria]);
        criteria[NUM_CRITERIA - 1 - current_criteria]->addAdjacentCriteria(criteria[NUM_CRITERIA - 1 - current_criteria - factor]);
        criteria[NUM_CRITERIA - 1 - current_criteria - factor]->addAdjacentCriteria(criteria[NUM_CRITERIA - 1 - current_criteria]);

        // Covers the case of the criteria being at the left or right edge of the board
        if (current_criteria >= 12 && (current_criteria % 12 == 0 || current_criteria % 12 == 5)) {
            if (current_criteria % 12 == 0) {
                criteria[current_criteria]->addAdjacentGoal(goals[current_goal + 2]);
                criteria[current_criteria + factor]->addAdjacentGoal(goals[current_goal + 2]);
                criteria[NUM_CRITERIA - 1 - current_criteria]->addAdjacentGoal(goals[NUM_GOALS - 1 - current_goal - 2]);
                criteria[NUM_CRITERIA - 1 - current_criteria - factor]->addAdjacentGoal(goals[NUM_GOALS - 1 - current_goal - 2]);
            } else {
                criteria[current_criteria]->addAdjacentGoal(goals[current_goal + goal_factor]);
                criteria[current_criteria + factor]->addAdjacentGoal(goals[current_goal + goal_factor]);
                criteria[NUM_CRITERIA - 1 - current_criteria]->addAdjacentGoal(goals[NUM_GOALS - 1 - current_goal - goal_factor]);
                criteria[NUM_CRITERIA - 1 - current_criteria - factor]->addAdjacentGoal(goals[NUM_GOALS - 1 - current_goal - goal_factor]);
                row_tile = 1;
                goal_factor = row;
                current_goal += 2 * row;
            }
            ++current_criteria;
            continue;
        }

        // Add adjacent goals for each criteria
        criteria[current_criteria]->addAdjacentGoal(goals[current_goal]);
        criteria[current_criteria + 1]->addAdjacentGoal(goals[current_goal]);
        criteria[NUM_CRITERIA - 1 - current_criteria]->addAdjacentGoal(goals[NUM_GOALS - 1 - current_goal]);
        criteria[NUM_CRITERIA - 2 - current_criteria]->addAdjacentGoal(goals[NUM_GOALS - 1 - current_goal]);
        criteria[current_criteria]->addAdjacentGoal(goals[current_goal + goal_factor]);
        criteria[current_criteria + 1]->addAdjacentGoal(goals[current_goal + goal_factor + 1]);
        criteria[NUM_CRITERIA - 1 - current_criteria]->addAdjacentGoal(goals[NUM_GOALS - 1 - current_goal - goal_factor]);
        criteria[NUM_CRITERIA - 2 - current_criteria]->addAdjacentGoal(goals[NUM_GOALS - 2 - current_goal - goal_factor]);
        criteria[current_criteria + factor]->addAdjacentGoal(goals[current_goal + goal_factor]);
        criteria[current_criteria + factor + 1]->addAdjacentGoal(goals[current_goal + goal_factor + 1]);
        criteria[NUM_CRITERIA - 1 - current_criteria - factor]->addAdjacentGoal(goals[NUM_GOALS - 1 - current_goal - goal_factor]);
        criteria[NUM_CRITERIA - 2 - current_criteria - factor]->addAdjacentGoal(goals[NUM_GOALS - 2 - current_goal - goal_factor]);

        // Add addjacent criteria for each criteria
        criteria[current_criteria]->addAdjacentCriteria(criteria[current_criteria + 1]);
        criteria[current_criteria + 1]->addAdjacentCriteria(criteria[current_criteria]);
        criteria[NUM_CRITERIA - 1 - current_criteria]->addAdjacentCriteria(criteria[NUM_CRITERIA - 2 - current_criteria]);
        criteria[NUM_CRITERIA - 2 - current_criteria]->addAdjacentCriteria(criteria[NUM_CRITERIA - 1 - current_criteria]);
        criteria[current_criteria + 1]->addAdjacentCriteria(criteria[current_criteria + 1 + factor]);
        criteria[current_criteria + 1 + factor]->addAdjacentCriteria(criteria[current_criteria + 1]);
        criteria[NUM_CRITERIA - 2 - current_criteria]->addAdjacentCriteria(criteria[NUM_CRITERIA - 2 - current_criteria - factor]);
        criteria[NUM_CRITERIA - 2 - current_criteria - factor]->addAdjacentCriteria(criteria[NUM_CRITERIA - 2 - current_criteria]);
        current_criteria += 2;
        ++row_tile;
        ++goal_factor;

        if (row_tile > row) { // Update and reset parameters when we reach the end of a row
            row_tile = 1;
            current_goal += 2 * row;
            if (current_criteria == 2) {
                factor += 2;
                ++row;
            } else if (current_criteria == 6) {
                factor += 1;
                ++row;
            }
            goal_factor = row;
        }
        ++current_goal;
    }

    for (int i = 0; i < NUM_CRITERIA; ++i) { // Add adjacent goals for each goal
        for (auto it = criteria[i]->beginGoalIterator(); it != criteria[i]->endGoalIterator(); ++it) {
            auto it2 = it;
            ++it2;
            for (;it2 != criteria[i]->endGoalIterator(); ++it2) {
                (*it)->addAdjacentGoal(*it2);
                (*it2)->addAdjacentGoal(*it);
            }
        }
    }
}

void Game::init_tiles(std::vector<int> resources, std::vector<int> tile_nums) {
    tiles = new Tile *[NUM_TILES];
    std::vector<int> netflix_nums = {};
    for (int i = 0; i < NUM_TILES; ++i) { // Find which tiles has Netflix
        if (tile_nums[i] == GEESE_ROLL) {
            netflix_nums.emplace_back(i);
        }
    }
    int tile = 0;
    int factor = 0;
    int resource_counter = 0;
    int reverse_counter = resources.size() - 1;

    while (tile < 1) { // The bottom and top tile pattern is always the same
        if (tile_nums[tile] == GEESE_ROLL) { // Check if current tile is supposed to be NETFLIX
            tiles[tile] = new Tile {NETFLIX, tile_nums[tile], false};
        } else {
            tiles[tile] = new Tile {resources[resource_counter], tile_nums[tile], false};
            ++resource_counter;

        }
        if (tile_nums[NUM_TILES - tile - 1] == GEESE_ROLL) {
            tiles[NUM_TILES - tile - 1] = new Tile {NETFLIX, tile_nums[NUM_TILES - tile - 1], false};
        } else {
            tiles[NUM_TILES - tile - 1] = new Tile {resources[reverse_counter], tile_nums[NUM_TILES - tile - 1], false};
            --reverse_counter;
        }
        criteria[factor]->addTile(tiles[tile]);
        criteria[factor + 1]->addTile(tiles[tile]);
        criteria[factor + 3]->addTile(tiles[tile]);
        criteria[factor + 4]->addTile(tiles[tile]);
        criteria[factor + 8]->addTile(tiles[tile]);
        criteria[factor + 9]->addTile(tiles[tile]);
        criteria[NUM_CRITERIA - 1 - factor]->addTile(tiles[NUM_TILES - tile - 1]);
        criteria[NUM_CRITERIA - factor - 2]->addTile(tiles[NUM_TILES - tile - 1]);
        criteria[NUM_CRITERIA - factor - 4]->addTile(tiles[NUM_TILES - tile - 1]);
        criteria[NUM_CRITERIA - factor - 5]->addTile(tiles[NUM_TILES - tile - 1]);
        criteria[NUM_CRITERIA - factor - 9]->addTile(tiles[NUM_TILES - tile - 1]);
        criteria[NUM_CRITERIA - factor - 10]->addTile(tiles[NUM_TILES - tile - 1]);
        ++tile;
        factor += 2;
    }

    while (tile < 3) { // 2nd and 2nd last have the same pattern
        if (tile_nums[tile] == GEESE_ROLL) {
            tiles[tile] = new Tile {NETFLIX, tile_nums[tile], false};
        } else {
            tiles[tile] = new Tile {resources[resource_counter], tile_nums[tile], false};
            ++resource_counter;
        }
        if (tile_nums[NUM_TILES - 1 - tile] == GEESE_ROLL) {
            tiles[NUM_TILES - tile - 1] = new Tile {NETFLIX, tile_nums[NUM_TILES - tile - 1], false};
        } else {
            tiles[NUM_TILES - tile - 1] = new Tile {resources[reverse_counter], tile_nums[NUM_TILES - tile - 1], false};
            --reverse_counter;
        }
        criteria[factor]->addTile(tiles[tile]);
        criteria[factor + 1]->addTile(tiles[tile]);
        criteria[factor + 5]->addTile(tiles[tile]);
        criteria[factor + 6]->addTile(tiles[tile]);
        criteria[factor + 11]->addTile(tiles[tile]);
        criteria[factor + 12]->addTile(tiles[tile]);
        criteria[NUM_CRITERIA - factor - 1]->addTile(tiles[NUM_TILES - tile - 1]);
        criteria[NUM_CRITERIA - factor - 2]->addTile(tiles[NUM_TILES - tile - 1]);
        criteria[NUM_CRITERIA - factor - 6]->addTile(tiles[NUM_TILES - tile - 1]);
        criteria[NUM_CRITERIA - factor - 7]->addTile(tiles[NUM_TILES - tile - 1]);
        criteria[NUM_CRITERIA - factor - 12]->addTile(tiles[NUM_TILES - tile - 1]);
        criteria[NUM_CRITERIA - factor - 13]->addTile(tiles[NUM_TILES - tile - 1]);
        ++tile;
        factor += 2;
    }

    while (tile < NUM_TILES - 3) { // Recursive pattern for the middle rows of tiles
        if (tile_nums[tile] == GEESE_ROLL) {
            tiles[tile] = new Tile {NETFLIX, tile_nums[tile], false};
            --resource_counter;
        } else {
            tiles[tile] = new Tile {resources[resource_counter], tile_nums[tile], false};
        }
        criteria[factor]->addTile(tiles[tile]);
        criteria[factor + 1]->addTile(tiles[tile]);
        criteria[factor + 6]->addTile(tiles[tile]);
        criteria[factor + 7]->addTile(tiles[tile]);
        criteria[factor + 12]->addTile(tiles[tile]);
        criteria[factor + 13]->addTile(tiles[tile]);
        factor += 2;
        ++tile;
        if (factor >= 12 && (factor % 12 == 0 || factor % 12 == 5)) { // Edge of board case
            ++factor;
        }
        ++resource_counter;
    }
}

 bool Game::geeseRolled() const {
    if (dice_roll == GEESE_ROLL) {
        return true;
    }
    return false;
 }

bool Game::addFirstAssignment(Player *player, int assignment, bool collect) {
    if (!availableCriterion(player, assignment)) {
        return false;
    }
    player->addCriteria(getCriteria(assignment));
    if (collect && enhanced) { // Will collect on 2nd achievement placement if enhanced version
        player->collectResources(getCriteria(assignment));
    }
    Board *board = new AssessmentDecorator {b, assignment, player->getCriteriaInitials(criteria[assignment])};
    updateBoard(board);
    return true;
}

bool Game::gainedResources(std::vector<Player *> players) const { // Check if any player gained resources
    for (auto it = players.begin(); it != players.end(); ++it) {
        if ((*it)->getGainedResources()) {
            return true;
        }
    }
    return false;
}

void Game::loseResources(std::vector<Player *> players, ostream &out) {
    for (auto player:players) {
        player->loseResources(out);
    }
}

bool Game::adjacentStudents(int num, std::vector<Player *> players, Player *stealer, ostream &out) {
    int counter = 0;
    bool adjacent = false;
    for (auto it = players.begin(); it != players.end(); ++it) {
        if ((*it) != stealer && (*it)->hasTile(num) && (*it)->hasResources()) {
            adjacent = true;
            if (counter == 0) {
                out << "Student " << stealer->getName() << " can choose to steal from " << (*it)->getName();
            } else {
                out << ", " << (*it)->getName();
            }
            ++counter;
        }
    }
    if (adjacent) {
        out << "." << endl;
    }
    return adjacent;
}

bool Game::steal(Player *stealer, std::vector<Player *> players, string name, ostream &out, int num) {
    Player *stealee = nullptr;
      for (auto it = players.begin(); it != players.end(); ++it) {
        if ((*it)->getName() == name && (*it) != stealer && (*it)->hasTile(num) && (*it)->hasResources()) {
            stealee = *it;
        }
      }
    if (!stealee) {
        return false;
    }
    std::vector<int> stealee_resources = {};
    for (int i = 0; i < stealee->getResource(CAFFEINE); ++i) {
        stealee_resources.emplace_back(CAFFEINE);
    }
    for (int i = 0; i < stealee->getResource(LAB); ++i) {
        stealee_resources.emplace_back(LAB);
    }
    for (int i = 0; i < stealee->getResource(LECTURE); ++i) {
        stealee_resources.emplace_back(LECTURE);
    }
    for (int i = 0; i < stealee->getResource(STUDY); ++i) {
        stealee_resources.emplace_back(STUDY);
    }
    for (int i = 0; i < stealee->getResource(TUTORIAL); ++i) {
        stealee_resources.emplace_back(TUTORIAL);
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(stealee_resources.begin(), stealee_resources.end(), g);
    int stolen_resource = stealee_resources[0];
    out << "Student " << stealer->getName() << " steals ";
    out << convert_resource(stolen_resource);
    stealer->changeResource(stolen_resource, 1);
    stealee->changeResource(stolen_resource, -1);
    out << " from student " << stealee->getName() << "." << endl;
    return true;
}

ostream &Game::getStatuses(std::vector<Player *> players, ostream &out) {
    for (int i = 0; i < players.size(); ++i) {
        players[i]->getStatus(out);
    }
    return out;
}

bool Game::availableGoal(Player *player, int goal) const {
    if (goal >= NUM_GOALS || goal < 0) {
        return false;
    }
    if (goals[goal]->getPlayer()) {
        return false;
    }
    return player->adjacentGoal(goal);
}

bool Game::buyGoal(Player *player, int num) {
    if (player->getResource(STUDY) > 0 && player->getResource(TUTORIAL) > 0) {
        ostringstream s;
        s << player->getInitial() << "A"; // Goals always "A"
        Board *board = new GoalDecorator {b, num, s.str()};
        updateBoard(board);
        player->buyGoal(goals[num]);
        return true;
    }
    return false;
}

bool Game::availableCriterion(Player *player, int criterion_num) const {
    if (criterion_num < 0 || criterion_num >= NUM_CRITERIA) {
        return false;
    }
    if (criteria[criterion_num]->getPlayer()) {
        return false;
    }
    for (auto it = criteria[criterion_num]->beginCriteriaIterator(); 
         it != criteria[criterion_num]->endCriteriaIterator(); ++it) {
        if ((*it)->getPlayer()) {
            return false;
        }
    }
    if (turn > 0) { // Not applicable at beginning of game
        return player->adjacentCriterion(criteria[criterion_num]);
    }
    return true;
}

bool Game::buyCriteria(Player *player, int criterion_num) {
    if (player->getResource(CAFFEINE) > 0 && player->getResource(LAB) > 0 && player->getResource(LECTURE) > 0 && 
        player->getResource(TUTORIAL) > 0) {
        player->buyCriteria(criteria[criterion_num]);
        ostringstream s;
        s << player->getInitial() << criteria[criterion_num]->getLevelInitial();
        Board *board = new AssessmentDecorator {b, criterion_num, s.str()};
        updateBoard(board);
        return true;
    }
    return false;
}

bool Game::improvableCriterion(Player *player, int criterion_num) const {
    if (criterion_num < 0 || criterion_num >= NUM_CRITERIA || criteria[criterion_num]->getPlayer() != player || 
        criteria[criterion_num]->getLevel() >= 3) {
        return false;
    }
    return true;
}

bool Game::improveCriterion(Player *player, int criterion_num) {
    if (criteria[criterion_num]->getLevel() == 1) {
        if (player->getResource(LECTURE) >= 2 && player->getResource(STUDY) >= 3) {
            player->buyCriteria(criteria[criterion_num]);
            return true;
        } else {
            return false;
        }
    } else if (player->getResource(CAFFEINE) >= 3 && player->getResource(LAB) >= 2 && player->getResource(LECTURE) >= 2 && 
               player->getResource(TUTORIAL) >= 1 && player->getResource(STUDY) >= 2) {
        player->buyCriteria(criteria[criterion_num]);
        return true;
    } else {
        return false;
    }
}

void Game::save(std::vector<Player *> players, string save_file) {
    ofstream file {save_file};
    int geese = -1;
    file << turn << endl;
    for (auto it = players.begin(); it != players.end(); ++it) {
        file << (*it)->getResource(CAFFEINE) << " ";
        file << (*it)->getResource(LAB) << " ";
        file << (*it)->getResource(LECTURE) << " ";
        file << (*it)->getResource(STUDY) << " ";
        file << (*it)->getResource(TUTORIAL) << " ";
        file << 'g';
        for (auto it2 = (*it)->beginGoalIterator(); it2 != (*it)->endGoalIterator(); ++it2) {
            file << " " << (*it2)->getNumber();
        }
        file << " " << 'c';
        for (auto it2 = (*it)->beginCriteriaIterator(); it2 != (*it)->endCriteriaIterator(); ++it2) {
            file << " " << (*it2)->getNumber() << " " << (*it2)->getLevel();
        }
        file << endl;
    }
    for (int i = 0; i < NUM_TILES; ++i) {
        file << tiles[i]->getResource() << " " << tiles[i]->getNum() << " ";
        if (i == NUM_TILES - 1) {
            file << tiles[i]->getResource() << " " << tiles[i]->getNum();
        }
        if (tiles[i]->hasGeese()) {
            geese = i;
        }
    }
    file << endl;
    file << geese << endl;
}

bool Game::winner(Player *player) const {
    if (player->getVictoryPoints() >= NUM_VICTORY_POINTS) {
        return true;
    }
    return false;
 }

 bool Game::tradeable(Player *player, string player2, string resource1, string resource2, std::vector<Player *> players) const {
    Player *other_player = nullptr;
    for (int i = 0; i < players.size(); ++i) {
        if (player2 == players[i]->getName() && players[i] != player) { // Cannot trade with yourself
            other_player = players[i];
        }
    }
    if (!other_player) {
        return false;
    }
    int res1 = string_to_resource(resource1);
    int res2 = string_to_resource(resource2);
    if (res1 < 0 || res2 < 0) {
        return false;
    }
    return true;
 }

bool Game::trade(Player *player, string player2, string resource1,
               string resource2, std::vector<Player *> players) {
    Player *other_player = nullptr;
    for (int i = 0; i < players.size(); ++i) {
        if (players[i]->getName() == player2) {
            other_player = players[i];
        }
    }
    if (player->getResource(string_to_resource(resource1)) >= 1 && 
         other_player->getResource(string_to_resource(resource2)) >= 1) {
        player->changeResource(string_to_resource(resource1), -1);
        player->changeResource(string_to_resource(resource2), 1);
        other_player->changeResource(string_to_resource(resource1), 1);
        other_player->changeResource(string_to_resource(resource2), -1);
    } else {
        return false;
    }
    return true;
}

bool Game::exchangeable(Player *player, string res1, string res2) const {
    if (string_to_resource(res1) < 0 || string_to_resource(res2) < 0) {
        return false;
    }
    return true;
}

bool Game::exchange(Player *player, string res1, string res2) {
    int trading_in = string_to_resource(res1);
    int receiving = string_to_resource(res2);
    if (player->getResource(trading_in) >= TRADE_IN) {
        player->changeResource(trading_in, -1 * TRADE_IN);
        player->changeResource(receiving, 1);
        return true;
    }
    return false;
}
