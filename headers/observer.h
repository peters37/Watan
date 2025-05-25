#include <iostream>

class Observer {
  public:
    virtual void notify(std::ostream &out) = 0;
    virtual ~Observer() = default;
};
