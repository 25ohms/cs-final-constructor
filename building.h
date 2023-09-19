#ifndef _BUILDING_H_
#define _BUILDING_H_  
#include "boardcomponents.h"

class Building : public Vertex {
  public:
    enum BuildingType{BASEMENT = 1, HOUSE = 2, TOWER = 3};
  private:
    Vertex *component;
    BuildingType b;
  public:
    Building(Player *p, Vertex *v, BuildingType type);
    ~Building();
    int getBuildingType() const;
    void improveBuilding();
    std::string getDisplay() const override;
    std::string getBuildingSymbol() const;
    static int getResourceVal(std::string type);
    void notify(std::string type) override;
};

#endif
