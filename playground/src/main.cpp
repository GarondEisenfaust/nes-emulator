#include <boost/dll/runtime_symbol_info.hpp>
#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include "Definitions.h"
#include "Rectangle.h"
#include "Shader.h"
#include <GLFW/glfw3.h>

std::string GetExecutableDirectory() { return std::string(boost::dll::program_location().parent_path().c_str()); }

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// The MAIN function, from here we start the application and run the game loop
int main() {
  // Init GLFW
  glfwInit();
  // Set all the required options for GLFW
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  // Create a GLFWwindow object that we can use for GLFW's functions
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
  glfwMakeContextCurrent(window);

  // Set the required callback functions
  glfwSetKeyCallback(window, key_callback);

  // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
  glewExperimental = GL_TRUE;
  // Initialize GLEW to setup the OpenGL Function pointers
  glewInit();

  // Define the viewport dimensions
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  // Build and compile our shader program
  // Vertex shader

  auto workDir = GetExecutableDirectory();
  Shader fragmentShader((workDir + "/shaders/FragmentShader.glsl").c_str(), GL_FRAGMENT_SHADER);
  Shader vertexShader((workDir + "/shaders/VertexShader.glsl").c_str(), GL_VERTEX_SHADER);

  // Link shaders
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader.GetHandle());
  glAttachShader(shaderProgram, fragmentShader.GetHandle());
  glLinkProgram(shaderProgram);
  // Check for linking errors

  // Set up vertex data (and buffer(s)) and attribute pointers
  // We add a new set of vertices to form a second triangle (a total of 6 vertices); the vertex attribute configuration
  // remains the same (still one 3-float position vector per vertex)

  std::array<Rectangle::TriangleVertices, 6> triangles = {
      Rectangle(0, 0, 50, 50).CalculateTriangles(),     Rectangle(50, 50, 50, 50).CalculateTriangles(),
      Rectangle(100, 100, 50, 50).CalculateTriangles(), Rectangle(100, 0, 50, 50).CalculateTriangles(),
      Rectangle(0, 100, 50, 50).CalculateTriangles(),   Rectangle(0, 200, 50, 50).CalculateTriangles()};

  std::vector<float> vertices;
  vertices.resize(triangles.size() * NUMBER_OF_VERTICES * NUMBER_OF_COORDINATES);
  std::memcpy(vertices.data(), triangles.data(), sizeof(triangles));

  GLuint VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);  // Note that this is allowed, the call to glVertexAttribPointer registered VBO as
                                     // the currently bound vertex buffer object so afterwards we can safely unbind

  glBindVertexArray(0);  // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

  // Game loop
  while (!glfwWindowShouldClose(window)) {
    // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response
    // functions
    glfwPollEvents();

    // Render
    // Clear the colorbuffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw our first triangle
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);

    // Swap the screen buffers
    glfwSwapBuffers(window);
  }
  // Properly de-allocate all resources once they've outlived their purpose
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  // Terminate GLFW, clearing any resources allocated by GLFW.
  glfwTerminate();
  return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
}
