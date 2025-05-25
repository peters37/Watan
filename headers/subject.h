#include "observer.h"
#include <vector>
#include <iostream>

class Subject {
  protected:
    std::vector<Observer *> observers;
  public:
    void attach(Observer *ob);
    void detach(Observer *ob);
    void notifyObservers(std::ostream &out);
    virtual ~Subject() = 0;
};
