#ifndef FILE_H
#define FILE_H

#include <vector>

class Tile {
    int resource;
    int dice_roll;
    bool geese;
  public:
    // Constructor
    Tile(int resource, int dice_roll, bool geese);
    // Accessors
    int getResource() const;
    bool hasGeese() const;
    int getNum() const;
    // Mutators
    void addGeese();
    void removeGeese();
};

#endif
