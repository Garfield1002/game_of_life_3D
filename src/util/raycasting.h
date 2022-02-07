#ifndef RAYCASTING_H
#define RAYCASTING_H

// Returns the list of axis aligned boxes on a segment
#include <jhr_skip_list.h>

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <memory>
#include <tuple>

#include "../geometry/box.h"
#include "../geometry/rbox.h"

typedef std::tuple<RBox const *, Face> BoxFace;

class RayCast {
 public:
  RayCast(const glm::vec3 &origin, const glm::vec3 &direction, float distance);
  ~RayCast();

  std::unique_ptr<const glm::vec3> AARect(const glm::vec3 &a,
                                          const glm::vec3 &b,
                                          size_t axis) const;

  std::unique_ptr<const glm::vec3> IntersectsCube(const glm::vec3 &origin,
                                                  float size) const;
  std::tuple<RBox const *, Face> IntersectsBoxes(
      jhr::Skip_List<RBox> &boxes) const;

 private:
  const glm::vec3 origin_;
  const glm::vec3 direction_;
  const float distance_;

  static inline bool between(const float number, const float a, const float b);
  static float distance2(const glm::vec3 &a, const glm::vec3 &b);
};
#endif
