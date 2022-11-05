#include "RenderContext.h"
#include "Definitions.h"
#include "Rectangle.h"
#include "Shader.h"
#include <boost/dll/runtime_symbol_info.hpp>
#include <iostream>

std::string GetExecutableDirectory() { return std::string(boost::dll::program_location().parent_path().c_str()); }
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

RenderContext::RenderContext() : mVertices(std::make_unique<std::vector<float>>()) {  // Init GLFW
  glfwInit();
  // Set all the required options for GLFW
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  // Create a GLFWwindow object that we can use for GLFW's functions
  mWindow = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
  glfwMakeContextCurrent(mWindow);

  // Set the required callback functions
  glfwSetKeyCallback(mWindow, key_callback);

  // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
  glewExperimental = GL_TRUE;
  // Initialize GLEW to setup the OpenGL Function pointers
  glewInit();

  // Define the viewport dimensions
  glfwGetFramebufferSize(mWindow, &mWidth, &mHeight);
  glViewport(0, 0, mWidth, mHeight);

  // Build and compile our shader program
  // Vertex shader

  auto workDir = GetExecutableDirectory();
  Shader fragmentShader((workDir + "/shaders/FragmentShader.glsl").c_str(), GL_FRAGMENT_SHADER);
  Shader vertexShader((workDir + "/shaders/VertexShader.glsl").c_str(), GL_VERTEX_SHADER);
  mShaderProgram = glCreateProgram();
  glAttachShader(mShaderProgram, vertexShader.GetHandle());
  glAttachShader(mShaderProgram, fragmentShader.GetHandle());
  glLinkProgram(mShaderProgram);
}

void RenderContext::Init() {  // Link shaders
  glGenVertexArrays(1, &mVAO);
  glGenBuffers(1, &mVBO);
  // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
  glBindVertexArray(mVAO);

  glBindBuffer(GL_ARRAY_BUFFER, mVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mVertices->size(), mVertices->data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);  // Note that this is allowed, the call to glVertexAttribPointer registered VBO as
                                     // the currently bound vertex buffer object so afterwards we can safely unbind

  glBindVertexArray(0);  // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
}

RenderContext::~RenderContext() {  // Properly de-allocate all resources once they've outlived their purpose
  glDeleteVertexArrays(1, &mVAO);
  glDeleteBuffers(1, &mVBO);
  // Terminate GLFW, clearing any resources allocated by GLFW.
  glfwTerminate();
}

void RenderContext::AddVertices(Rectangle::TriangleVertices& verticesToAdd) {
  std::copy(verticesToAdd.begin(), verticesToAdd.end(), std::back_inserter(*mVertices));
}

void RenderContext::GameLoop(std::function<void()> loop) {  // Game loop
  while (!glfwWindowShouldClose(mWindow)) {
    // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response
    // functions
    glfwPollEvents();

    // Render
    // Clear the colorbuffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw our first triangle
    glUseProgram(mShaderProgram);
    glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLES, 0, mVertices->size());
    glBindVertexArray(0);

    // Swap the screen buffers
    glfwSwapBuffers(mWindow);

    loop();
  }
}

int RenderContext::GetHeight() { return mHeight; }
int RenderContext::GetWidth() { return mWidth; }

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
}
