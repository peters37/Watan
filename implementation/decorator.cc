#include "../headers/decorator.h"

Decorator::Decorator(Board *b): Board {b}, component {b} {}

Decorator::~Decorator() {delete component;}
