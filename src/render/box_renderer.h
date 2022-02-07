#ifndef BOX_RENDERER_H
#define BOX_RENDERER_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <vector>

#include "../geometry/box.h"
#include "../shader/shader.h"
#include "renderer.h"

class BoxRenderer : virtual public Renderer {
 public:
  static Shader *shader;

  BoxRenderer();

  friend class RBox;

  static void init();
  static void preDraw();
  void draw() const;

 private:
  static const float vertices[];
  static const unsigned int indices[];
  static const glm::vec3 color, light_color, light_pos;
  static GLuint buffer_model_;
  static unsigned int amount_;

  // GLuint lightVAO_;
};

#endif  // !BOX_RENDERER_H