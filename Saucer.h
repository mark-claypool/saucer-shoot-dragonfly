//
// Saucer.h
//
 
#include "Object.h"
#include "EventCollision.h"
 
class Saucer : public df::Object {
 
 private:
  void moveToStart();
  void out();
  void hit(const df::EventCollision *p_collision_event);

 public:
  Saucer();
  ~Saucer();
  int eventHandler(const df::Event *p_e);
};
