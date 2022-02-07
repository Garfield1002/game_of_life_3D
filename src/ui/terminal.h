/* This class is a singleton for no particular reason.
 * It can be a singleton and I wanted to try it out.
 */
#ifndef TERMINAL_H
#define TERMINAL_H

#include <GLFW/glfw3.h>

#include <regex>

#include "../player.h"
#include "../render/font_renderer.h"
#include "../render/renderer.h"
#include "../world.h"

class Terminal : public Renderer {
 public:
  Shader* shader;

  /*
   * This method needs to be called first to create an instance
   */
  static void createInstance(GLFWwindow* window,
                             std::shared_ptr<FontRenderer> fr,
                             std::shared_ptr<World> world,
                             const Player const& player);

  /*
   * This method needs to be called after createInstance has been called once
   */
  static Terminal& getInstance();

  bool getVisible();
  void setVisible(bool newVisible);

  void draw() const;

  std::string command{"/"};

 private:
  Terminal(GLFWwindow* window, std::shared_ptr<FontRenderer> fr,
           std::shared_ptr<World> world, const Player const& player);
  ~Terminal();

  static void character_callback(GLFWwindow* window, unsigned int codepoint);
  static void key_callback(GLFWwindow* window, int key, int scancode,
                           int action, int mods);

  const Player const& player_;

  GLFWwindow* window_;
  std::shared_ptr<FontRenderer> fr_;
  std::shared_ptr<World> world_;

  bool visible{false};
  glm::mat4 projection;

  static Terminal* instance;
  static const float vertices[];
  static const unsigned int indices[];
};
#endif  // !TERMINAL_H
