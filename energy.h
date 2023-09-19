#ifndef _ENERGY_H_
#define _ENERGY_H_
#include "tileDecorator.h"

class Energy : public TileDecorator {
  public:
    Energy(Tile *t, int rollVal);
    std::string getType() const override;
    std::string getDisplayType() const override;
    Tile *getComponent() const override;
};

#endif
