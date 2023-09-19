#ifndef _PARK_H_
#define _PARK_H_
#include "tileDecorator.h"

class Park : public TileDecorator {
  int rollVal;
  public:
    Park(Tile *t);
    std::string getType() const override;
    std::string getDisplayType() const override;
    std::string getDisplayRollValue() const override;
    Tile * getComponent() const override;
};

#endif
