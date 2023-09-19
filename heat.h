#ifndef _HEAT_H_
#define _HEAT_H_
#include "tileDecorator.h"

class Heat : public TileDecorator {
  public:
    Heat(Tile *t, int rollVal);
    std::string getType() const override;
    std::string getDisplayType() const override;
    Tile *getComponent() const override;
};

#endif
