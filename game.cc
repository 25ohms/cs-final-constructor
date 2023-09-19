#include "game.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <sstream>

Game::Game(): b{new Board}, curPlayer{0} {}
Game::~Game() { delete b; }

void Game::printBoard() const { b->printBoard(); }

void Game::printStatus() const {
    for (auto *p : players) p->printStatus();
}

void Game::loadInfo() {
    b->loadInfo();
}

void Game::setPlayer(Player *player) { players.emplace_back(player); }

Player *Game::getCurPlayer() const { return players[curPlayer]; }

void Game::notifyTiles(int roll) { b->notifyTiles(roll); }

void Game::printCurRes() const {
    players[curPlayer]->printResidences();
}

void Game::buildRoad(int edgeNum) {
    b->buildRoad(edgeNum, players[curPlayer]);
}

void Game::buildRes(int vertNum) {
    b->buildRes(vertNum, players[curPlayer]);
}

bool Game::buildStartingRes(int vertNum, std::string colour) {
    Player *p;
    if (colour == "Blue") p = players[0];
    else if (colour == "Red") p = players[1];
    else if (colour == "Orange") p = players[2];
    else if (colour == "Yellow") p = players[3];
    return b->buildStartingRes(vertNum, p); 
}

void Game::improveRes(int vertNum) {
    b->improveRes(vertNum, players[curPlayer]);
}

void Game::trade(int otherPlayer, std::string give, std::string take) {
    std::string r;
    std::cout << players[curPlayer]->getPlayerString() << " offers " << players[otherPlayer]->getPlayerString()
        << " one " << give << " for one " << take << ".\n Does " << players[otherPlayer]->getPlayerString()
        << " accept this offer? (yes/no)\n> ";
    while(true) {
        std::cin >> r;
        if (r == "yes" || r == "Yes") {
            int n1, n2;
            if (give == "Brick" || give == "brick") n1 = 0;
            else if (give == "Energy" || give == "energy") n1 = 1;
            else if (give == "Glass" || give == "glass") n1 = 2;
            else if (give == "Heat" || give == "heat") n1 = 3;
            else if (give == "Wifi" || give == "wifi" || give == "WiFi") n1 = 4;
            if (take == "Brick" || take == "brick") n2 = 0;
            else if (take == "Energy" || take == "energy") n2 = 1;
            else if (take == "Glass" || take == "glass") n2 = 2;
            else if (take == "Heat" || take == "heat") n2 = 3;
            else if (take == "Wifi" || take == "wifi" || take == "WiFi") n2 = 4;
            players[curPlayer]->allocateResource(n1, -1);
            players[curPlayer]->allocateResource(n2, 1);
            players[otherPlayer]->allocateResource(n2, -1);
            players[otherPlayer]->allocateResource(n1, 1);
            break;
        } else if (r == "no" || r == "No") break;
        else std::cout << "Invalid entry, enter yes/no: " << std::endl;
    }
}

void Game::next() { curPlayer = (curPlayer + 1) % 4; }

void Game::loadPlayerData(int playerNum, std::string info) {
    std::istringstream iss{info};
    int n;
    for (int j = 0; j < 5; j++) {
        iss >> n;
        players[playerNum]->allocateResource(j, n);
    }
    std::string str, cond, what;
    while (iss >> str) {
        if (str == "r" || str == "h") {
            cond = str;
            continue;
        } else if (cond == "r") {
            n = std::stoi(str);
            b->loadRoad(n, players[playerNum]);
        } else if (cond == "h") {
            iss >> what;
            n = std::stoi(str);
            if (what == "B") {
                b->loadRes(n, players[playerNum], 1);
            } else if (what == "H") {
                b->loadRes(n, players[playerNum], 2);
            } else if (what == "T") {
                b->loadRes(n, players[playerNum], 3);
            }
        }
    }
}

void Game::load(std::string file) {
    std::ifstream f(file + ".sv");
    std::string line, val;
    
    // load current player
    std::getline(f, line);
    std::istringstream iss{line};
    iss >> curPlayer;

    // get player info
    std::vector<std::string> playerInfoStrings;
    for (int i = 0; i < 4; i++) {
        std::getline(f, line);
        playerInfoStrings.emplace_back(line);
    }

    // load board info
    std::getline(f, line);
    b->inputData(line);
    b->loadInfo();

    // load player info
    for (int i = 0; i < 4; i++) {
        loadPlayerData(i, playerInfoStrings[i]);
    }

    // load goose tile
    int goose;
    f >> goose;
    b->moveGoose(goose, true);
}

void Game::save(std::string file) {
    std::ofstream f(file + ".sv");
    f << curPlayer << std::endl;
    std::string val;
    std::ostringstream oss;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 5; j++) {
            f << players[i]->getResourceNum(j) << " ";
        }
        std::vector<int> roadIndices = players[i]->getRoadIndices();
        std::map<int, std::string> buildDict = players[i]->getBuildingDict();
        if (!roadIndices.empty()) {
            f << "r" << " ";
            for (auto r : roadIndices) f << r << " ";
        }
        if (!buildDict.empty()) {
            f << "h" << " ";
            int count = 0;
            for (auto &pair : buildDict) {
                if (count != 0) f << " ";
                count++;
                f << pair.first << " " << pair.second;
            }
        }
        f << std::endl;
    }
    f << b->getBoardInfo() << std::endl;
    f << b->getGooseTile() << std::endl;
}

void Game::setBoard(std::string file) { 
    std::ifstream f(file + ".txt");
    std::string line;
    std::getline(f, line);
    b->inputData(line);
    loadInfo();
}

void Game::randomizeBoard(unsigned seed) {
    b->randomizeBoard(seed);
    loadInfo();
}

void Game::help() {
    std::cout << "Valid commands:\nboard\nstatus\nresidences\nbuild-road <edge#>\n"
        << "build-res <housing#>\nimprove <housing#>\ntrade <colour> <give> <take>\n"
        << "next\nsave <file>\nhelp" << std::endl;
}

void Game::swapGeese(bool hasSeed, unsigned seed) {
    for (auto &p : players) {
        int half = p->getTotalResources() / 2;
        if (p->getTotalResources() >= 10) {
            std::cout << "Builder " << p->getPlayerString() << " loses " 
                << half << " resources to the geese. They lose:" << std::endl;
            if (hasSeed) p->loseRandom(half, seed);
            else p->loseRandom(half);
        }
    }
    std::cout << "Choose where to place the GEESE." << std::endl;
    int n;
    do {
        std::cout << "> ";
        std::cin >> n;
    } while (!b->moveGoose(n));
    std::vector<Player*> possibleVictims;
    for (auto &p : players) {
        if (p != players[curPlayer] && p->getTotalResources() > 0) {
            std::map<int, std::string> dict = p->getBuildingDict();
            if (dict.count(n) > 0) possibleVictims.emplace_back(p);
        }
    }
    if (!possibleVictims.empty()) {
        std::cout << "Builder " << players[curPlayer]->getPlayerString() << " can choose to steal from: ";
        int count = 0;
        for (auto  &p : possibleVictims) {
            if (count != 0) std::cout << ", ";
            std::cout << p->getPlayerString(); 
        }
        bool notFound = true;
        while (notFound) {
            std::cout << "\n> ";
            std::string victim = "";
            if (std::cin >> victim){
                for (auto  &p : possibleVictims) {
                    if (victim == p->getPlayerString()) {
                        notFound = false;
                        std::vector<int> nums;
                        for (int i = 0; i < 5; i++) {
                            for (int j = 0; j < p->getResourceNum(i); j++) {
                                nums.emplace_back(i);
                            }
                        }
                        std::default_random_engine rng{seed};
                        std::shuffle(nums.begin(), nums.end(), rng);
                        int n1 = nums[0];
                        p->allocateResource(n1, -1);
                        players[curPlayer]->allocateResource(n1, 1);
                        std::string what = "";
                        if (n1 == 0) what = "brick";
                        else if (n1 == 1) what = "energy";
                        else if (n1 == 2) what = "glass";
                        else if (n1 == 3) what = "heat";
                        else if (n1 == 4) what = "WiFi";
                        std::cout << "Builder " << players[curPlayer]->getPlayerString() << " steals "
                            << what << " from builder " << p->getPlayerString() << "." << std::endl;
                    }
                }
                if (notFound) std::cout << "That builder cannot be stolen from." << std::endl;
            }
        }
    } else std::cout << "Builder " << players[curPlayer]->getPlayerString()
        << " has no builders to steal from." << std::endl;
    
}

bool Game::checkForWin() {
    if (getCurPlayer()->getBuilderPoints() >= 10) return true;
    else return false;
}
