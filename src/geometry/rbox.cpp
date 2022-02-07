#include "rbox.h"

void RBox::updateBuffer(jhr::Skip_List<RBox> &boxes) {
  // Updates the amount of boxes that will be rendered
  BoxRenderer::amount_ = boxes.length();

  // List of all the model matrices
  std::unique_ptr<glm::mat4[]> modelMatrices{new glm::mat4[boxes.length()]};

  for (size_t i = 0; i < boxes.length(); i++) {
    modelMatrices[i] = boxes[i].modelMatrix();
  }

  glBindBuffer(GL_ARRAY_BUFFER, BoxRenderer::buffer_model_);
  glBufferData(GL_ARRAY_BUFFER, boxes.length() * sizeof(glm::mat4),
               &modelMatrices[0], GL_DYNAMIC_DRAW);

  for (unsigned int i = 0; i < boxes.length(); i++) {
    unsigned int VAO = boxes[i].renderer_.VAO_;

    glBindVertexArray(VAO);
    // set attribute pointers for matrix (4 times vec4)
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                          (void *)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                          (void *)(sizeof(glm::vec4)));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                          (void *)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                          (void *)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);
  }
}
