#ifndef _OBSERVER_H_
#define _OBSERVER_H_
#include <string>

class Observer {
  public:
    virtual void notify(std::string type) = 0;
    virtual ~Observer();
};

#endif
