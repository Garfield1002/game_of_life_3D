#include "shader.h"

// Loads a string from a file.
std::string Shader::StrFromFile(const std::string &path) {
  std::string str;
  std::ifstream file;
  // ensure ifstream object can throw exceptions
  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    std::stringstream stream;
    file.open(path);         // opens the file
    stream << file.rdbuf();  // read the file's buffer content into the stream
    file.close();            // close file handlers
    str = stream.str();      // convert the stream into a string
  } catch (const std::ifstream::failure &) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    exit(EXIT_FAILURE);
  }
  return str;
}

GLuint Shader::CompileShader(const char *shader_code, GLenum type) {
  GLuint shader;
  GLint success;
  char info_log[512];

  shader = glCreateShader(type);
  glShaderSource(shader, 1, &shader_code, NULL);
  glCompileShader(shader);

  // print compile errors if any
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, info_log);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << info_log << std::endl;
  };

  return shader;
}

// Shader constructor, loads and links a new fragment and vertex shader.
Shader::Shader(const std::string &vertex_path, const std::string &fragment_path)
    : ID_{glCreateProgram()} {
  GLuint vertex{
      CompileShader(StrFromFile(vertex_path).c_str(), GL_VERTEX_SHADER)};
  GLuint fragment{
      CompileShader(StrFromFile(fragment_path).c_str(), GL_FRAGMENT_SHADER)};

  // shader program
  glAttachShader(ID_, vertex);
  glAttachShader(ID_, fragment);
  glLinkProgram(ID_);

  // print linking errors if any
  GLint success;
  char info_log[512];
  glGetProgramiv(ID_, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID_, 512, NULL, info_log);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << info_log << std::endl;
  }

  // delete the shaders as they're linked into our program now and no longer
  // necessary
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Shader::use() const { glUseProgram(ID_); }

void Shader::set_bool(const std::string &name, GLboolean value) const {
  glUniform1i(glGetUniformLocation(ID_, name.c_str()), (int)value);
}

void Shader::set_int(const std::string &name, GLint value) const {
  glUniform1i(glGetUniformLocation(ID_, name.c_str()), value);
}

void Shader::set_float(const std::string &name, GLfloat value) const {
  glUniform1f(glGetUniformLocation(ID_, name.c_str()), value);
}

void Shader::set_vec3(const std::string &name, const glm::vec3 &value) const {
  glUniform3fv(glGetUniformLocation(ID_, name.c_str()), 1, &value[0]);
}

void Shader::set_vec4(const std::string &name, const glm::vec4 &value) const {
  glUniform4fv(glGetUniformLocation(ID_, name.c_str()), 1, &value[0]);
}

void Shader::set_mat4(const std::string &name, const glm::mat4 &value) const {
  glUniformMatrix4fv(glGetUniformLocation(ID_, name.c_str()), 1, GL_FALSE,
                     &value[0][0]);
}
