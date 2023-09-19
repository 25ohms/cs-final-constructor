#include "game.h"
#include "player.h"
#include "fairDice.h"
#include "loadedDice.h"
#include "player.h"
#include <iostream>
#include <sstream>



int main(int argc, char *argv[]) {
    bool exitProgram = false;
    while (!exitProgram) {
        std::vector<Player*> players;
        Player *winner = nullptr;
        Game *g = new Game{};
        for (int i = 0; i < 4; i++) {
            players.emplace_back(new Player{i});
            g->setPlayer(players[i]);
        }
        bool boardLoaded = false;
        bool loadRandom = false;
        bool hasSeed = false;
        bool loadSaved = false;
        unsigned seed;
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            std::string prvArg = argv[i - 1];
            if (prvArg == "-seed") {
                std::istringstream iss{arg};
                iss >> seed;
                hasSeed = true;
            } else if (prvArg == "-load" && !boardLoaded) {
                boardLoaded = true;
                loadSaved = true;
                loadRandom = false;
                g->load(arg);
            } else if (prvArg == "-board" && !boardLoaded) {
                boardLoaded = true;
                loadRandom = false;
                g->setBoard(arg);
            } else if (arg == "-random-board" && !boardLoaded) {
                loadRandom = true;
            }
        }
        if (loadRandom) {
            if (hasSeed) g->randomizeBoard(seed);
            else {
                g->randomizeBoard();
            }
        } else if (!boardLoaded){
            g->setBoard("layout");
        } 
        g->printBoard();
        bool endGame = false;
        if (!loadSaved) {
            for (int i = 0; i < 8; i++) {
                std::string colour = "";
                if (i == 0 || i == 7) colour = "Blue";
                else if (i == 1 || i == 6) colour = "Red";
                else if (i == 2 || i == 5) colour = "Orange";
                else if (i == 3 || i == 4) colour = "Yellow";
                std::cout << "Builder " << colour << ", where do you want to build a basement?" << std::endl;
                while (true) {
                    std::cout << "> ";
                    int val;
                    if (std::cin >> val && val >= 0 && val < 54 && g->buildStartingRes(val, colour)) break;
                    else if(std::cin.eof()) {
                        endGame = true;
                        exitProgram = true;
                        break;
                    } else {
                        std::cout << "Enter a valid number:" << std::endl;
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    } 
                }
                if (exitProgram) break;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::unique_ptr<LoadedDice> loaded = std::make_unique<LoadedDice>();
        std::unique_ptr<FairDice> fair; 
        if (hasSeed) fair = std::make_unique<FairDice>(seed);
        else fair = std::make_unique<FairDice>();
        Dice *curDice = fair.get();
        Player *curPlayer;
        while (!endGame) {
            curPlayer = g->getCurPlayer();
            std::cout << "Builder " << curPlayer->getPlayerString() << "'s turn." << std::endl;
            curPlayer->printStatus();
            std::string command;
            bool commandPhase = false;
            bool rollPhase = true;
            std::string line;
            while (rollPhase) {
                std::cout << "> ";
                if (!std::getline(std::cin, line)) {
                    g->save("backup");
                    rollPhase = false;
                    endGame = true;
                    break;
                }
                std::istringstream iss{line};
                while (iss >> command) {
                    if (command == "load" ) {
                        curDice = loaded.get();
                    }
                    else if (command == "fair") {
                        curDice = fair.get();
                    }
                    else if (command == "roll") {
                        int roll = curDice->roll();
                        if (roll == 7) {
                            if (hasSeed) g->swapGeese(true, seed);
                            else g->swapGeese(false);
                        } else {
                            g->notifyTiles(roll);
                        }
                        rollPhase = false;
                        commandPhase = true;
                        break;
                    } else std::cout << "Invalid command." << std::endl;
                }
                if (std::cin.eof()) g->save("backup");
                else if (commandPhase) std::cout << "> ";
            }
            if (endGame || std::cin.eof()) break;
            while (commandPhase) {
                if (!std::getline(std::cin, line)) {
                    g->save("backup");
                    endGame = true;
                    commandPhase = false;
                    break;
                }
                std::istringstream iss{line};
                while (iss >> command) {
                    if (command == "board" ) {
                        g->printBoard();
                    } else if (command == "status") {
                        g->getCurPlayer()->printStatus();
                    } else if (command == "residences") {
                        std::cout << "Player " << curPlayer->getPlayerString() << " has the built the following residences: ";
                        g->printCurRes();
                        std::cout << std::endl;
                    } else if (command == "build-res") {
                        int n;
                        iss >> n;
                        g->buildRes(n);
                        if (g->checkForWin()) {
                            endGame = true;
                            commandPhase = false;
                            winner = curPlayer;
                            break;
                        }
                    } else if (command == "build-road") {
                        int n;
                        iss >> n;
                        g->buildRoad(n);
                    } else if (command == "improve") {
                        int n;
                        iss >> n;
                        g->improveRes(n);
                        if (g->checkForWin()) {
                            endGame = true;
                            commandPhase = false;
                            winner = curPlayer;
                            break;
                        }
                    } else if (command == "trade") {
                        int n;
                        std::string give, take, colour = "";
                        iss >> colour >> give >> take;
                        if (colour == "blue" || colour == "Blue") n = 0;
                        else if (colour == "red" || colour == "Red") n = 1;
                        else if (colour == "orange" || colour == "Orange") n = 2;
                        else if (colour == "yellow" || colour == "Yellow") n = 3;
                        else {
                            std::istringstream iss2{colour};
                            iss2 >> n;
                        }
                        g->trade(n, give, take);
                    } else if (command == "next") {
                        g->next();
                        commandPhase = false;
                        rollPhase = true;
                        break;
                    } else if (command == "save") {
                        iss >> command;
                        g->save(command);
                        endGame = true;
                        commandPhase = false;
                        break;
                    } else if (command == "help") {
                        g->help();
                    } else if (command == "iwin") { // testing, remove this before showcase
                        endGame = true;
                        commandPhase = false;
                        winner = curPlayer;
                        break;
                    } else std::cout << "Invalid command." << std::endl;
                    std::cout << "> ";
                }
                if (rollPhase) break;
            }
            if (endGame && winner) std::cout << "Player " << winner->getPlayerString() << " wins!" <<std::endl;
        }
        if (std::cin.eof()) exitProgram = true;
        else {
            std::cout << "Would you like to play again?\n> ";
            std::string response = "";
            while (std::cin >> response) {
                if (response == "yes" || response == "Yes") {
                    delete g;
                    winner = nullptr;
                    for (auto *p : players) p->reset();
                    break;
                } else if (response == "no" || response == "No") {
                    exitProgram = true;
                    for (auto *p : players) delete p;
                    break;
                } else std::cout << "Enter either yes or no.\n> ";
            }
        }
    }
}
