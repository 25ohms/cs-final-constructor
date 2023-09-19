#ifndef _BLANK_EDGE_H_
#define _BLANK_EDGE_H_
#include "boardcomponents.h"

class BlankEdge : public Edge {
  public:
    void setVertex(std::shared_ptr<Vertex> &vertex) override;
    void setTile(std::shared_ptr<Tile> &tile) override;
    void setAdjacentEdge(std::shared_ptr<Edge> &adjEdge) override;
};

#endif
