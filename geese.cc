#include "geese.h"
#include <iostream>

Geese::Geese(Tile *t, int rollVal) : TileDecorator{t, rollVal} { type = getType(); }
std::string Geese::getType() const { return "GEESE"; }
std::string Geese::getDisplayType() const { return t->getDisplayType(); }
Tile *Geese::getComponent() const { return t; }
void Geese::setComponent(Tile *newTile) { t = newTile; }
