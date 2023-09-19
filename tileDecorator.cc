#include "tileDecorator.h"

TileDecorator::TileDecorator(Tile *c, int rollVal) : 
    Tile{c->getTileNum(), (rollVal = -1) ? c->getRollValue() : rollVal}, t{c} {}
