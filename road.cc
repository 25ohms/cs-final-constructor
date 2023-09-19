#include "road.h"

Road::Road(Player *p, Edge *component) :
    Edge{component->getEdgeNum(), p}, component{component} {
        updateForAdjacent();
    }
std::string Road::getEdgeSymbol() const { return "R"; }
