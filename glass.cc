#include "glass.h"

Glass::Glass(Tile *t, int rollVal) : TileDecorator{t, rollVal} { type = getType(); }
std::string Glass::getType() const { return "GLASS"; }
std::string Glass::getDisplayType() const { return "GLASS "; }
Tile *Glass::getComponent() const { return t; }
