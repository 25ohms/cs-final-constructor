#include "brick.h"

Brick::Brick(Tile *t, int rollVal) : TileDecorator{t, rollVal} { type = getType(); }
std::string Brick::getType() const { return "BRICK"; }
std::string Brick::getDisplayType() const { return "BRICK "; }
Tile *Brick::getComponent() const { return t; }
