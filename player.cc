#include "player.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>

Player::Player(int n, std::vector<ResourceTypes> r): 
    resources{r},
    playerNum{n} 
{
    totalResources = resources.size(); 
    if (playerNum == 0) {
        playerString = "Blue";
        playerSymbol = "B";
    } else if (playerNum == 1) {
        playerString = "Red";
        playerSymbol = "R";
    } else if (playerNum == 2) {
        playerString = "Orange";
        playerSymbol = "O";
    } else if (playerNum == 3) {
        playerString = "Yellow";
        playerSymbol = "Y";
    }
}
int Player::getPlayerNum() const { return playerNum; }
std::string Player::getPlayerString() const { return playerString; }
std::string Player::getPlayerSymbol() const { return playerSymbol; }
int Player::getBuilderPoints() const { return builderPoints; }
void Player::allocateResource(int resourceType, int howMany) {
    if (howMany > 0) {
        for (int i = 0; i < howMany; i++) {
            if (resourceType == 0) resources.emplace_back(Player::BRICK);
            else if (resourceType == 1) resources.emplace_back(Player::ENERGY);
            else if (resourceType == 2) resources.emplace_back(Player::GLASS);
            else if (resourceType == 3) resources.emplace_back(Player::HEAT);
            else if (resourceType == 4) resources.emplace_back(Player::WIFI);  
        }
    } else {
        ResourceTypes res;
        if (resourceType == 0) res = Player::BRICK;
        else if (resourceType == 1) res = Player::ENERGY;
        else if (resourceType == 2) res = Player::GLASS;
        else if (resourceType == 3) res = Player::HEAT;
        else if (resourceType == 4) res = Player::WIFI;
        for (int i = 0 - howMany; i > 0; i--) {
            auto it = std::find(resources.begin(), resources.end(), res);
            if (it != resources.end()) {
                resources.erase(it);
            }
        }
    }
    totalResources = resources.size();
}

int Player::getResourceNum(int resourceType) const {
    int count = std::count(resources.begin(), resources.end(), resourceType);
    return count;
}
void Player::createBuilding(int vertexNum, std::string building) {
    builderPoints++;
    buildingDict[vertexNum] = building; 
}
void Player::createRoad(int edgeNum) {
    roadIndices.emplace_back(edgeNum);
}
std::map<int, std::string> Player::getBuildingDict() const {
    return buildingDict;
}
std::vector<int> Player::getRoadIndices() const {
    return roadIndices;
}
void Player::printStatus() const {
    std::cout << getPlayerString() << " has " << getBuilderPoints() << " building points, "
        << getResourceNum(0) << " brick, " << getResourceNum(1) << " energy, "
        << getResourceNum(2) << " glass, " << getResourceNum(3) << " heat, and "
        << getResourceNum(4) << " WiFi." << std::endl;
}
void Player::printResidences() const {
    int count = 0;
    for (auto &[key, value] : buildingDict) {
        if (count != 0) std::cout << " ";
        std::cout << key << " " << value;
    }
    std::cout << std::endl;
}
void Player::loseRandom(int n, unsigned seed) {
    std::default_random_engine rng{seed};
    std::vector<int> nums{0, 1, 2, 3, 4};
    std::vector<int> copyVector;
    for (auto r : resources) copyVector.emplace_back(0);
    std::copy(resources.begin(), resources.end(), copyVector.begin());
    for (int i = 0; i < n; i++) {
        int n1;
        do {
            std::shuffle(nums.begin(), nums.end(), rng);
            n1 = nums[0];
        } while (getResourceNum(n1) == 0);
        allocateResource(n1, -1);
    }
    for (int i = 0; i < 5; i++) {
        if (std::count(copyVector.begin(), copyVector.end(), i) > getResourceNum(i)) {
            std::string what;
            if (i == 0) what = "Brick";
            else if (i == 1) what = "Energy";
            else if (i == 2) what = "Glass";
            else if (i == 3) what = "Heat";
            else if (i == 4) what = "WiFi";
            std::cout << std::count(copyVector.begin(), copyVector.end(), i) - getResourceNum(i) 
                << " " << what << std::endl;
        }
    }
}
int Player::getTotalResources() const { return totalResources; }

void Player::reset() {
    for (auto resource : resources) resources.pop_back();
    totalResources = 0;
    builderPoints = 0;
    for (auto r : roadIndices) roadIndices.pop_back();
    buildingDict.clear();
}
