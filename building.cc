#include "building.h"
#include <iostream>

Building::Building(Player *p, Vertex *v, BuildingType type): 
    Vertex{v->getVertexNum(), p},
    component{v},
    b{type}
{
    tiles = v->getTiles();
    adjVertices = v->getAdjacentVertices();
    edges = v->getEdges();
    for (auto &o : tiles) {
        o->attach(this);
    }
}

Building::~Building() {
    for(auto &o: tiles) o->detach(this);
}

void Building::improveBuilding() {
    if (b == Building::BASEMENT) b = Building::HOUSE;
    else if (b == Building::HOUSE) b = Building::TOWER;
}

int Building::getBuildingType() const { return b; }
std::string Building::getBuildingSymbol() const { 
    if (b == 1) return "B";
    else if (b == 2) return "H";
    else if (b == 3) return "T";
    else return "";
}
std::string Building::getDisplay() const {
    std::string s = "";
    s += getPlayer()->getPlayerSymbol();
    s += getBuildingSymbol();
    return s;
}

int Building::getResourceVal(std::string type) {
    int resourceVal = 0;
    if (type == "BRICK") {}
    if (type == "ENERGY") {++resourceVal; }
    if (type == "GLASS") {resourceVal += 2;}
    if (type == "HEAT") {resourceVal += 3;}
    if (type == "WIFI") {resourceVal += 4;}
    return resourceVal;
}

void Building::notify(std::string type) {
    int resourceVal = getResourceVal(type);
    player->allocateResource(resourceVal, b);
}
