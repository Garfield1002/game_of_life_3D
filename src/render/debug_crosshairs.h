#ifndef DEBUG_CROSSHAIRS_H
#define DEBUG_CROSSHAIRS_H

#include "../camera/camera.h"
#include "../shader/shader.h"
#include "renderer.h"

class DebugCrosshairs : public Renderer {
 public:
  Shader *shader;

  Camera &camera;

  DebugCrosshairs(Camera &camera);

  DebugCrosshairs(const DebugCrosshairs &other);

  void draw() const;

 private:
  static const float vertices[];
  static const unsigned int indices[];
};

#endif  // !DEBUG_CROSSHAIRS_H
