#ifndef _WIFI_H_
#define _WIFI_H_
#include "tileDecorator.h"

class Wifi : public TileDecorator {
  public:
    Wifi(Tile *t, int rollVal);
    std::string getType() const override;
    std::string getDisplayType() const override;
    Tile *getComponent() const override;
};

#endif
