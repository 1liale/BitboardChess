#ifndef __SUBJECT_H__
#define __SUBJECT_H__
#include <vector>

class Observer;

class Subject {
std::vector<Observer *> observers;

public: 
  virtual ~Subject() = default;
  void attach( Observer *o );
  void detach( Observer *o );

protected:
  void updateObservers();
};

#endif