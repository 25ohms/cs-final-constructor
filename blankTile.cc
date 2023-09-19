#include "blankTile.h"

void BlankTile::setVertex(std::shared_ptr<Vertex> &vertex) { vertices.emplace_back(vertex); }
void BlankTile::setEdge(std::shared_ptr<Edge> &edge) { edges.emplace_back(edge); } 
Tile *BlankTile::getComponent() const { return nullptr; }
std::string BlankTile::getDisplayType() const { return ""; }
