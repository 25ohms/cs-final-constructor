#ifndef _BLANK_VERTEX_H_
#define _BLANK_VERTEX_H_

#include "boardcomponents.h"

class BlankVertex : public Vertex {
  public:
    void setAdjacentVertex(std::shared_ptr<Vertex> &vertex) override;
    void setTile(std::shared_ptr<Tile> &tile) override;
    void setEdge(std::shared_ptr<Edge> &edge) override;
    void notify(std::string type) override;
    std::string getDisplay() const override;
    std::string getBuildingSymbol() const;
};

#endif
