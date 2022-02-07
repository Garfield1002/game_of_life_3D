#ifndef RBOX_H
#define RBOX_H

#include <jhr_skip_list.h>

#include "../render/box_renderer.h"
#include "../render/frame_renderer.h"
#include "box.h"

class RBox : public Box {
 public:
  // GAME OF LIFE SPECIFIC
  bool alive_{true};
  int neighbours_{0};
  // =====================

  const BoxRenderer renderer_;
  const FrameRenderer frame_renderer_;
  RBox(float x, float y, float z)
      : Box(x, y, z), renderer_(), frame_renderer_(){};
  RBox(float x, float y, float z, bool alive)
      : Box(x, y, z), renderer_(), frame_renderer_(), alive_{alive} {};
  RBox(const glm::vec3 &origin) : Box(origin), renderer_(), frame_renderer_(){};
  RBox(const glm::vec3 &origin, bool alive)
      : Box(origin), renderer_(), frame_renderer_(), alive_{alive} {};
  RBox(const glm::vec3 &origin, const glm::vec3 &size)
      : Box(origin, size), renderer_(), frame_renderer_(){};

  RBox(const RBox &box)
      : Box(box.origin_, box.size_),
        renderer_(),
        frame_renderer_(),
        alive_{box.alive_},
        neighbours_{box.neighbours_} {};

  // TODO remove friend and add a key type
  friend bool operator<(const RBox &box1, const RBox &box2) {
    if (box1.origin_.x == box2.origin_.x) {
      if (box1.origin_.y == box2.origin_.y) {
        return box1.origin_.z < box2.origin_.z;
      }
      return box1.origin_.y < box2.origin_.y;
    }
    return box1.origin_.x < box2.origin_.x;
  }

  friend bool operator==(const RBox &box1, const RBox &box2) {
    return box1.origin_.x == box2.origin_.x &&
           box1.origin_.y == box2.origin_.y && box1.origin_.z == box2.origin_.z;
  }

  static void updateBuffer(jhr::Skip_List<RBox> &boxes);
};
#endif  // !RBOX_H
