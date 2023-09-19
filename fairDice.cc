#include <algorithm>
#include <random>
#include <chrono>
#include <iostream>
#include "fairDice.h"

int FairDice::roll() {
    std::vector<int> nums {1, 2, 3, 4, 5, 6};
    int n1, n2;
    unsigned newSeed;
    hasSeed ? newSeed = seed : newSeed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng{newSeed};
    std::shuffle(nums.begin(), nums.end(), rng);
    n1 = nums[0];
    std::shuffle(nums.begin(), nums.end(), rng);
    n2 = nums[0];
    std::cout << "You rolled: " << n1 + n2 << std::endl;
    return n1 + n2;
}

FairDice::FairDice(unsigned int seed): Dice{}, seed{seed}, hasSeed{true} {}

FairDice::FairDice(): Dice{}, seed{0}, hasSeed{false} {}
