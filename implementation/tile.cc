#include "../headers/tile.h"

Tile::Tile(int resource, int dice_roll, bool geese):
    resource {resource}, dice_roll {dice_roll}, geese {geese} {}

int Tile::getResource() const {
    return resource;
}

bool Tile::hasGeese() const {
    return geese;
}

int Tile::getNum() const {
    return dice_roll;
}

void Tile::addGeese() {
    geese = true;
}

void Tile::removeGeese() {
    geese = false;
}
