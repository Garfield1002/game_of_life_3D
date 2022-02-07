#include "box_renderer.h"

const float BoxRenderer::vertices[]{
    0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  -1.0f,  //
    1.0f, 0.0f, 0.0f, 0.0f,  0.0f,  -1.0f,  //
    1.0f, 1.0f, 0.0f, 0.0f,  0.0f,  -1.0f,  //
    1.0f, 1.0f, 0.0f, 0.0f,  0.0f,  -1.0f,  //
    0.0f, 1.0f, 0.0f, 0.0f,  0.0f,  -1.0f,  //
    0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  -1.0f,  //

    0.0f, 0.0f, 1.0f, 0.0f,  0.0f,  1.0f,  //
    1.0f, 0.0f, 1.0f, 0.0f,  0.0f,  1.0f,  //
    1.0f, 1.0f, 1.0f, 0.0f,  0.0f,  1.0f,  //
    1.0f, 1.0f, 1.0f, 0.0f,  0.0f,  1.0f,  //
    0.0f, 1.0f, 1.0f, 0.0f,  0.0f,  1.0f,  //
    0.0f, 0.0f, 1.0f, 0.0f,  0.0f,  1.0f,  //

    0.0f, 1.0f, 1.0f, -1.0f, 0.0f,  0.0f,  //
    0.0f, 1.0f, 0.0f, -1.0f, 0.0f,  0.0f,  //
    0.0f, 0.0f, 0.0f, -1.0f, 0.0f,  0.0f,  //
    0.0f, 0.0f, 0.0f, -1.0f, 0.0f,  0.0f,  //
    0.0f, 0.0f, 1.0f, -1.0f, 0.0f,  0.0f,  //
    0.0f, 1.0f, 1.0f, -1.0f, 0.0f,  0.0f,  //

    1.0f, 1.0f, 1.0f, 1.0f,  0.0f,  0.0f,  //
    1.0f, 1.0f, 0.0f, 1.0f,  0.0f,  0.0f,  //
    1.0f, 0.0f, 0.0f, 1.0f,  0.0f,  0.0f,  //
    1.0f, 0.0f, 0.0f, 1.0f,  0.0f,  0.0f,  //
    1.0f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f,  //
    1.0f, 1.0f, 1.0f, 1.0f,  0.0f,  0.0f,  //

    0.0f, 0.0f, 0.0f, 0.0f,  -1.0f, 0.0f,  //
    1.0f, 0.0f, 0.0f, 0.0f,  -1.0f, 0.0f,  //
    1.0f, 0.0f, 1.0f, 0.0f,  -1.0f, 0.0f,  //
    1.0f, 0.0f, 1.0f, 0.0f,  -1.0f, 0.0f,  //
    0.0f, 0.0f, 1.0f, 0.0f,  -1.0f, 0.0f,  //
    0.0f, 0.0f, 0.0f, 0.0f,  -1.0f, 0.0f,  //

    0.0f, 1.0f, 0.0f, 0.0f,  1.0f,  0.0f,  //
    1.0f, 1.0f, 0.0f, 0.0f,  1.0f,  0.0f,  //
    1.0f, 1.0f, 1.0f, 0.0f,  1.0f,  0.0f,  //
    1.0f, 1.0f, 1.0f, 0.0f,  1.0f,  0.0f,  //
    0.0f, 1.0f, 1.0f, 0.0f,  1.0f,  0.0f,  //
    0.0f, 1.0f, 0.0f, 0.0f,  1.0f,  0.0f
};
    
    
/* 0.0F, 0.0F, 0.0F,   // 0
                                    0.0F, 0.0F, 1.0F,   // 1
                                    0.0F, 1.0F, 0.0F,   // 2
                                    0.0F, 1.0F, 1.0F,   // 3
                                    1.0F, 0.0F, 0.0F,   // 4
                                    1.0F, 0.0F, 1.0F,   // 5
                                    1.0F, 1.0F, 0.0F,   // 6
                                    1.0F, 1.0F, 1.0F,   //7
                                    //Normals
                                    0.0F, 0.0F, -1.0F,  // 8
                                    0.0F, 0.0F, 1.0F,   // 9
                                    -1.0F, 0.0F, 0.0F,  // 10
                                    1.0F, 0.0F, 0.0F,   // 11
                                    0.0F, -1.0F, 0.0F,  // 12
                                    0.0F, 1.0F, 0.0F,   // 13
};*/

const unsigned int BoxRenderer::indices[]{
    0, 1, 3,  // 10
    3, 2, 0,  //
    //
    0, 4, 5,  // 12
    5, 1, 0,  //
    //
    6, 7, 5,  // 11
    5, 4, 6,  //
    //
    6, 2, 3,  // 13
    3, 7, 6,  //
    //
    6, 4, 0,  // 8
    0, 2, 6,  //
    //
    1, 5, 7,  // 9
    7, 3, 1,  //
};

const glm::vec3 BoxRenderer::color = glm::vec3(1.0f, 0.847f, 0.003f);
const glm::vec3 BoxRenderer::light_color = glm::vec3(1.0f, 1.0f, 1.0f);
const glm::vec3 BoxRenderer::light_pos = glm::vec3(64.0f, 150.0f,-33.0f);

GLuint BoxRenderer::buffer_model_ = 0;
unsigned int BoxRenderer::amount_ = 0;
Shader *BoxRenderer::shader = nullptr;

BoxRenderer::BoxRenderer() {
  glGenVertexArrays(1, &VAO_);
  //glGenVertexArrays(1, &lightVAO_);
  glGenBuffers(1, &VBO_);
  //glGenBuffers(1, &EBO_);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(VAO_);
  //glBindVertexArray(lightVAO_);

  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
  //              GL_STATIC_DRAW);

  glVertexAttribPointer(0,         // the location
                        3,         // size of the vertex attribute vec3 => 3
                        GL_FLOAT,  // type of the data
                        GL_FALSE,  // don't normalize
                        6 * sizeof(float),  // stride
                        (void *)0  // offset
  );
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        6 * sizeof(float),
                        (void *)(3 * sizeof(float))
  );
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void BoxRenderer::init() {
  glGenBuffers(1, &buffer_model_);

  // TODO: delete when done using it
  if (shader == nullptr)
    BoxRenderer::shader = new Shader(
        "res/shaders/instance/vertex.glsl",
        "res/shaders/instance/fragment.glsl");  // generates the shader
}

// Called before any draw in a given frame
void BoxRenderer::preDraw() {
    BoxRenderer::shader->use(); 
    BoxRenderer::shader->set_vec3("objectColor", BoxRenderer::color);
    BoxRenderer::shader->set_vec3("lightColor", BoxRenderer::light_color);
    BoxRenderer::shader->set_vec3("lightPos", BoxRenderer::light_pos);
}

void BoxRenderer::draw() const {
  glBindVertexArray(VAO_);
  // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  glDrawArraysInstanced(GL_TRIANGLES, 0, 36, amount_);
  //glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, amount_);
  glBindVertexArray(0);
}
