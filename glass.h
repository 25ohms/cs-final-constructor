#ifndef _GLASS_H_
#define _GLASS_H_
#include "tileDecorator.h"

class Glass : public TileDecorator {
  public:
    Glass(Tile *t, int rollVal);
    std::string getType() const override;
    std::string getDisplayType() const override;
    Tile *getComponent() const override;
};

#endif
