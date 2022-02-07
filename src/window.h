#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <tuple>

#include "geometry/box.h"
#include "player.h"
#include "render/crosshairs.h"
#include "render/debug_crosshairs.h"
#include "render/font_renderer.h"
#include "settings.h"
#include "util/raycasting.h"
#include "world.h"
#include "ui/terminal.h"

class Window {
 public:
  Window();
  ~Window();

  void run();

 private:
  // Prints the error in the console
  static void ErrorCallback(int error, const char *description);
  static void FramebufferSizeCallback(GLFWwindow *window, int width,
                                      int height);
  static void CursorPosCallback(GLFWwindow *window, double xpos, double ypos);
  static void MouseButtonCallback(GLFWwindow *window, int button, int action,
                                  int mods);

  // Resizes the viewport
  void FramebufferSizeCallbackHelper(int width, int height);
  void CursorPosCallbackHelper(double xpos, double ypos);
  void MouseButtonCallbackHelper(int button, int action, int mods);

  void DisplayDebugMenu() const;

  void f3Input(int new_state);

  void ProcessInputs();

  int width_{Settings::SCR_WIDTH}, height_{Settings::SCR_HEIGHT};
  GLFWwindow *window_;
  bool debug_{false};

  Player player_;

  std::tuple<RBox const *, Face> boxFace_;

  float lastX = Settings::SCR_WIDTH / 2.0f;
  float lastY = Settings::SCR_HEIGHT / 2.0f;
  bool firstMouse = true;

  std::shared_ptr<World> world_ = nullptr;
  std::unique_ptr<DebugCrosshairs> dch_ = nullptr;
  std::unique_ptr<Crosshairs> ch_ = nullptr;
  std::shared_ptr<FontRenderer> ft_ = nullptr; // shared with the terminal

  // time between current frame and last frame
  float deltaTime_{0.0f};
  float lastFrame_{0.0f};
};

#endif  // !WINDOW_H
