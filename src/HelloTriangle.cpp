#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

// Important constants used in the code
const unsigned int SCREEN_WIDTH = 800; 
const unsigned int SCREEN_HEIGHT = 600; 


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// Example Vertex
// Since we are rendering a triangle all the z coordinates are 0


// Simple isoceles traingle
std::vector<float> basicVertices = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};


// Rectangle made from two triangles
std::vector<float> rectangleVertices = {
    // first triangle
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f,  0.5f, 0.0f,  // top left 
    // second triangle
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left
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
const char* vertexShaderSource = "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "void main()\n"
  "{\n"
    "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
  "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main()\n"
  "{\n"
    "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
  "}\0";



int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  
  if (window == NULL) {
    std::cout << "Failed to create GLFW Window\n";
    glfwTerminate();
    return EXIT_FAILURE;
  }
  
  glfwMakeContextCurrent(window);
  
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD\n";
    return EXIT_FAILURE;
  }
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // I dont think I need to allocate memory on the GPU inside the rendering loop
  // since it would re-allocate every iteration?? I can check and experiment
  // Vertex buffer object to store everything in batches since moving data from
  // CPU to GPU is costly 
  unsigned int VBO, VAO, EBO; 
  // creating the VBO
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  //Generating the VAO to bind to the VBO 
  glGenVertexArrays(1, &VAO);
  // Creating the EBO
  glGenBuffers(1,&EBO);
  
  
  // function specifically targetted to copy user-defined data into currently
  // bound buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float)*rectangleVertices.size(), rectangleVertices.data(), GL_STATIC_DRAW);
  
  // now creating the vertex shader object
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  // now working with the vertex shader
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  //checking vertex shader compilation success
  int vertexSuccess;
  char vertexInfoLog[512];
  // checking vertex shader compilation success
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexSuccess);
  if(!vertexSuccess) {
    glGetShaderInfoLog(vertexShader, 512, NULL, vertexInfoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << vertexInfoLog << std::endl;      
  }

  // now creating the fragment shader object
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  //now working with the fragment shader
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader); 
  //checking vertex shader compilation success
  int fragmentSuccess;
  char fragmentInfoLog[512];
  // checking vertex shader compilation success
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentSuccess);
  if(!fragmentSuccess) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, fragmentInfoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fragmentInfoLog << std::endl;      
  }

  // now the creating the shader program and linking the shaders
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  // linking the shaders (vertex and fragment)
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  // checking shader creation and linking success
  int shaderSuccess;
  char shaderInfoLog[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &shaderSuccess);
  if(!shaderSuccess) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, shaderInfoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING::FAILED\n" << shaderInfoLog << std::endl;
  }
  //deleting the shaders (vertex & fragment) due to no longer required
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // Now binding the VAO
  glBindVertexArray(VAO); 
  // copy our vertices array in a buffer for OpenGL to use
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float)*rectangleVertices.size(), rectangleVertices.data(), GL_STATIC_DRAW);
  // setting the vertex attribute pointers
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  //EBO binding and buffer copying 
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*indices.size(), indices.data(), GL_STATIC_DRAW);

  // Rendering Loop 
  while(!glfwWindowShouldClose(window)) {
    processInput(window);
    
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);   
  
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

