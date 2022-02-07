#ifndef SHADER_H
#define SHADER_H

#include <fstream>
#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
private:
  // the program ID
  const GLuint ID_;

  std::string StrFromFile(const std::string &path);

  GLuint CompileShader(const char *shader_code, GLenum type);

public:
  // constructor reads and builds the shader
  Shader(const std::string &vertexPath, const std::string &fragmentPath);

  ~Shader() { glDeleteProgram(ID_); }

  // use/activate the shader
  void use() const;

  // utility uniform functions
  void set_bool(const std::string &name, GLboolean value) const;
  void set_int(const std::string &name, GLint value) const;
  void set_float(const std::string &name, GLfloat value) const;
  void set_vec3(const std::string &name, const glm::vec3 &value) const;
  void set_vec4(const std::string &name, const glm::vec4 &value) const;
  void set_mat4(const std::string &name, const glm::mat4 &value) const;
};
#endif
