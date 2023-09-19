#ifndef _ROAD_H_
#define _ROAD_H_

#include "boardcomponents.h"

class Road : public Edge {
    Edge *component;
  public:
    Road(Player *p, Edge *component);
    std::string getEdgeSymbol() const override;
};

#endif
