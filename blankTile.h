#ifndef _BLANK_TILE_H_
#define _BLANK_TILE_H_
#include "boardcomponents.h"

class BlankTile : public Tile {
  public:
    void setVertex(std::shared_ptr<Vertex> &vertex) override;
    void setEdge(std::shared_ptr<Edge> &Edge) override;
    Tile *getComponent() const;
    std::string getDisplayType() const override;
};

#endif
