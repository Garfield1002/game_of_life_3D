#include "world.h"

World::World()
    : boxes_{RBox(0, 0, 0), RBox(1, 0, 0), RBox(2, 0, 0),
             RBox(2, 2, 2), RBox(2, 0, 2), RBox(10, 0, 0)} {
  RBox::updateBuffer(boxes_);
}

World::~World() {}

void World::setRunning(bool b) {
  if (b) last_time_ = glfwGetTime();
  running_ = b;
}

void World::tick() {
  if (!running_) return;

  float current_time = glfwGetTime();

  if (current_time - last_time_ < tick_time_) return;
  std::cout << "LOG:WORLD: Ticking" << std::endl;

  last_time_ = current_time;

  // Game of life logic goes here

  /*
   * Life wxyz is the rule set in which an alive cell will stay alive in the
   * next generation if it has n live neighbors and w <= n <= x and a dead cell
   * will become alive in the next generation if y <= n <= z.
   */

  // Adds all the neighbours to the "interesting blocks"
  RBoxes interesting{boxes_};

  for (RBox &box : boxes_)
    for (int x = -1; x <= 1; ++x)
      for (int y = -1; y <= 1; ++y)
        for (int z = -1; z <= 1; ++z) {
          glm::vec3 direction(x, y, z);
          if (direction == glm::vec3(0, 0, 0)) continue;
          RBox new_box = RBox(box.origin_ + direction, false);
          RBox &neighbour = interesting.findOrCreate(new_box);
          neighbour.neighbours_ += 1;
        }

  RBoxes boxes = RBoxes{};

  for (RBox &box : interesting) {
    if ((box.alive_ && rule.w <= box.neighbours_ &&
         box.neighbours_ <= rule.x) ||
        (!box.alive_ && rule.y <= box.neighbours_ &&
         box.neighbours_ <= rule.z)) {
      box.neighbours_ = 0;
      box.alive_ = true;
      boxes.insert(box);
    }
  }

  boxes_ = boxes;
  RBox::updateBuffer(boxes_);
}
