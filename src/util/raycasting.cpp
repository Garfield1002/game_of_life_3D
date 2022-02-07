#include "raycasting.h"

RayCast::RayCast(const glm::vec3 &origin, const glm::vec3 &direction,
                 float distance)
    : origin_{origin}, direction_{direction}, distance_{distance} {}

RayCast::~RayCast() {}

// Returns true if number is between a and b
inline bool RayCast::between(const float number, const float a, const float b) {
  return fmin(a, b) < number && number < fmax(a, b);
}

float RayCast::distance2(const glm::vec3 &a, const glm::vec3 &b) {
  glm::vec3 c = (a - b) * (a - b);
  return c.x + c.y + c.z;
}

std::unique_ptr<const glm::vec3> RayCast::AARect(const glm::vec3 &a,
                                                 const glm::vec3 &b,
                                                 size_t axis) const {
  // a and b need to be on the same "axis" plane
  if (a[axis] != b[axis]) {
    return nullptr;
  }

  // avoid division by 0
  if (direction_[axis] == 0) {
    return nullptr;
  }

  // find intersection of ray with plane
  float t{(a[axis] - origin_[axis]) / direction_[axis]};
  glm::vec3 intersection = origin_ + direction_ * t;

  intersection[axis] = a[axis];

  size_t axis_1{(axis + 1) % 3}, axis_2{(axis + 2) % 3};

  if (between(intersection[axis_1], a[axis_1], b[axis_1]) &&
      between(intersection[axis_2], a[axis_2], b[axis_2])) {
    return std::make_unique<glm::vec3>(intersection);
  }

  return nullptr;
}

std::unique_ptr<const glm::vec3> RayCast::IntersectsCube(
    const glm::vec3 &origin, float size) const {
  // only test the visible faces
  std::unique_ptr<glm::vec3[]> test_coordinates{new glm::vec3[6]};

  if (direction_.x > 0) {
    test_coordinates[0] = glm::vec3(0, 0, 0);
    test_coordinates[1] = glm::vec3(0, 1, 1);
  } else {
    test_coordinates[0] = glm::vec3(1, 0, 0);
    test_coordinates[1] = glm::vec3(1, 1, 1);
  }

  if (direction_.y > 0) {
    test_coordinates[2] = glm::vec3(0, 0, 0);
    test_coordinates[3] = glm::vec3(1, 0, 1);
  } else {
    test_coordinates[2] = glm::vec3(0, 1, 0);
    test_coordinates[3] = glm::vec3(1, 1, 1);
  }

  if (direction_.z > 0) {
    test_coordinates[4] = glm::vec3(0, 0, 0);
    test_coordinates[5] = glm::vec3(1, 1, 0);
  } else {
    test_coordinates[4] = glm::vec3(0, 0, 1);
    test_coordinates[5] = glm::vec3(1, 1, 1);
  }

  std::unique_ptr<const glm::vec3> min_intersection = nullptr;
  float min_distance = distance_ * distance_;

  for (size_t i = 0; i < 6; i += 2) {
    std::unique_ptr<const glm::vec3> intersection = AARect(
        origin + test_coordinates[i], origin + test_coordinates[i + 1], i / 2);

    if (intersection != nullptr) {
      if (distance2(*intersection, origin_) < min_distance) {
        min_distance = distance2(*intersection, origin_);
        min_intersection = std::move(intersection);
      }
    }
  }

  return min_intersection;
}

BoxFace RayCast::IntersectsBoxes(jhr::Skip_List<RBox> &boxes) const {
  std::unique_ptr<const glm::vec3> min_intersection = nullptr;
  Face face = Face::UP;
  float min_distance = distance_ * distance_;
  RBox const *min_box = nullptr;

  for (size_t i = 0; i < boxes.length(); i++) {
    std::unique_ptr<const glm::vec3> intersection =
        IntersectsCube(boxes[i].origin_, 1.0F);

    if (intersection != nullptr) {
      if (distance2(*intersection, origin_) < min_distance) {
        min_distance = distance2(*intersection, origin_);
        min_intersection = std::move(intersection);
        min_box = &boxes[i];

        if (min_intersection->x == min_box->origin_.x)
          face = Face::SOUTH;
        else if (min_intersection->x == min_box->origin_.x + 1.0F)
          face = Face::NORTH;
        else if (min_intersection->y == min_box->origin_.y)
          face = Face::DOWN;
        else if (min_intersection->y == min_box->origin_.y + 1.0F)
          face = Face::UP;
        else if (min_intersection->z == min_box->origin_.z)
          face = Face::WEST;
        else if (min_intersection->z == min_box->origin_.z + 1.0F)
          face = Face::EAST;
        else {
          std::cout << min_intersection->x << ',' << min_intersection->y << ','
                    << min_intersection->z << std::endl;
          std::cout << min_box->origin_.x << ',' << min_box->origin_.y << ','
                    << min_box->origin_.z
                    << std::endl;
        }
      }
    }
  }

  return std::tuple<RBox const *, Face>{min_box, face};
}
