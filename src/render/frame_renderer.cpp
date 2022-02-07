#include "frame_renderer.h"

const float FrameRenderer::vertices[]{0.0F, 0.0F, 0.0F,   // 0
                                      0.0F, 0.0F, 1.0F,   // 1
                                      0.0F, 1.0F, 0.0F,   // 2
                                      0.0F, 1.0F, 1.0F,   // 3
                                      1.0F, 0.0F, 0.0F,   // 4
                                      1.0F, 0.0F, 1.0F,   // 5
                                      1.0F, 1.0F, 0.0F,   // 6
                                      1.0F, 1.0F, 1.0F};  // 7

const unsigned int FrameRenderer::indices[]{0, 1,   //
                                            0, 2,   //
                                            0, 4,   //
                                            6, 4,   // s
                                            6, 7,   //
                                            6, 2,   //
                                            5, 1,   //
                                            5, 7,   //
                                            5, 4,   //
                                            3, 1,   //
                                            3, 7,   //
                                            3, 2};  //

Shader *FrameRenderer::shader = nullptr;

FrameRenderer::FrameRenderer() {
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

void FrameRenderer::init() {
  // TODO: delete when done using it
  if (shader == nullptr)
    FrameRenderer::shader = new Shader(
        "res/shaders/line/vertex.glsl",
        "res/shaders/line/fragment.glsl");  // generates the shader
}

// Called before any draw in a given frame
void FrameRenderer::preDraw() { FrameRenderer::shader->use(); }

void FrameRenderer::draw() const {
  glBindVertexArray(VAO_);
  glLineWidth(2.0F);
  glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
  glLineWidth(1.0F);
  glBindVertexArray(0);
}
