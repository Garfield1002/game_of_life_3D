#ifndef FONT_RENDERER_H
#define FONT_RENDERER_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <map>

#include "../shader/shader.h"

class FontRenderer {
 public:
  FontRenderer();
  ~FontRenderer();

  void print(const std::string &text, float x, float max_x, float y,
             float scale, glm::vec3 color);

  Shader shader;

  void print(const std::string &text, float x, float y,
             float scale, glm::vec3 color);

  void print(const std::string &text, float x, float y);

 private:
  GLuint VAO_, VBO_;
  glm::mat4 projection;

  struct Character {
    unsigned int textureID;  // ID handle of the glyph texture 
    glm::ivec2 Size;         // Size of glyph
    glm::ivec2 Bearing;      // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
  };

  std::map<char, Character> Characters;
};

#endif  // !FONT_RENDERER_H
