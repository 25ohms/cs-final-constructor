#ifndef _GEESE_H_
#define _GEESE_H_
#include "tileDecorator.h"

class Geese : public TileDecorator {
  public:
    Geese(Tile *t, int rollVal = -1);
    void setComponent(Tile *newTile) override;
    std::string getType() const override;
    std::string getDisplayType() const override;
    Tile *getComponent() const;
};

#endif
