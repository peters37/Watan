#include "../headers/player.h"

string convert_resource(int num);

Player::Player(Game *subject, string player, int victory_points, std::vector<Criteria *> criteria, std::vector<Goal *> goals):
    subject {subject}, player {player}, victory_points {victory_points}, caffeine {0}, lab {0}, lecture {0}, 
    study {0}, tutorial {0}, criteria {criteria}, goals {goals}, fair_dice {true}, gained_resources {false} {
        subject->attach(this);
}

Player::~Player() {
    subject->detach(this);
}

string Player::getName() const {
    return player;
}

char Player::getInitial() const {
    return player[0];
}

string Player::getCriteriaInitials(Criteria *new_criteria) const {
    ostringstream s;
    s << getInitial() << new_criteria->getLevelInitial();
    return s.str();
}

bool Player::getGainedResources() const {
    return gained_resources;
}

int Player::getVictoryPoints() const {
    return victory_points;
}

int Player::getResource(int resource) const {
    if (resource == CAFFEINE) {
        return caffeine;
    } else if (resource == LAB) {
        return lab;
    } else if (resource == LECTURE) {
        return lecture;
    } else if (resource == STUDY) {
        return study;
    } else if (resource == TUTORIAL) {
        return tutorial;
    } else {
        return -1;
    }
}

std::vector<Criteria *>::iterator Player::beginCriteriaIterator() {
    return criteria.begin();
}

std::vector<Criteria *>::iterator Player::endCriteriaIterator() {
    return criteria.end();
}

std::vector<Goal *>::iterator Player::beginGoalIterator() {
    return goals.begin();
}

std::vector<Goal *>::iterator Player::endGoalIterator() {
    return goals.end();
}

void Player::setResource(int resource, int num) {
    if (resource == CAFFEINE) {
        caffeine = num;
    } else if (resource == LAB) {
        lab = num;
    } else if (resource == LECTURE) {
        lecture = num;
    } else if (resource == STUDY) {
        study = num;
    } else {
        tutorial = num;
    }
}

void Player::addCriteria(Criteria *new_criteria) {
    new_criteria->increaseLevel(1);
    new_criteria->setPlayer(this);
    criteria.emplace_back(new_criteria);
    ++victory_points;
}

void Player::addGoal(Goal *new_goal) {
    new_goal->setPlayer(this);
    goals.emplace_back(new_goal);
}

void Player::setFair() {
    fair_dice = true;
}

void Player::setLoaded() {
    fair_dice = false;
}

void Player::changeResource(int resource, int change) {
    if (resource == CAFFEINE) {
        caffeine += change;
    } else if (resource == LAB) {
        lab += change;
    } else if (resource == LECTURE) {
        lecture += change;
    } else if (resource == STUDY) {
        study += change;
    } else {
        tutorial += change;
    }
}

void Player::upgradeCriteria(Criteria *improved_criteria, int upgrade) {
    improved_criteria->increaseLevel(upgrade);
    victory_points += upgrade;
}

void Player::resetPlayer() {
    victory_points = 0;
    caffeine = 0;
    lecture = 0;
    lab = 0;
    study = 0;
    tutorial = 0;
    criteria = {};
    goals = {};
    fair_dice = true;
    gained_resources = false;
}

void Player::notify(ostream &out) {
    gained_resources = false;
    int num = subject->getState();
    if (num == GEESE_ROLL) {
        return;
    }
    std::vector<int> new_resources = {0, 0, 0, 0, 0};
    for (auto it = criteria.begin(); it != criteria.end(); ++it) {
        for (auto it2 = (*it)->beginTileIterator(); it2 != (*it)->endTileIterator(); ++it2) {
            if ((*it2)->getNum() == num && !(*it2)->hasGeese()) {
                gained_resources = true;
                collectResource((*it2)->getResource(), (*it)->getLevel());
                new_resources[(*it2)->getResource()] += (*it)->getLevel();
            }
        }
    }
    if (gained_resources) {
        outputGainedResources(new_resources, out);
    }
}

void Player::collectResources(Criteria *new_criteria) {
    for (auto it = new_criteria->beginTileIterator(); it != new_criteria->endTileIterator(); ++it) {
        if ((*it)->hasGeese() == true) {
            continue;
        }
        int level = new_criteria->getLevel();
        if ((*it)->getResource() == CAFFEINE) { 
            caffeine += level;
        } else if ((*it)->getResource() == LAB) {
            lab += level;
        } else if ((*it)->getResource() == LECTURE) {
            lecture += level;
        } else if ((*it)->getResource() == STUDY) {
            study += level;
        } else if ((*it)->getResource() == TUTORIAL) {
            tutorial += level;
        }
    }
}

ostream &Player::getStatus(ostream &out) {
    out << player << " has " << victory_points << " victory points, " << caffeine << " caffeine, " << lab << " lab, ";
    out << lecture << " lecture, " << tutorial << " tutorial, and " << study << " study." << endl;
    return out;
}

bool Player::roll_dice(istream &in, ostream &out) {
    int dice_roll;
    if (fair_dice) { // Randomized roll
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(1, 6);
        dice_roll = distrib(gen) + distrib(gen);
        out << getName() << " rolled a " << dice_roll << "." << endl;
        subject->updateRoll(dice_roll, out);
        return true;
    } else {
        out << "Input a roll between 2 and 12: > ";
        if (in >> dice_roll && dice_roll >= 2 && dice_roll <= 12) {
            subject->updateRoll(dice_roll, out);
            return true;
        } else {
            return false;
        }
    }
}

void Player::collectResource(int resource, int level) {
    if (resource == CAFFEINE) {
        caffeine += level;
    } else if (resource == LAB) {
        lab += level;
    } else if (resource == LECTURE) {
        lecture += level;
    } else if (resource == STUDY) {
        study += level;
    } else {
        tutorial += level;
    }
}

ostream &Player::outputGainedResources(std::vector<int> new_resources, ostream &out) {
    out << "Student " << player << " gained:" << endl;
    for (int i = 0; i < new_resources.size(); ++i) {
        if (new_resources[i] != 0) {
            out << new_resources[i] << " " << convert_resource(i) << "." << endl;
        }
    }
    return out;
}

void Player::loseResources(ostream &out) {
    int total_resources = caffeine + lab + lecture + study + tutorial;
    if (!(total_resources >= LOSE_RESOURCES)) {
        return;
    }
    int lost_number = total_resources / 2;
    std::vector<int> all_resources = {};
    for (int i = 0; i < caffeine; ++i) {
        all_resources.emplace_back(CAFFEINE);
    }
    for (int i = 0; i < lab; ++i) {
        all_resources.emplace_back(LAB);
    }
    for (int i = 0; i < lecture; ++i) {
        all_resources.emplace_back(LECTURE);
    }
    for (int i = 0; i < study; ++i) {
        all_resources.emplace_back(STUDY);
    }
    for (int i = 0; i < tutorial; ++i) {
        all_resources.emplace_back(TUTORIAL);
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(all_resources.begin(), all_resources.end(), g);
    std::vector<int> lost_resources = {0, 0, 0, 0, 0};
    for (int i = 0; i < lost_number; ++i) {
        changeResource(i, -1);
        lost_resources[all_resources[i]] += 1;
    }
    outputLostResources(lost_resources, out);
}

ostream &Player::outputLostResources(std::vector<int> lost_resources, ostream &out) {
    out << "Student " << player << " loses " << lost_resources.size() << " resources to the geese. They lose:" << endl;
    for (int i = 0; i < lost_resources.size(); ++i) {
        if (lost_resources[i] != 0) {
            out << lost_resources[i] << " " << convert_resource(i) << "." << endl;
        }
    }
    return out;
}

bool Player::hasTile(int num) {
    Tile *tile = subject->getTile(num);
    for (auto it = criteria.begin(); it != criteria.end(); ++it) {
        for (auto it2 = (*it)->beginTileIterator(); it2 != (*it)->endTileIterator(); ++it2) {
            if ((*it2) == tile) {
                return true;
            }
        }
    }
    return false;
}

bool Player::hasResources() {
    if (caffeine + lab + lecture + study + tutorial > 0) {
        return true;
    }
    return false;
}

ostream &Player::printCriteria(ostream &out) {
    if (criteria.size() == 0) {
        out << "You have no completed criteria.";
    } else {
        out << getName() << " has completed:" << endl;;
    }
    for (auto it = criteria.begin(); it != criteria.end(); ++it) {
        out << (*it)->getNumber() << " " << (*it)->getLevel() << " ";
    }
    out << endl;
    return out;
}

bool Player::adjacentGoal(int num) {
    for (auto it = criteria.begin(); it != criteria.end(); ++it) {
        for (auto it2 = (*it)->beginGoalIterator(); it2 != (*it)->endGoalIterator(); ++it2) {
            if ((*it2)->getNumber() == num) {
                return true;
            }
        }
    }
    for (auto it = goals.begin(); it != goals.end(); ++it) {
        for (auto it2 = (*it)->beginIterator(); it2 != (*it)->endIterator(); ++it2) {
            if ((*it2)->getNumber() == num) {
                return true;
            }
        }
    }
    return false;
}

void Player::buyGoal(Goal *goal) {
    --tutorial;
    --study;
    addGoal(goal);
}

bool Player::adjacentCriterion(Criteria *new_criteria) {
    for (auto it = new_criteria->beginGoalIterator(); it != new_criteria->endGoalIterator(); ++it) {
        for (auto it2 = goals.begin(); it2 != goals.end(); ++it2) {
            if (*it2 == *it) {
                return true;
            }
        }
    }
    return false;
}

void Player::buyCriteria(Criteria *new_criteria) {
    if (new_criteria->getLevel() == 0) {
        --caffeine;
        --lab;
        --lecture;
        --tutorial;
        addCriteria(new_criteria);
    } else if (new_criteria->getLevel() == 1) {
        lecture -= 2;
        study -= 3;
        upgradeCriteria(new_criteria, 1);
    } else {
        caffeine -= 3;
        lab -= 2;
        lecture -= 2;
        --tutorial;
        study -= 2;
        upgradeCriteria(new_criteria, 1);
    }
}
