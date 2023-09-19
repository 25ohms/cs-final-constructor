#ifndef _BRICK_H_
#define _BRICK_H_
#include "tileDecorator.h"

class Brick : public TileDecorator {
  public:
    Brick(Tile *t, int rollVal);
    std::string getType() const override;
    std::string getDisplayType() const override;
    Tile *getComponent() const override;
};

#endif
