#include "line.h"

Line::Line(std::vector<float> &vertices2)
    : shader_("res/shaders/line/vertex.glsl", "res/shaders/line/fragment.glsl"),
      vertices_{vertices2} {

  glLineWidth(2.0f);
  line_color_ = glm::vec3(/*A point*/
                          0.0f);

  float vertices[] = {1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1,

                      1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0,

                      0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1,

                      0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1};

  glGenVertexArrays(1, &VAO_);
  glGenBuffers(1, &VBO_);
  glBindVertexArray(VAO_);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

Line::~Line() {
  glDeleteVertexArrays(1, &VAO_);
  glDeleteBuffers(1, &VBO_);
}

void Line::draw() {
  shader_.use();
  shader_.set_mat4("MVP", mvp_);
  shader_.set_vec3("color", line_color_);

  glBindVertexArray(VAO_);
  glDrawArrays(GL_LINES, 0, 24);
}
