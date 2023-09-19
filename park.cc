#include "park.h"

Park::Park(Tile *t) : TileDecorator{t}, rollVal(7) { type = getType(); }
std::string Park::getType() const { return "PARK"; }
std::string Park::getDisplayType() const { return "PARK  "; }
std::string Park::getDisplayRollValue() const { return "  "; }
Tile *Park::getComponent() const { return t; }
