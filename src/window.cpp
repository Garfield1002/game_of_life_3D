#include "window.h"

Window::Window() {
  glfwSetErrorCallback(Window::ErrorCallback);

  int glfwInitRes = glfwInit();

  if (!glfwInitRes) {
    fprintf(stderr, "Unable to initialize GLFW\n");
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window_ = glfwCreateWindow(width_, height_, Settings::NAME, nullptr, nullptr);

  if (!window_) {
    fprintf(stderr, "Unable to create GLFW window\n");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window_);

  glfwSetWindowUserPointer(window_, this);

  // Called whenever the window is resized
  glfwSetFramebufferSizeCallback(window_, Window::FramebufferSizeCallback);
  glfwSetCursorPosCallback(window_, Window::CursorPosCallback);
  glfwSetMouseButtonCallback(window_, Window::MouseButtonCallback);

  // tell GLFW to capture our mouse
  glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  int gladInitRes = gladLoadGL();
  if (!gladInitRes) {
    fprintf(stderr, "Unable to initialize glad\n");
    glfwDestroyWindow(window_);
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  if (!window_) {
    exit(EXIT_FAILURE);
  }

  // Initialize shaders
  BoxRenderer::init();
  FrameRenderer::init();

  // UI
  ft_ = std::make_shared<FontRenderer>();
  dch_ = std::make_unique<DebugCrosshairs>(DebugCrosshairs(player_.camera));
  ch_ = std::make_unique<Crosshairs>();

  // World
  world_ = std::make_shared<World>();

  // initializes the terminal
  Terminal::createInstance(window_, ft_, world_, player_);
}

Window::~Window() {
  delete FrameRenderer::shader;
  delete BoxRenderer::shader;

  glfwDestroyWindow(window_);
  glfwTerminate();
}

void Window::run() {
  while (!glfwWindowShouldClose(window_)) {
    float currentFrame = glfwGetTime();
    deltaTime_ = currentFrame - lastFrame_;
    lastFrame_ = currentFrame;

    world_->tick();

    ProcessInputs();

    glClearColor(0.0906f, 0.0953f, 0.0969f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    // test if intersecting with cube

    BoxRenderer::preDraw();

    BoxRenderer::shader->set_mat4("projection", player_.camera.projection);
    BoxRenderer::shader->set_mat4("view", player_.camera.view());

    // FIX
    if (world_->boxes_.length() > 0) world_->boxes_[0].renderer_.draw();

    // Draw a frame on the box we are targetting
    FrameRenderer::preDraw();
    FrameRenderer::shader->set_mat4("projection", player_.camera.projection);
    FrameRenderer::shader->set_mat4("view", player_.camera.view());

    RayCast raycast(player_.camera.position_, player_.camera.front_, 100.0f);

    boxFace_ = raycast.IntersectsBoxes(world_->boxes_);

    if (std::get<0>(boxFace_) != nullptr) {
      FrameRenderer::shader->set_mat4("model",
                                      std::get<0>(boxFace_)->modelMatrix());
      std::get<0>(boxFace_)->frame_renderer_.draw();
    }

    // Draws the text
    ft_->print(Settings::VERSION, 1580.0F, 1020.0F);

    // Draws the terminal
    Terminal::getInstance().draw();

    DisplayDebugMenu();

    // Draws the cursor
    if (debug_)
      dch_->draw();
    else
      ch_->draw();

    glfwSwapBuffers(window_);
    glfwPollEvents();
  }
}

void Window::ErrorCallback(int error, const char *description) {
  fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

void Window::FramebufferSizeCallbackHelper(int width, int height) {
  width_ = width;
  height_ = height;
  glViewport(0, 0, width_, height_);
}

void Window::CursorPosCallbackHelper(double xpos, double ypos) {
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset =
      lastY - ypos;  // reversed since y-coordinates go from bottom to top

  lastX = xpos;
  lastY = ypos;

  player_.camera.ProcessMouseMovement(xoffset, yoffset);
}

void Window::MouseButtonCallbackHelper(int button, int action, int mods) {
  auto [intersect_box, face] = boxFace_;
  if (intersect_box != nullptr) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
      world_->boxes_.remove(*intersect_box);
      RBox::updateBuffer(world_->boxes_);
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
      glm::vec3 origin = intersect_box->origin_;
      switch (face) {
        case Face::UP:
          origin += glm::vec3(0.0F, 1.0F, 0.0F);
          break;
        case Face::DOWN:
          origin += glm::vec3(0.0F, -1.0F, 0.0F);
          break;
        case Face::NORTH:
          origin += glm::vec3(1.0F, 0.0F, 0.0F);
          break;
        case Face::SOUTH:
          origin += glm::vec3(-1.0F, 0.0F, 0.0F);
          break;
        case Face::EAST:
          origin += glm::vec3(0.0F, 0.0F, 1.0F);
          break;
        default:
          origin += glm::vec3(0.0F, 0.0F, -1.0F);
          break;
      }
      world_->boxes_.insert(RBox(origin));
      RBox::updateBuffer(world_->boxes_);
    }
  }
}

void Window::DisplayDebugMenu() const {
  if (!debug_) return;

  static double lastTime = glfwGetTime();
  static int nbFrames = 0;
  static double fps;

  double currentTime = glfwGetTime();
  nbFrames++;
  if (currentTime - lastTime >= 0.5) {
    fps = static_cast<double>(nbFrames) / (currentTime - lastTime);
    nbFrames = 0;
    lastTime = currentTime;
  }

  std::string facing;

  if (player_.camera.front_.x * player_.camera.front_.x >
      player_.camera.front_.z * player_.camera.front_.z)
    facing = player_.camera.front_.x > 0 ? "NORTH" : "SOUTH";
  else
    facing = player_.camera.front_.z > 0 ? "EAST" : "WEST";

  auto [intersect_box, face] = boxFace_;

  char buffer[512];
  std::sprintf(buffer,
               "Debug Menu\n"                         //
               "Position: %4.2fx; %4.2fy; %4.2fz\n"   //
               "Facing: %s\n"                         //
               "SPF: %4.6fs       FPS: %4.2f\n"       //
               "Nb Cubes: %u\n"                       //
               "Targeting: %4.2fx; %4.2fy; %4.2fz\n"  //
               "Face: %d",
               player_.camera.position_.x, player_.camera.position_.y,
               player_.camera.position_.z, facing.c_str(), 1 / fps, fps,
               world_->boxes_.length(),  //
               intersect_box != nullptr ? intersect_box->origin_.x : '-',
               intersect_box != nullptr ? intersect_box->origin_.y : '-',
               intersect_box != nullptr ? intersect_box->origin_.z : '-', face);

  std::string debugstr{buffer};

  ft_->print(debugstr, 25.0F, 1020.0F, 1.0F, glm::vec3(1.0F, 1.0F, 1.0F));
}

void Window::FramebufferSizeCallback(GLFWwindow *window, int width,
                                     int height) {
  return static_cast<Window *>(glfwGetWindowUserPointer(window))
      ->FramebufferSizeCallbackHelper(width, height);
}

void Window::CursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
  return static_cast<Window *>(glfwGetWindowUserPointer(window))
      ->CursorPosCallbackHelper(xpos, ypos);
}

void Window::MouseButtonCallback(GLFWwindow *window, int button, int action,
                                 int mods) {
  return static_cast<Window *>(glfwGetWindowUserPointer(window))
      ->MouseButtonCallbackHelper(button, action, mods);
}

void Window::f3Input(int new_state) {
  static int state = GLFW_RELEASE;

  if (state != new_state) {
    if (state == GLFW_PRESS) debug_ = !debug_;
    state = new_state;
  }
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Window::ProcessInputs() {
  Terminal &term = Terminal::getInstance();

  if (!term.getVisible()) {
    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window_, true);

    if (glfwGetKey(window_, GLFW_KEY_SLASH) == GLFW_PRESS)
      term.setVisible(true);

    f3Input(glfwGetKey(window_, GLFW_KEY_F3));

    if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS)
      player_.camera.ProcessKeyboard(FORWARD, deltaTime_);
    if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS)
      player_.camera.ProcessKeyboard(BACKWARD, deltaTime_);
    if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS)
      player_.camera.ProcessKeyboard(LEFT, deltaTime_);
    if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS)
      player_.camera.ProcessKeyboard(RIGHT, deltaTime_);
    if (glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_PRESS)
      player_.camera.ProcessKeyboard(UP, deltaTime_);
    if (glfwGetKey(window_, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
      player_.camera.ProcessKeyboard(DOWN, deltaTime_);
  } else {
    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      term.setVisible(false);
  }
}