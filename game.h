#include "board.h"
#include "player.h"
#include <random>
#include <chrono>

class Game {
    Board *b;
    std::vector<Player*> players;
    int curPlayer;
    void loadPlayerData(int playerNum, std::string info);
  public:
    Game();
    ~Game();
    void randomizeBoard(unsigned seed = std::chrono::system_clock::now().time_since_epoch().count());
    void setBoard(std::string file);
    void setPlayer(Player *player);
    void notifyTiles(int roll);
    void printBoard() const;
    void printStatus() const;
    void printCurRes() const;
    Player *getCurPlayer() const;
    void buildRoad(int edgeNum);
    void buildRes(int vertNum);
    bool buildStartingRes(int vertNum, std::string colour);
    void improveRes(int vertNum);
    void trade(int otherPlayer, std::string give, std::string take);
    void next();
    void save(std::string file);
    void load(std::string file);
    void help();
    void loadInfo();
    bool checkForWin();
    void swapGeese(bool hasSeed, 
      unsigned seed = std::chrono::system_clock::now().time_since_epoch().count());
};
