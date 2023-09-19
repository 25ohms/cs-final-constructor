#ifndef _BOARD_H_
#define _BOARD_H_
#include <fstream>
#include <algorithm>
#include "boardcomponents.h"
#include "blankVertex.h"
#include "blankEdge.h"
#include "blankTile.h"

class Board {
    std::vector<std::shared_ptr<Vertex>> vertices;
    std::vector<std::shared_ptr<Tile>> tiles;
    std::vector<std::shared_ptr<Edge>> edges;
  public:
    Board();
    ~Board();
    void loadInfo();
    void loadRes(int vertNum, Player *curPlayer, int whatRes);
    void loadRoad(int edgeNum, Player *curPlayer);
    void buildRoad(int edgeNum, Player *curPlayer);
    void buildRes(int vertNum, Player *curPlayer);
    bool buildStartingRes(int vertNum, Player *curPlayer);
    void improveRes(int vertNum, Player *curPlayer);
    void printBoard();
    void inputData(std::string boardInfo);
    std::string getBoardInfo() const;
    int getGooseTile() const;
    bool moveGoose(int n, bool fromLoad = false);
    void notifyTiles(int roll);
    void randomizeBoard(unsigned seed);
};

#endif
