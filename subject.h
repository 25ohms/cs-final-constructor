#ifndef _SUBJECT_H_
#define _SUBJECT_H_
#include <vector>
#include <string>
#include "observer.h"

class Subject {
  protected:
    std::string type;
    std::vector<Observer*> observers;
  public:
    Subject();
    void attach(Observer *o);
    void detach(Observer *o);
    virtual void notifyObservers();
    virtual ~Subject() = 0;
};

#endif
