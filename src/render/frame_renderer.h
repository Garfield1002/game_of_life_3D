#ifndef FRAME_RENDERER_H
#define FRAME_RENDERER_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <vector>

#include "../geometry/box.h"
#include "../shader/shader.h"
#include "renderer.h"

class FrameRenderer : virtual public Renderer {
 public:
  static Shader *shader;

  FrameRenderer();

  static void init();
  static void preDraw();
  void draw() const;

 private:
  static const float vertices[];
  static const unsigned int indices[];
};

#endif  // !FRAME_RENDERER_H