// System includes
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <vector>

// Library includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>


// Important constants used in the code
const unsigned int SCREEN_WIDTH = 800; 
const unsigned int SCREEN_HEIGHT = 600; 

// Forward declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void shaderCompiler(unsigned int& shaderObject, const std::string& shaderCode, GLenum shaderType); 
void shaderLinker(unsigned int& shaderProgram, const std::vector<unsigned int>& shaders);  


// Example Vertex Arrayss

// Simple isoceles traingle
std::vector<float> basicVertices = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};


// Rectangle made from two triangles
std::vector<float> Triangle = {
    // second triangle
     0.0f, 0.0f, 0.0f,  // bottom right
     0.0f, 1.0f, 0.0f,  // bottom left
     1.0f, 0.0f, 0.0f   // top left
};

std::vector<float> Triangle1 = {
    // first triangle
    -1.0f,  1.0f, 0.0f,  // top right
     0.0f, 0.0f, 0.0f,  // bottom right
    -1.0f,  0.0f, 0.0f,  // top left 
};

// Rectangle made from EBO's which reduce overhead by specifiying which index
// attributes to use which reduces the redundance
std::vector<float> EBOVertices {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};

std::vector<unsigned int> indices = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};  


// Makedo shader in the form of a string
const std::string  vertexShaderSource = "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "void main()\n"
  "{\n"
    "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
  "}\0";

const std::string fragmentShaderSource = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main()\n"
  "{\n"
    "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
  "}\0";

const std::string fragmentShaderSource1 = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main()\n"
  "{\n"
    "FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
  "}\0";


int main() {

// Boiler Plate GLFW code for initializing OpenGL and the API version and mode
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
 
// Function to create a basic GLFW window 
  GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
  
  if (window == NULL) {
    std::cout << "Failed to create GLFW Window\n";
    glfwTerminate();
    return EXIT_FAILURE;
  }
 
// Make the current window object the OpenGL context 
  glfwMakeContextCurrent(window);
  
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD\n";
    return EXIT_FAILURE;
  }
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

/*
  Here is where the actual OpenGL code will go for me to render my first
  triangle. I will learn about how to deal with the VBO and VAO's only first

  I will then complete the Exercises using only these two entities. 

  Only then will I start reading about EBOs and how to use them

*/
// managing the shader bs first:

// Creating the vertex and fragment shaders
  unsigned int vertexShader;
  shaderCompiler(vertexShader, vertexShaderSource, GL_VERTEX_SHADER);
  unsigned int fragmentShader;
  shaderCompiler(fragmentShader, fragmentShaderSource, GL_FRAGMENT_SHADER);
  unsigned int fragmentShader1;
  shaderCompiler(fragmentShader1, fragmentShaderSource1, GL_FRAGMENT_SHADER);
 
// Linking the shaders to a concrete shader program
  unsigned int shaderProgram; 
  shaderLinker(shaderProgram, {vertexShader, fragmentShader});   
  
  unsigned int shaderProgram1; 
  shaderLinker(shaderProgram1, {vertexShader, fragmentShader1});
  
  glDeleteShader(vertexShader); 
  glDeleteShader(fragmentShader); 
  glDeleteShader(fragmentShader1); 
// Working with the VBO
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float)*Triangle.size(), Triangle.data(), GL_STATIC_DRAW);

// Finally working with the VAO to actually draw the triangle
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
// copy our vertices from VBO to VAO for OpenGL to use
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float)*Triangle.size(), Triangle.data(), GL_STATIC_DRAW); 
// finally setting the vertex pointers
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

// Working with the VBO
  unsigned int VBO1;
  glGenBuffers(1, &VBO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float)*Triangle1.size(), Triangle1.data(), GL_STATIC_DRAW);

// Finally working with the VAO to actually draw the triangle
  unsigned int VAO1;
  glGenVertexArrays(1, &VAO1);
  glBindVertexArray(VAO1);
// copy our vertices from VBO to VAO for OpenGL to use
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float)*Triangle1.size(), Triangle1.data(), GL_STATIC_DRAW); 
// finally setting the vertex pointers
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
   
// Rendering Loop 
  while(!glfwWindowShouldClose(window)) {
    processInput(window);
    
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
   
// Now Drawing the first Triangle:
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3); 
// Now Drawing the second Triangle: 
    glUseProgram(shaderProgram1);
    glBindVertexArray(VAO1);
    glDrawArrays(GL_TRIANGLES, 0, 3); 
    
    glfwSwapBuffers(window);
    glfwPollEvents();
  } 
  
  glfwTerminate();
  return EXIT_SUCCESS;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void shaderCompiler(unsigned int& shaderObject, const std::string& shaderCode, GLenum shaderType) {
  // actual shader compilation
  shaderObject = glCreateShader(shaderType);
  auto shaderSourceCode = shaderCode.data(); 
  glShaderSource(shaderObject, 1, &shaderSourceCode, NULL);
  glCompileShader(shaderObject);
  // checker code for success 
  int success;
  char infoLog[512];
  glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &success);
  if(!success) {
    glGetShaderInfoLog(shaderObject, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
  }
}
 
void shaderLinker(unsigned int& shaderProgram, const std::vector<unsigned int>& shaders) {
  int success;
  char infoLog[512];
  shaderProgram = glCreateProgram();
  for( auto shader : shaders) {
    glAttachShader(shaderProgram, shader);
  }
  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if(!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
  }
  for( auto shader : shaders) {
    glDeleteShader(shader);
  }
}
  
  
