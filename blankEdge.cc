#include "blankEdge.h"

void BlankEdge::setVertex(std::shared_ptr<Vertex> &vertex) { vertices.emplace_back(vertex); }
void BlankEdge::setTile(std::shared_ptr<Tile> &tile) { tiles.emplace_back(tile); } 
void BlankEdge::setAdjacentEdge(std::shared_ptr<Edge> &edge) { adjEdges.emplace_back(edge); }
