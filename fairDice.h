#ifndef __FAIRDICE_H__
#define __FAIRDICE_H__
#include "dice.h"

class FairDice: public Dice {
    int seed = 0;
    bool hasSeed = false;
  public:
    explicit FairDice(unsigned int seed);
    FairDice();
    int roll() override;
};

#endif
