#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>

using gl_id = unsigned int;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);
std::pair<gl_id, std::optional<std::string>> load_shader_file(
    const std::string& filename, const int shader_type);
std::pair<gl_id, std::optional<std::string>> link_program(
    const std::vector<gl_id>& shaders);
gl_id create_vertex_array_object();
gl_id create_vertex_buffer_object();

int main() {
  // initialize window
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  const auto window =
      glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);

  if (window == nullptr) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  framebuffer_size_callback(window, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // load shaders
  const auto [vertexShader, vertexError] =
      load_shader_file("shaders/vertex.glsl", GL_VERTEX_SHADER);
  if (vertexError) {
    std::cerr << vertexError.value() << std::endl;
    glfwTerminate();
    return -1;
  }

  const auto [fragmentShader, fragmentError] =
      load_shader_file("shaders/fragment.glsl", GL_FRAGMENT_SHADER);
  if (fragmentError) {
    std::cerr << fragmentError.value() << std::endl;
    glfwTerminate();
    return -1;
  }

  const auto [shaderProgram, programError] =
      link_program({vertexShader, fragmentShader});
  if (programError) {
    std::cerr << programError.value() << std::endl;
    glfwTerminate();
    return -1;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // load data
  const auto VAO = create_vertex_array_object();
  glBindVertexArray(VAO);

  const float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f,
                            0.0f,  0.0f,  0.5f, 0.0f};

  const auto VBO = create_vertex_buffer_object();
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);

  // render loop
  while (!glfwWindowShouldClose(window)) {
    process_input(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}

void framebuffer_size_callback([[maybe_unused]] GLFWwindow* window, int width,
                               int height) {
  glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

std::pair<gl_id, std::optional<std::string>> link_program(
    const std::vector<gl_id>& shaders) {
  const gl_id program = glCreateProgram();

  for (auto shader : shaders) glAttachShader(program, shader);

  glLinkProgram(program);

  int success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);

  if (!success) {
    std::string error;
    error.resize(512);
    glGetProgramInfoLog(program, 512, NULL, error.data());
    return {program, {error}};
  } else {
    return {program, {}};
  }
}

std::pair<gl_id, std::optional<std::string>> load_shader_file(
    const std::string& filename, const int shader_type) {
  std::ifstream file(filename);
  std::string source;

  file.seekg(0, std::ios::end);
  source.resize(file.tellg());
  file.seekg(0);
  file.read(source.data(), source.size());

  const char* source_chars = source.c_str();

  const gl_id shader = glCreateShader(shader_type);
  glShaderSource(shader, 1, &source_chars, nullptr);
  glCompileShader(shader);

  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    std::string error;
    error.resize(512);
    glGetShaderInfoLog(shader, 512, nullptr, error.data());

    return {shader, {error}};
  } else {
    return {shader, {}};
  }
}

gl_id create_vertex_array_object() {
  gl_id VAO;
  glGenVertexArrays(1, &VAO);
  return VAO;
}

gl_id create_vertex_buffer_object() {
  gl_id VBO;
  glGenBuffers(1, &VBO);
  return VBO;
}
