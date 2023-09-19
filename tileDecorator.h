#ifndef __TILE_DECORATOR_H__
#define __TILE_DECORATOR_H__
#include "boardcomponents.h"

class TileDecorator : public Tile {
  protected:
    Tile *t;
  public:
    TileDecorator(Tile *c, int rollVal = -1);
    virtual ~TileDecorator() = default;
};

#endif
