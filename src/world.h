#ifndef WORLD_H
#define WORLD_H

#include <GLFW/glfw3.h>
#include <jhr_skip_list.h>

#include "geometry/rbox.h"

typedef jhr::Skip_List<RBox> RBoxes;

struct Rule {
  int w, x, y, z;
};

class World {
 private:
  bool running_{false};
  float last_time_{};

 public:
  World();
  ~World();

  // make private
  RBoxes boxes_, initial_state;

  void setRunning(bool b);

  float tick_time_{1.0F};

  Rule rule{4, 5, 5, 5};

  void tick();
};
#endif  // !WORLD_H
