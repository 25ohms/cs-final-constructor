#include "blankVertex.h"
#include <iostream>
#include <string>

void BlankVertex::setAdjacentVertex(std::shared_ptr<Vertex> &vertex) { adjVertices.emplace_back(vertex); }
void BlankVertex::setTile(std::shared_ptr<Tile> &tile) { tiles.emplace_back(tile); } 
void BlankVertex::setEdge(std::shared_ptr<Edge> &edge) { edges.emplace_back(edge); } 
void BlankVertex::notify(std::string type) {}
std::string BlankVertex::getBuildingSymbol() const { return ""; }
std::string BlankVertex::getDisplay() const {
    std::string res = std::to_string(vertexNum);
    if (vertexNum < 10) return " " + res;
    else return "" + res;
}
