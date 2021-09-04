#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

#include <glm/glm.hpp>

namespace {
void errorCallback(int error, const char* description) {
  fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

GLFWwindow* initialize() {
  int glfwInitRes = glfwInit();
  if (!glfwInitRes) {
    fprintf(stderr, "Unable to initialize GLFW\n");
    return nullptr;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Change window name =======================================================
  GLFWwindow* window = glfwCreateWindow(960, 540, "Template", nullptr, nullptr);
  if (!window) {
    fprintf(stderr, "Unable to create GLFW window\n");
    glfwTerminate();
    return nullptr;
  }

  glfwMakeContextCurrent(window);

  int gladInitRes = gladLoadGL();
  if (!gladInitRes) {
    fprintf(stderr, "Unable to initialize glad\n");
    glfwDestroyWindow(window);
    glfwTerminate();
    return nullptr;
  }

  return window;
}
}  // namespace

int main() {
  glfwSetErrorCallback(::errorCallback);

  glm::vec3 test{};

  GLFWwindow* window = ::initialize();
  if (!window) {
    exit(EXIT_FAILURE);
  }

  glClearColor(0.36f, 0.52f, 0.69f, 1.0f);

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  exit(EXIT_SUCCESS);
}