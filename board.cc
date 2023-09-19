#include "board.h"
#include "boardcomponents.h"
#include "blankEdge.h"
#include "blankTile.h"
#include "blankVertex.h"
#include "building.h"
#include "road.h"
#include "brick.h"
#include "energy.h"
#include "glass.h"
#include "heat.h"
#include "wifi.h"
#include "park.h"
#include "geese.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <random>

Board::Board() {
    for (int i = 0; i < 72; ++i) {
        Edge *newEdge = new BlankEdge{};
        edges.emplace_back(std::shared_ptr<Edge>(newEdge));
    }
    for (int i = 0; i < 54; ++i) {
        Vertex *newVertex = new BlankVertex{};
        vertices.emplace_back(std::shared_ptr<Vertex>(newVertex));
    }
    for (int i = 0; i < 19; ++i) {
        Tile *newTile = new BlankTile{};
        tiles.emplace_back(std::shared_ptr<Tile>(newTile));
    }
}

Board::~Board() {}

void Board::loadInfo() {
    std::ifstream tf{"tileinfo.txt"};
    std::string line;
    while(std::getline(tf, line)) {
        std::istringstream iss{line};
        std::string s;
        int t, n;
        iss >> t >> s;
        if (s == "e") {
            while (iss >> n) tiles[t]->setEdge(edges[n]);
        } else if (s == "v") {
            while (iss >> n) tiles[t]->setVertex(vertices[n]);
        }
    }
    std::ifstream vf{"vertexinfo.txt"};
    while(std::getline(vf, line)) {
        std::istringstream iss{line};
        std::string s;
        int v, n;
        iss >> v >> s;
        if (s == "e") {
            while (iss >> n) vertices[v]->setEdge(edges[n]);
        } else if (s == "t") {
            while (iss >> n) vertices[v]->setTile(tiles[n]);
        } else if (s == "v") {
            while (iss >> n) vertices[v]->setAdjacentVertex(vertices[n]);
        }
    }
    std::ifstream ef{"edgeinfo.txt"};
    while(std::getline(ef, line)) {
        std::istringstream iss{line};
        std::string s;
        int e, n;
        iss >> e >> s;
        if (s == "e") {
            while (iss >> n) edges[e]->setAdjacentEdge(edges[n]);
        } else if (s == "t") {
            while (iss >> n) edges[e]->setTile(tiles[n]);
        } else if (s == "v") {
            while (iss >> n) edges[e]->setVertex(vertices[n]);
        }
    }
}

void Board::inputData(std::string boardInfo) {
    std::istringstream iss{boardInfo};
    for (int i = 0; i < 19; i++) {
        int type, val;
        Tile *newTile;
        iss >> type >> val;
        if (type == 0) newTile = new Brick{tiles[i].get(), val};
        else if (type == 1) newTile = new Energy{tiles[i].get(), val};
        else if (type == 2) newTile = new Glass{tiles[i].get(), val};
        else if (type == 3) newTile = new Heat{tiles[i].get(), val};
        else if (type == 4) newTile = new Wifi{tiles[i].get(), val};
        else if (type == 5) {
            newTile = new Park{tiles[i].get()};
            newTile->changeGoosed();
        }
        newTile->setRollValue(val);
        tiles[i].reset(newTile);
        newTile = nullptr;
    }
}

void Board::buildRoad(int edgeNum, Player *curPlayer) {
    if (curPlayer->getResourceNum(3) >= 1 && curPlayer->getResourceNum(4) >= 1) {
        if (!edges[edgeNum]->getPlayer()) {
            bool canBuild = false;
            for (auto &v : edges[edgeNum]->getVertices())  {
                if (v->getPlayer() == curPlayer) canBuild = true;
            }
            if (!canBuild) {
                for (auto &e : edges[edgeNum]->getAdjacentEdges()) {
                    if (edges[e->getEdgeNum()]->getPlayer() == curPlayer) canBuild = true;
                }
                for (auto &v : edges[edgeNum]->getVertices()) {
                    if (vertices[v->getVertexNum()]->getPlayer()) canBuild = false;
                }
            }
            if (!canBuild) {
                for (auto &e : edges[edgeNum]->getAdjacentEdges()) {
                    if (edges[e->getEdgeNum()]->getPlayer() == curPlayer && !canBuild) {
                        canBuild = true;
                        for (auto &v : e->getVertices()) {
                            if (vertices[v->getVertexNum()]->getPlayer() && 
                                vertices[v->getVertexNum()]->getPlayer() != curPlayer) 
                                canBuild = false;
                        }
                    }
                }
            }
            if (canBuild) {
                curPlayer->allocateResource(3, -1);
                curPlayer->allocateResource(4, -1);
                Edge *newRoad = new Road{curPlayer, edges[edgeNum].get()};
                edges[edgeNum].reset(newRoad);
                curPlayer->createRoad(edgeNum);
                edges[edgeNum]->updateForAdjacent();
            } else std::cout << "You cannot build here." << std::endl;
        } else std::cout << "You cannot build here." << std::endl;
    } else std::cout << "You do not have enough resources." << std::endl;
}

void Board::buildRes(int vertNum, Player *curPlayer) {
    if (curPlayer->getResourceNum(0) >= 1 && curPlayer->getResourceNum(1) >= 1 
        && curPlayer->getResourceNum(2) >= 1 && curPlayer->getResourceNum(4) >= 1) {
        bool canBuild = false;
        for (auto &e : vertices[vertNum]->getEdges()) {
            if (edges[e->getEdgeNum()]->getPlayer() == curPlayer) canBuild = true;
        }
        if (vertices[vertNum]->getPlayer()) canBuild = false;
        for (auto &v : vertices[vertNum]->getAdjacentVertices()) if (v->getPlayer()) canBuild = false;
        if (canBuild == true) {
            curPlayer->allocateResource(0, -1);
            curPlayer->allocateResource(1, -1);
            curPlayer->allocateResource(2, -1);
            curPlayer->allocateResource(4, -1);
            Vertex *newRes = new Building{curPlayer, vertices[vertNum].get(), Building::BASEMENT};
            vertices[vertNum].reset(newRes);
            curPlayer->createBuilding(vertNum, "B");
            vertices[vertNum]->updateForAdjacent();
        } else std::cout << "You cannot build there." << std::endl;
    } else std::cout << "You do not have enough resources." << std::endl;
}

void Board::loadRes(int vertNum, Player *curPlayer, int whatRes) {
    Vertex *newRes;
    if (whatRes == 1) {
        newRes = new Building{curPlayer, vertices[vertNum].get(), Building::BASEMENT};
        curPlayer->createBuilding(vertNum, "B");
    } else if (whatRes == 2) {
         newRes = new Building{curPlayer, vertices[vertNum].get(), Building::HOUSE};
         curPlayer->createBuilding(vertNum, "H");
    } else if (whatRes == 3) {
        newRes = new Building{curPlayer, vertices[vertNum].get(), Building::TOWER};
        curPlayer->createBuilding(vertNum, "T");
    } 
    vertices[vertNum].reset(newRes);
    vertices[vertNum]->updateForAdjacent();
}

void Board::loadRoad(int edgeNum, Player *curPlayer) {
    Edge *newRoad = new Road{curPlayer, edges[edgeNum].get()};
    edges[edgeNum].reset(newRoad);
    curPlayer->createRoad(edgeNum);
    edges[edgeNum]->updateForAdjacent();
}

bool Board::buildStartingRes(int vertNum, Player *curPlayer) {
    bool canBuild = true;
    for (auto &v : vertices[vertNum]->getAdjacentVertices()) {
        if (v->getPlayer()) canBuild = false;
    } if (canBuild && !vertices[vertNum]->getPlayer()) {
        Vertex *newRes = new Building{curPlayer, vertices[vertNum].get(), Building::BASEMENT};
        vertices[vertNum].reset(newRes);
        curPlayer->createBuilding(vertNum, "B");
        vertices[vertNum]->updateForAdjacent();
        return true;
    } else {
        std::cout << "You cannot build here." << std::endl;
        return false;
    }
}

void Board::improveRes(int vertNum, Player *curPlayer) {
    if (vertices[vertNum]->getPlayer() == curPlayer 
        && vertices[vertNum]->getBuildingSymbol() != "T") {
        if (vertices[vertNum]->getBuildingSymbol() == "B" 
            && curPlayer->getResourceNum(2) >= 2 && curPlayer->getResourceNum(3) >= 3) {
            curPlayer->allocateResource(2, -2);
            curPlayer->allocateResource(3, -3);
            vertices[vertNum]->improveBuilding();
            curPlayer->createBuilding(vertNum, "H");
        } else if (vertices[vertNum]->getBuildingSymbol() == "H" && curPlayer->getResourceNum(0) >= 3
            && curPlayer->getResourceNum(1) >= 2 && curPlayer->getResourceNum(2) >= 2 
            && curPlayer->getResourceNum(3) >= 2 && curPlayer->getResourceNum(4) >= 1) {
            vertices[vertNum]->improveBuilding();
            curPlayer->createBuilding(vertNum, "T");
        } else std::cout << "You cannot build here." << std::endl;
    } else std::cout << "You cannot build here." << std::endl;
}

std::string Board::getBoardInfo() const{
    std::string s = "";
    for (int i = 0; i < 19; i++) {
        std::string r = tiles[i]->getType();
        if (r == "BRICK") {
            s += "0 ";
        } else if (r == "ENERGY") {
            s += "1 ";
        } else if (r == "GLASS") {
            s += "2 ";
        } else if (r == "HEAT") {
            s += "3 ";
        } else if (r == "WIFI") {
            s += "4 ";
        }
        if (r != "PARK") s += std::to_string(tiles[i]->getRollValue());
        else s += "5 7";
        if (i != 18) s += " ";
    }
    return s;
}

int Board::getGooseTile() const {
    for (int i = 0; i < 19; i++) {
        if (tiles[i]->isGeesed()) return i;
    }
    return -1;
}

bool Board::moveGoose(int n, bool fromLoad) {
    int prev = getGooseTile();
    if (!fromLoad && (n == prev || n < 0 || n > 18)) {
        std::cout << "Enter a valid tile." << std::endl;
        return false;
    } else if (n == prev) {
        return false;
    } else if (prev >= 0) {
        tiles[prev]->changeGoosed();
        tiles[n]->changeGoosed();
        return true;
    } else {
        tiles[n]->changeGoosed();
        return true;
    }
}

void Board::notifyTiles(int roll) {
    for (auto &t : tiles) {
        if (t->getRollValue() == roll && !t->isGeesed()) {
            t->notifyObservers();
        }
    }
 }

void Board::randomizeBoard(unsigned seed) {
    int n1, n2;
    std::vector<int> tileNums{4, 4, 4, 3, 3, 1};
    std::vector<int> rollNums{2, 3, 3, 4, 4, 5, 5, 6, 6,
                              8, 8, 9, 9, 10, 10, 11, 11, 12};
    std::vector<int> n1Nums{0, 1, 2, 3, 4, 5};
    std::vector<int> n2Nums{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
                            14, 15, 16, 17};
    std::default_random_engine rng{seed};
    std::vector<int> emptyNums{0, 0, 0, 0, 0, 0};
    std::string s = "";
    int count = 0;
    std::cin.clear();
    while (tileNums != emptyNums) {
        if (count != 0) s += " ";
        count++;
        while (true) {
            std::shuffle(n1Nums.begin(), n1Nums.end(), rng);
            std::shuffle(n2Nums.begin(), n2Nums.end(), rng);
            n1 = n1Nums[0];
            n2 = n2Nums[0];
            if (tileNums[n1] > 0 && rollNums[n2] > 0) break;
        }
        if (n1 == 5) {
            tileNums[5]--;
            s += "5 7";
        } else {
            s += std::to_string(n1) + " " + std::to_string(rollNums[n2]);
            tileNums[n1]--;
            rollNums[n2] = 0;
        }
    }
    inputData(s);
}

void Board::printBoard() {
    int nextVert = 0;
    int nextEdge = 0;
    int nextTile = 0;
    for (int i = 0; i < 41; i++) {
        if (i % 4 == 0) {
            if ((nextVert > 1 && nextVert < 6) || (nextVert > 47 && nextVert < 52)) {
                std::cout << "          |" << vertices[nextVert]->getDisplay() << "|--" << edges[nextEdge]->getDisplay() 
                    << "--|" << vertices[nextVert + 1]->getDisplay() << "|  " << tiles[nextTile - 1]->getDisplayRollValue() << "  |" 
                    << vertices[nextVert + 2]->getDisplay() << "|--" << edges[nextEdge + 1]->getDisplay() << "--|" 
                    << vertices[nextVert + 3]->getDisplay() << "|          " << std::endl;
                nextVert += 4;
                nextEdge += 2;
            } else if (nextVert > 5 && nextVert < 48) {
                if (i % 8 == 0) {
                    std::cout << "|" << vertices[nextVert]->getDisplay() << "|--" << edges[nextEdge]->getDisplay() 
                        << "--|" << vertices[nextVert + 1]->getDisplay() << "|  " << tiles[nextTile - 2]->getDisplayRollValue() << "  |" 
                        << vertices[nextVert + 2]->getDisplay() << "|--" << edges[nextEdge + 1]->getDisplay() << "--|" 
                        << vertices[nextVert + 3]->getDisplay() << "|  " << tiles[nextTile - 1]->getDisplayRollValue() << "  |"
                        << vertices[nextVert + 4]->getDisplay() << "|--" << edges[nextEdge + 2]->getDisplay() << "--|" 
                        << vertices[nextVert + 5]->getDisplay() << "|" << std::endl;
                    nextVert += 6;
                    nextEdge += 3;
                } else {
                    std::cout << "|" << vertices[nextVert]->getDisplay() << "|  " << tiles[nextTile - 3]->getDisplayRollValue()
                        << "  |" << vertices[nextVert + 1]->getDisplay() << "|--" << edges[nextEdge]->getDisplay() << "--|" 
                        << vertices[nextVert + 2]->getDisplay() << "|  " << tiles[nextTile - 2]->getDisplayRollValue() << "  |" 
                        << vertices[nextVert + 3]->getDisplay() << "|--" << edges[nextEdge + 1]->getDisplay() << "--|"
                        << vertices[nextVert + 4]->getDisplay() << "|  " << tiles[nextTile - 1]->getDisplayRollValue() << "  |" 
                        << vertices[nextVert + 5]->getDisplay() << "|" << std::endl;
                    nextVert += 6;
                    nextEdge += 2;
                }
            } else {
                std::cout << "                    |" << vertices[nextVert]->getDisplay() << "|--" << edges[nextEdge]->getDisplay() 
                    << "--|" << vertices[nextVert + 1]->getDisplay() << "|                    " << std::endl; 
                nextVert += 2;
                nextEdge++;
            }
        } else if ((i - 1) % 4 == 0) {
            if (i == 1 || (i == 37 && !tiles[18]->isGeesed())) 
                std::cout << "                      |         |                     " << std::endl;
            else if (i == 37 && tiles[18]->isGeesed())
                std::cout << "                      |  GEESE  |                     " << std::endl;
            else if (i == 33) {
                std::cout << "            |";
                if (tiles[nextTile - 2]->isGeesed()) std::cout << "  GEESE  |";
                else std::cout << "         |";
                std::cout << "         |";
                if (tiles[nextTile - 1]->isGeesed()) std::cout << "  GEESE  |           " << std::endl;
                else std::cout << "         |           " << std::endl;
            } else if (i == 5) {
                std::cout << "            |         |";
                if (tiles[nextTile - 1]->isGeesed()) std::cout << "  GEESE  |           " << std::endl;
                else std::cout << "         |";
                std::cout << "         |           " << std::endl;
            } else if ((i - 1) % 8) {
                std::cout << "  |";
                if (tiles[nextTile - 3]->isGeesed()) std::cout << "  GEESE  |";
                else std::cout << "         |";
                std::cout << "         |";
                if (tiles[nextTile - 2]->isGeesed()) std::cout << "  GEESE  |";
                else std::cout << "         |";
                std::cout << "         |";
                if (tiles[nextTile - 1]->isGeesed()) std::cout << "  GEESE  | " << std::endl;
                else std::cout << "         | " << std::endl;
            } else {
                std::cout << "  |         |";
                if (tiles[nextTile - 2]->isGeesed()) std::cout << "  GEESE  |";
                else std::cout << "         |";
                std::cout << "         |";
                if (tiles[nextTile - 1]->isGeesed()) std::cout << "  GEESE  | " << std::endl;
                else std::cout << "         |         | " << std::endl;
            }
        } else if ((i - 2) % 4 == 0) {
            if (nextVert > 5 && nextVert < 48) {
                if (nextVert == 6) {
                    std::cout << "           " << edges[nextEdge]->getDisplay() << "   "  << tiles[nextTile]->getDisplayTileNum() 
                        << "   " << edges[nextEdge + 1]->getDisplay() << "        "  << edges[nextEdge + 2]->getDisplay() 
                        << "   "  << tiles[nextTile + 1]->getDisplayTileNum() << "   " << edges[nextEdge + 3]->getDisplay() 
                        << "           " << std::endl;
                    nextEdge += 4;
                    nextTile += 2;
                } else if ((i - 2) % 8 == 0) {
                    std::cout << " " << edges[nextEdge]->getDisplay() << "   "  << tiles[nextTile]->getDisplayTileNum() 
                        << "   " << edges[nextEdge + 1]->getDisplay() << "        "  << edges[nextEdge + 2]->getDisplay() 
                        << "   "  << tiles[nextTile + 1]->getDisplayTileNum() << "   " << edges[nextEdge + 3]->getDisplay()
                        << "        "  << edges[nextEdge + 4]->getDisplay() << "   "  
                        << tiles[nextTile + 2]->getDisplayTileNum() << "   " << edges[nextEdge + 5]->getDisplay() << std::endl;
                    nextEdge += 6;
                    nextTile += 3;
                } else if (nextVert == 2) {
                    std::cout << "                     " << edges[nextEdge]->getDisplay() << "   " 
                        << tiles[nextTile]->getDisplayTileNum() << "   " << edges[nextEdge + 1]->getDisplay() 
                        << "                     " << std::endl;
                    nextEdge += 2;
                    nextTile++;
                } else if ((i + 2) % 8 == 0) {
                    std::cout << " " << edges[nextEdge]->getDisplay() << "        "  << edges[nextEdge + 1]->getDisplay() 
                        << "   "  << tiles[nextTile]->getDisplayTileNum() << "   " << edges[nextEdge + 2]->getDisplay() << "        " 
                        << edges[nextEdge + 3]->getDisplay() << "   " << tiles[nextTile + 1]->getDisplayTileNum() << "   " 
                        << edges[nextEdge + 4]->getDisplay() << "        " << edges[nextEdge + 5]->getDisplay() 
                        << std::endl;
                    nextEdge += 6;
                    nextTile += 2;
                }
            } else if (nextVert == 2) {
                std::cout << "                     " << edges[nextEdge]->getDisplay() << "   "  << tiles[nextTile]->getDisplayTileNum() 
                    << "   " << edges[nextEdge + 1]->getDisplay() << "                     " << std::endl;
                nextEdge += 2;
                nextTile++;
            } else if (nextVert == 48) {
                std::cout << "           " << edges[nextEdge]->getDisplay() << "        " << edges[nextEdge + 1]->getDisplay() 
                    << "   " << tiles[nextTile]->getDisplayTileNum() << "   " << edges[nextEdge + 2]->getDisplay() <<
                    "        " << edges[nextEdge + 3]->getDisplay() << "          " << std::endl;
                nextEdge += 4;
                nextTile += 1;
            } else if (nextVert == 52) {
                std::cout << "                     " << edges[nextEdge]->getDisplay() << "        " 
                << edges[nextEdge + 1]->getDisplay() << "                     " << std::endl;
                nextEdge += 2;
            }
        } else if ((i - 3) % 4 == 0) {
            if (i == 3) {
                std::cout << "                      |  " << tiles[nextTile - 1]->getDisplayType() 
                << " |                     " << std::endl;
            }  else if (i == 7) {
                std::cout << "            |  " << tiles[nextTile - 2]->getDisplayType() << " |         |  " 
                    << tiles[nextTile - 1]->getDisplayType() << " |           " << std::endl;
            } else if (i == 35) {
                std::cout << "            |         |  " << tiles[nextTile - 1]->getDisplayType() 
                    << " |         |           " << std::endl;
            } else if (i == 39)  {
                std::cout << "                      |         |                     " << std::endl;
            } else if ((i - 7) % 8 == 0) {
                std::cout << "  |         |  " << tiles[nextTile - 2]->getDisplayType() << " |         |  " 
                    << tiles[nextTile - 1]->getDisplayType() << " |         | " << std::endl;
            } else if ((i + 5) % 8 == 0) {
                std::cout << "  |  " << tiles[nextTile - 3]->getDisplayType() << " |         |  " << tiles[nextTile - 2]->getDisplayType() 
                    << " |         |  " << tiles[nextTile - 1]->getDisplayType() << " | " << std::endl;
            }
            
        }
    }
}
