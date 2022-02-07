#ifndef BOX_H
#define BOX_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

enum class Face { UP, DOWN, NORTH, SOUTH, EAST, WEST };

class Box {
 public:
  Box(glm::vec3 origin, glm::vec3 size);
  Box(glm::vec3 origin);
  Box(float x, float y, float z);

  glm::mat4 modelMatrix() const;

  // The origin of the cube. That is, the node of the cube with min(x), min(y)
  // and min(z)
  const glm::vec3 origin_, size_;
};

#endif  // !BOX_H
