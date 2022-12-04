#ifndef __OBSERVER_H__
#define __OBSERVER_H__

class Observer {
 public: 
  virtual void update() = 0;
  virtual ~Observer() = default;
};

#endif