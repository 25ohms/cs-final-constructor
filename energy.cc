#include "energy.h"

Energy::Energy(Tile *t, int rollVal) : TileDecorator{t, rollVal} { type = getType(); }
std::string Energy::getType() const { return "ENERGY"; }
std::string Energy::getDisplayType() const { return "ENERGY"; }
Tile *Energy::getComponent() const { return t; }
