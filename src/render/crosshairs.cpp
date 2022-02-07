#include "crosshairs.h"

const float Crosshairs::vertices[]{
    0.0f,
    -40.0f / 1080.0f,
    0.0f,  //
    0.0f,
    40.0f / 1080.0f,
    0.0f,  //
    -40.0f / 1920.0f,
    0.0f,
    0.0f,  //
    40.0f / 1920.0f,
    0.0f,
    0.0f,  //

    3.0f / 1920.0f,
    -37.0f / 1080.0f,
    0.0f,  //
    3.0f / 1920.0f,
    43.0f / 1080.0f,
    0.0f,  //
    -37.0f / 1920.0f,
    3.0f / 1080.0f,
    0.0f,  //
    43.0f / 1920.0f,
    3.0f / 1080.0f,
    0.0f  //
};

const unsigned int Crosshairs::indices[]{0, 1, 2, 3, 4, 5, 6, 7};

Crosshairs::Crosshairs() {
  shader = new Shader("res/shaders/crosshairs/vertex.glsl",
                      "res/shaders/crosshairs/fragment.glsl");
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

void Crosshairs::draw() const {
  shader->use();
  glBindVertexArray(VAO_);

  glLineWidth(3.0F);
  shader->set_vec3("color", glm::vec3(1.0F, 1.0F, 1.0F));
  glDrawElements(GL_LINES, 4, GL_UNSIGNED_INT, 0);

  shader->set_vec3("color", glm::vec3(0.0F, 0.0F, 0.0F));
  glDrawElements(GL_LINES, 4, GL_UNSIGNED_INT,
                 (void *)(4 * sizeof(unsigned int)));

  glLineWidth(1.0F);
  glBindVertexArray(0);
}
