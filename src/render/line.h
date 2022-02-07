// Draw a fucking line in opengl
#ifndef LINE_H
#define LINE_H

#include "../shader/shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class Line {
public:
  Line(std::vector<float> &vertices);

  ~Line();

  inline void set_mvp(glm::mat4 mvp) { mvp_ = mvp; };

  void draw();

private:
  std::vector<float> vertices_;
  glm::vec3 line_color_;
  glm::mat4 mvp_ = glm::mat4(1);

  GLuint VAO_, VBO_;

  Shader shader_;
};

#endif // !LINE_H