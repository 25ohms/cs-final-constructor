#include "subject.h"
#include <algorithm>
#include <iostream>

void Subject::attach (Observer *o) { observers.emplace_back(o); }
void Subject::detach (Observer *o) {
    auto it = std::find(observers.begin(), observers.end(), o);
    if (it != observers.end()) observers.erase(it);
}

void Subject::notifyObservers () {
    for (auto &o : observers) {
        o->notify(type);
    }
}
Subject::Subject() {};
Subject::~Subject() {};
