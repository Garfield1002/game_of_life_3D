#ifndef RENDERER_H
#define RENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../shader/shader.h"

class Renderer {
 public:
  virtual ~Renderer() {
    glDeleteVertexArrays(1, &VAO_);
    glDeleteBuffers(1, &VBO_);
    glDeleteBuffers(1, &EBO_);
  }

  virtual void draw() const = 0;

 protected:
  GLuint VAO_, EBO_, VBO_;
};

#endif  // !RENDERER_H
