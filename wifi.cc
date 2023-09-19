#include "wifi.h"

Wifi::Wifi(Tile *t, int rollVal) : TileDecorator{t, rollVal} { type = getType(); }
std::string Wifi::getType() const { return "WIFI"; }
std::string Wifi::getDisplayType() const { return "WIFI  "; }
Tile *Wifi::getComponent() const { return t; }
