#include "font_renderer.h"

FontRenderer::FontRenderer()
    : shader("res/shaders/font/vertex.glsl", "res/shaders/font/fragment.glsl") {
  std::cout << "LOG::FREETYPE: Starting initialization" << std::endl;

  FT_Library ft;
  if (FT_Init_FreeType(&ft)) {
    std::cout << "ERROR::FREETYPE: Could not init FreeType Library"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  FT_Face face;
  if (FT_New_Face(ft, "res/fonts/arial.ttf", 0, &face)) {
    std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    exit(EXIT_FAILURE);
  }

  FT_Set_Pixel_Sizes(face, 0, 48);

  std::cout << "LOG::FREETYPE: Loading fonts" << std::endl;

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  // disable byte-alignment restriction

  for (unsigned char c = 0; c < 128; c++) {
    // load character glyph
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
      std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
      continue;
    }
    // generate texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
                 face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                 face->glyph->bitmap.buffer);
    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // now store character for later use
    Character character = {
        texture,
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        face->glyph->advance.x};
    Characters.insert(std::pair<char, Character>(c, character));
  }

  std::cout << "LOG::FREETYPE: Loading fonts done" << std::endl;

  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  std::cout << "LOG::FREETYPE: Initialization done" << std::endl;

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  projection = glm::ortho(0.0f, 1920.0F, 0.0f, 1080.0F);

  glGenVertexArrays(1, &VAO_);
  glGenBuffers(1, &VBO_);

  glBindVertexArray(VAO_);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

FontRenderer::~FontRenderer() {
  glDeleteVertexArrays(1, &VAO_);
  glDeleteBuffers(1, &VBO_);
}

void FontRenderer::print(const std::string& text, float x, float max_x, float y,
                         float scale, glm::vec3 color) {
  // figures out the width of the text then prints it with an ofset so it dosn't
  // overflow
  const float x0{x};
  // iterate through all characters
  std::string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++) {
    // TODO
    // if (*c == '\n') {
    // y -= 64;
    // x = x0;
    // continue;
    //}

    const Character ch = Characters[*c];
    x += (ch.Advance >> 6) *
         scale;  // bitshift by 6 to get value in pixels (2^6 = 64)
  }

  print(text, x > max_x ? max_x - x : x0, y, scale, color);
}

void FontRenderer::print(const std::string& text, float x, float y, float scale,
                         glm::vec3 color) {
  const float x0{x};

  // activate corresponding render state
  shader.use();
  shader.set_vec3("textColor", color);
  shader.set_mat4("projection", projection);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(VAO_);

  // iterate through all characters
  std::string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++) {
    if (*c == '\n') {
      y -= 64;
      x = x0;
      continue;
    }

    const Character ch = Characters[*c];

    float xpos = x + ch.Bearing.x * scale;
    float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

    float w = ch.Size.x * scale;
    float h = ch.Size.y * scale;
    // update VBO for each character
    float vertices[6][4] = {
        {xpos, ypos + h, 0.0f, 0.0f},    {xpos, ypos, 0.0f, 1.0f},
        {xpos + w, ypos, 1.0f, 1.0f},

        {xpos, ypos + h, 0.0f, 0.0f},    {xpos + w, ypos, 1.0f, 1.0f},
        {xpos + w, ypos + h, 1.0f, 0.0f}};
    // render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, ch.textureID);
    // update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // now advance cursors for next glyph (note that advance is number of 1/64
    // pixels)
    x += (ch.Advance >> 6) *
         scale;  // bitshift by 6 to get value in pixels (2^6 = 64)
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void FontRenderer::print(const std::string& text, float x, float y) {
  print(text, x, y, 1.0F, glm::vec3(1.0F, 0.5F, 0.6F));
}
