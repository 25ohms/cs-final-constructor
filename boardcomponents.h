#ifndef _BOARD_COMPONENTS_H_
#define _BOARD_COMPONENTS_H_

#include <vector>
#include <memory>
#include "player.h"
#include "observer.h"
#include "subject.h"

class Vertex;
class Edge;
class Tile;

class Vertex: public Observer {
    inline static int numVertices = 0; // max should be 54
  protected:
    int vertexNum;
    Player *player;
    std::vector<std::shared_ptr<Edge>> edges;
    std::vector<std::shared_ptr<Tile>> tiles;
    std::vector<std::shared_ptr<Vertex>> adjVertices;
  public:
    explicit Vertex(int vertexNum = numVertices, Player *p = nullptr);
    virtual ~Vertex();
    virtual std::string getBuildingSymbol() const = 0;
    virtual int getVertexNum() const;
    virtual Player* getPlayer() const;
    virtual int getHowMany() const;
    virtual std::string getDisplay() const = 0;
    virtual void setEdge(std::shared_ptr<Edge> &edge); // call by reference because we don't want to accidentally increase the ref count
    virtual void setTile(std::shared_ptr<Tile> &tile);
    virtual void setAdjacentVertex(std::shared_ptr<Vertex> &adjVertex);
    virtual std::vector<std::shared_ptr<Edge>> &getEdges();
    virtual std::vector<std::shared_ptr<Vertex>> &getAdjacentVertices();
    virtual std::vector<std::shared_ptr<Tile>> &getTiles();
    virtual void updateForAdjacent();
    virtual void improveBuilding();
};

class Edge {
    inline static int numEdges = 0; // max should be 72
  protected:
    int edgeNum;
    Player *player;
    std::vector<std::shared_ptr<Vertex>> vertices;
    std::vector<std::shared_ptr<Tile>> tiles;
    std::vector<std::shared_ptr<Edge>> adjEdges;
  public:
    explicit Edge(int edgeNum = numEdges, Player *p = nullptr);
    virtual ~Edge();
    virtual int getEdgeNum() const;
    virtual std::string getEdgeSymbol() const;
    virtual Player* getPlayer() const;
    virtual std::string getDisplay() const;
    virtual void setVertex(std::shared_ptr<Vertex> &vertex);
    virtual void setTile(std::shared_ptr<Tile> &tile);
    virtual void setAdjacentEdge(std::shared_ptr<Edge> &adjEdge);
    virtual std::vector<std::shared_ptr<Edge>> &getAdjacentEdges();
    virtual std::vector<std::shared_ptr<Vertex>> &getVertices();
    virtual std::vector<std::shared_ptr<Tile>> &getTiles();
    virtual void updateForAdjacent();
};

class Tile: public Subject {
    inline static int numTiles = 0; // max should be 19
  protected:
    int tileNum;
    int rollVal;
    std::vector<std::shared_ptr<Vertex>> vertices;
    std::vector<std::shared_ptr<Edge>> edges;
    bool isGoosed;
  public:
    Tile(int tileNum = numTiles, int rollVal = 0);
    virtual ~Tile();
    virtual int getTileNum() const;
    virtual int getRollValue() const;
    virtual void setComponent(Tile *newTile);
    virtual std::string getType() const;
    virtual std::string getDisplayType() const;
    virtual std::string getDisplayTileNum() const;
    virtual std::string getDisplayRollValue() const;
    virtual Tile *getComponent() const = 0;
    virtual void changeGoosed();
    virtual bool isGeesed();
    virtual void setRollValue(int roll);
    virtual void setVertex(std::shared_ptr<Vertex> &vertex);
    virtual void setEdge(std::shared_ptr<Edge> &edge);
    virtual std::vector<std::shared_ptr<Edge>> &getEdges();
    virtual std::vector<std::shared_ptr<Vertex>> &getVertices();
};



#endif
