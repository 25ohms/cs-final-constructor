#ifndef __LOADEDDICE_H__
#define __LOADEDDICE_H__
#include "dice.h"

class LoadedDice: public Dice {
    int roll() override;
};

#endif
