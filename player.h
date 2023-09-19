#ifndef _PLAYER_H_
#define _PLAYER_H_
#include <map>
#include <vector>
#include <string>
#include <chrono>

// TODO: 
    // Implement bool hasResources() for given building/road
    // Implement spendResources() when constructing a building/road
class Player {
  public:
    enum ResourceTypes{BRICK, ENERGY, GLASS, HEAT, WIFI};
                    // 0      1       2      3     4
  private:
    std::vector<ResourceTypes> resources;
    const inline static std::vector<ResourceTypes> DEFAULT_RESOURCES;
    int totalResources = 0;
    std::map<int, std::string> buildingDict; 
    std::vector<int> roadIndices; 
    int builderPoints = 0;
    int playerNum;
    std::string playerString;
    std::string playerSymbol;
  public:
    void reset();
    void createBuilding(int vertexNum, std::string building);
    void createRoad(int edgeNum);
    int getPlayerNum() const;
    std::string getPlayerString() const;
    std::string getPlayerSymbol() const;
    std::map<int, std::string> getBuildingDict() const;
    std::vector<int> getRoadIndices() const;
    void allocateResource(int resourceType, int howMany);
    int getResourceNum(int resourceType) const;
    int getTotalResources() const;
    void printStatus() const;
    int getBuilderPoints() const;
    void printResidences() const;
    void loseRandom(int n, 
      unsigned seed = std::chrono::system_clock::now().time_since_epoch().count());
    Player(int n, std::vector<ResourceTypes> r = DEFAULT_RESOURCES);
};

#endif
