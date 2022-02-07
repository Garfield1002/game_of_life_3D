#include "terminal.h"

Terminal* Terminal::instance = nullptr;

const int height{30}, width{1024};

const float Terminal::vertices[]{
    0.0f,  100.0f + height, 1.0f,  //
    width, 100.0f + height, 1.0f,  //
    width, 100.0f - height, 1.0f,  //
    0.0f,  100.0f - height, 1.0f,  //
};

const unsigned int Terminal::indices[]{0, 1, 2, 2, 3, 0};

void Terminal::draw() const {
  if (!Terminal::visible) return;
  shader->use();

  shader->set_vec4("color", glm::vec4(0.0F, 0.0F, 0.0F, 0.1F));
  shader->set_mat4("projection", projection);
  shader->set_mat4("view", glm::mat4(1));
  shader->set_mat4("model", glm::mat4(1));

  glBindVertexArray(VAO_);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  // TODO eventually add a text cursor

  glDisable(GL_DEPTH_TEST);
  fr_->print(command, 25.0F, 1024.0F, 80.0F, 1.0F, glm::vec3(1.0F, 1.0F, 1.0F));
  glEnable(GL_DEPTH_TEST);
}

void Terminal::createInstance(GLFWwindow* window,
                              std::shared_ptr<FontRenderer> fr,
                              std::shared_ptr<World> world,
                              const Player const& player) {
  Terminal::instance = new Terminal(window, fr, world, player);
}

Terminal& Terminal::getInstance() {
  if (Terminal::instance == nullptr)
    throw std::exception("ERROR:TERMINAL:Terminal not initialized");

  return *Terminal::instance;
}

bool Terminal::getVisible() { return visible; }

void Terminal::key_callback(GLFWwindow* window, int key, int scancode,
                            int action, int mods) {
  Terminal& term = Terminal::getInstance();

  std::regex set_pattern("/set (~?-?\\d+) (~?-?\\d+) (~?-?\\d+)");
  std::regex rule_pattern("/rule (\\d+) (\\d+) (\\d+) (\\d+)");

  switch (key) {
    case GLFW_KEY_ENTER:
      if (action == GLFW_PRESS) {
        // Clear
        if (term.command == "/clear") {
          std::cout << "LOG::TERMINAL: Clearing all blocks" << std::endl;
          term.world_->boxes_.clear();
          term.setVisible(false);
          return;
        }

        // Set
        std::smatch match;
        if (std::regex_search(term.command, match, set_pattern)) {
          int x{match[1].str()[0] == '~'
                    ? (int)term.player_.camera.position_.x +
                          std::stoi(match[1].str().substr(1))
                    : std::stoi(match[1].str())};
          int y{match[2].str()[0] == '~'
                    ? (int)term.player_.camera.position_.y +
                          std::stoi(match[2].str().substr(1))
                    : std::stoi(match[2].str())};
          int z{match[3].str()[0] == '~'
                    ? (int)term.player_.camera.position_.z +
                          std::stoi(match[3].str().substr(1))
                    : std::stoi(match[3].str())};

          std::cout << "LOG::TERMINAL: Setting a block at " << x << "," << y
                    << "," << z << std::endl;
          term.world_->boxes_.insert(RBox(glm::vec3(x, y, z)));
          RBox::updateBuffer(term.world_->boxes_);

          term.setVisible(false);
          return;
        }

        // Rule
        if (std::regex_search(term.command, match, rule_pattern)) {
          int w{std::stoi(match[1].str())};
          int x{std::stoi(match[2].str())};
          int y{std::stoi(match[3].str())};
          int z{std::stoi(match[4].str())};

          term.world_->rule = Rule{w, x, y, z};

          term.setVisible(false);
          return;
        }

        // Start
        if (term.command == "/start") {
          std::cout << "LOG::TERMINAL: Stating the simulation" << std::endl;
          term.world_->initial_state = RBoxes(term.world_->boxes_);
          term.world_->setRunning(true);

          term.setVisible(false);
          return;
        }

        // Stop
        if (term.command == "/stop") {
          std::cout << "LOG::TERMINAL: Stopping the simulation" << std::endl;
          term.world_->setRunning(false);

          term.setVisible(false);
          return;
        }

        // Reset
        if (term.command == "/reset") {
          std::cout << "LOG::TERMINAL: Resetting the simulation" << std::endl;
          term.world_->setRunning(false);
          term.world_->boxes_ = RBoxes(term.world_->initial_state);
          RBox::updateBuffer(term.world_->boxes_);

          term.setVisible(false);
          return;
        }

        // Save
        //if (term.command == "/save") {
        //}

        // Load
        //if (term.command == "/load") {
        //}

        // Help
        //if (term.command == "/help")
        //return;
      }
       term.setVisible(false);
      return;
    case GLFW_KEY_BACKSPACE:
      if (action != GLFW_RELEASE && term.command.length() > 0)
        term.command.pop_back();
      return;
    default:
      break;
  }
}

void Terminal::character_callback(GLFWwindow* window, unsigned int codepoint) {
  Terminal& term = Terminal::getInstance();
  term.command.push_back(codepoint);
}

void Terminal::setVisible(bool newVisible) {
  visible = newVisible;
  if (visible) {
    glfwSetCharCallback(window_, Terminal::character_callback);
    glfwSetKeyCallback(window_, Terminal::key_callback);
  } else {
    glfwSetCharCallback(window_, NULL);
    glfwSetKeyCallback(window_, NULL);
    command = "/";
  }
}

Terminal::Terminal(GLFWwindow* window, std::shared_ptr<FontRenderer> fr,
                   std::shared_ptr<World> world, const Player const& player)
    : window_{window}, fr_{fr}, world_{world}, player_{player} {
  shader = new Shader("res/shaders/default/vertex.glsl",
                      "res/shaders/default/fragment.glsl");

  projection = glm::ortho(0.0f, 1920.0F, 0.0f, 1080.0F);

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
                        (void*)0   // offset
  );
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

Terminal::~Terminal() {}