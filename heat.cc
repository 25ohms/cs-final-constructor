#include "heat.h"

Heat::Heat(Tile *t, int rollVal) : TileDecorator{t, rollVal} { type = getType(); }
std::string Heat::getType() const { return "HEAT"; }
std::string Heat::getDisplayType() const { return "HEAT  "; }
Tile *Heat::getComponent() const { return t; }
