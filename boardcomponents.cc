#include "boardcomponents.h"
#include <string>
#include <iostream>

Vertex::Vertex(int vertexNum, Player *p) : vertexNum{vertexNum}, player{p} { ++numVertices; }
Vertex::~Vertex() { --numVertices; }
int Vertex::getVertexNum() const { return vertexNum; }
Player* Vertex::getPlayer() const { return player; }
int Vertex::getHowMany() const { return 0; }
std::vector<std::shared_ptr<Edge>> &Vertex::getEdges() { return edges; }
std::vector<std::shared_ptr<Vertex>> &Vertex::getAdjacentVertices() { return adjVertices; }
std::vector<std::shared_ptr<Tile>> &Vertex::getTiles() { return tiles; }
void Vertex::setAdjacentVertex(std::shared_ptr<Vertex> &vertex) { adjVertices.emplace_back(vertex); }
void Vertex::setEdge(std::shared_ptr<Edge> &edge) { edges.emplace_back(edge); }
void Vertex::setTile(std::shared_ptr<Tile> &tile) { tiles.emplace_back(tile); }
void Vertex::improveBuilding() {}
void Vertex::updateForAdjacent() {
    for (auto &v : adjVertices) {
        for (auto &av : v->getAdjacentVertices()) {
            if (av->getVertexNum() && av->getVertexNum() == vertexNum) av.reset(this);
        }
    }
    for (auto &e : edges) {
        for (auto &av : e->getVertices()) {
            if (av->getVertexNum() == vertexNum) av.reset(this);
        }
    }
    for (auto &t : tiles) {
        for (auto &av : t->getVertices()) {
            if (av->getVertexNum() == vertexNum) av.reset(this);
        }
    }
}


Edge::Edge(int edgeNum, Player *p) : edgeNum{edgeNum}, player{p} { ++numEdges; }
Edge::~Edge() { --numEdges; }
int Edge::getEdgeNum() const { return edgeNum; }
Player* Edge::getPlayer() const { return player; }
std::string Edge::getEdgeSymbol() const {
    std::string res = std::to_string(edgeNum);
    if (edgeNum < 10) return " " + res;
    else return "" + res;
}
std::string Edge::getDisplay() const {
    std::string s = "";
    if (getPlayer()) {
       s = getPlayer()->getPlayerSymbol() + "R";
    } else {
       s = "" + getEdgeSymbol();
    }
    return s;
}
std::vector<std::shared_ptr<Edge>> &Edge::getAdjacentEdges() { return adjEdges; }
std::vector<std::shared_ptr<Vertex>> &Edge::getVertices() { return vertices; }
std::vector<std::shared_ptr<Tile>> &Edge::getTiles() { return tiles; }
void Edge::setVertex(std::shared_ptr<Vertex> &vertex) { vertices.emplace_back(vertex); }
void Edge::setAdjacentEdge(std::shared_ptr<Edge> &edge) { adjEdges.emplace_back(edge); }
void Edge::setTile(std::shared_ptr<Tile> &tile) { tiles.emplace_back(tile); }
void Edge::updateForAdjacent() {
    for (auto &e : adjEdges) {
        std::shared_ptr<Edge> thisEdge(this);
        e->setAdjacentEdge(thisEdge);
    }
    for (auto &v : vertices) {
        std::shared_ptr<Edge> thisEdge(this);
        v->setEdge(thisEdge);
    }
    for (auto &t : tiles) {
        std::shared_ptr<Edge> thisEdge(this);
        t->setEdge(thisEdge);
    }
}

Tile::Tile(int tileNum, int rollVal) : 
    tileNum{tileNum}, rollVal(rollVal), isGoosed{false} { ++numTiles; }
Tile::~Tile() { --numTiles; };
void Tile::setComponent(Tile *newTile) {};
int Tile::getTileNum() const { return tileNum; }
std::string Tile::getType() const { return "blank"; }
int Tile::getRollValue() const { return rollVal; }
std::string Tile::getDisplayType() const { return "BLANK "; }
std::string Tile::getDisplayTileNum() const {
    std::string res = std::to_string(tileNum);
    if (tileNum < 10) return " " + res;
    else return "" + res;
};
std::string Tile::getDisplayRollValue() const {
    std::string res = std::to_string(rollVal);
    if (rollVal == 7) return "  ";
    else if (rollVal < 10) return " " + res;
    else return "" + res;
}
void Tile::changeGoosed() { isGoosed = !isGoosed; }
bool Tile::isGeesed() { return isGoosed; }
std::vector<std::shared_ptr<Edge>> &Tile::getEdges() { return edges; }
std::vector<std::shared_ptr<Vertex>> &Tile::getVertices() { return vertices; }
void Tile::setVertex(std::shared_ptr<Vertex> &vertex) { vertices.emplace_back(vertex); }
void Tile::setEdge(std::shared_ptr<Edge> &edge) { edges.emplace_back(edge); }
void Tile::setRollValue(int roll) { rollVal = roll; }
