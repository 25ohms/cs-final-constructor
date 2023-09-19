#ifndef __DICE_H__
#define __DICE_H__

class Dice {
  public:
    virtual int roll();
    virtual ~Dice() = default;
};

#endif
