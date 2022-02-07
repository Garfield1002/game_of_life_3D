#ifndef CROSSHAIRS_H
#define CROSSHAIRS_H

#include "renderer.h"
#include "../shader/shader.h"

class Crosshairs : public Renderer {
 public:
  Shader *shader;

  Crosshairs();

  void draw() const;

 private:
  static const float vertices[];
  static const unsigned int indices[];
};

#endif  // !CROSSHAIRS_H
