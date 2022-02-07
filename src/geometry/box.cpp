#include "box.h"

Box::Box(glm::vec3 origin, glm::vec3 size) : origin_{origin}, size_{size} {}

Box::Box(glm::vec3 origin) : origin_{origin}, size_{1} {}

Box::Box(float x, float y, float z) : origin_{glm::vec3(x, y, z)}, size_{1} {}

glm::mat4 Box::modelMatrix() const {
  return glm::translate(glm::mat4(1.0F), origin_);
}
