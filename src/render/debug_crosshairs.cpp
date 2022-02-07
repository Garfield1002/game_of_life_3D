#include "debug_crosshairs.h"

const float DebugCrosshairs::vertices[]{
    0.07f, 0.0f,  0.0f,  0.0f, 0.0f, 0.0f,

    0.0f,  0.07f, 0.0f,  0.0f, 0.0f, 0.0f,

    0.0f,  0.0f,  0.07f, 0.0f, 0.0f, 0.0f,
};

const unsigned int DebugCrosshairs::indices[]{0, 1, 2, 3, 4, 5};

DebugCrosshairs::DebugCrosshairs(Camera &camera) : camera{camera} {
  shader = new Shader("res/shaders/default/vertex.glsl",
                      "res/shaders/default/fragment.glsl");
  glGenVertexArrays(1, &VAO_);
  glGenBuffers(1, &VBO_);
  glGenBuffers(1, &EBO_);

  glBindVertexArray(VAO_);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0,         // the location
                        3,         // size of the vertex attribute vec3 => 3
                        GL_FLOAT,  // type of the data
                        GL_FALSE,  // don't normalize
                        0,         // stride
                        (void *)0  // offset
  );
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

DebugCrosshairs::DebugCrosshairs(const DebugCrosshairs &other)
    : camera{other.camera} {
  shader = new Shader("res/shaders/default/vertex.glsl",
                      "res/shaders/default/fragment.glsl");
  glGenVertexArrays(1, &VAO_);
  glGenBuffers(1, &VBO_);
  glGenBuffers(1, &EBO_);

  glBindVertexArray(VAO_);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0,         // the location
                        3,         // size of the vertex attribute vec3 => 3
                        GL_FLOAT,  // type of the data
                        GL_FALSE,  // don't normalize
                        0,         // stride
                        (void *)0  // offset
  );
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}


void DebugCrosshairs::draw() const {
  glDisable(GL_DEPTH_TEST);
  shader->use();
  glBindVertexArray(VAO_);

  shader->set_mat4("projection", camera.projection);
  shader->set_mat4("view", camera.view());
  shader->set_mat4("model", glm::translate(glm::mat4(1.0F),
                                           camera.position_ + camera.front_));

  glLineWidth(3.0F);
  shader->set_vec4("color", glm::vec4(1.0F, 0.0F, 0.0F, 1.0F));
  glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);

  shader->set_vec4("color", glm::vec4(0.0F, 1.0F, 0.0F, 1.0F));
  glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT,
                 (void *)(2 * sizeof(unsigned int)));

  shader->set_vec4("color", glm::vec4(0.0F, 0.0F, 1.0F, 1.0F));
  glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT,
                 (void *)(4 * sizeof(unsigned int)));

  glLineWidth(1.0F);
  glBindVertexArray(0);
  glEnable(GL_DEPTH_TEST);
}
