#include <iostream>
// Library which gives "function pointers" of OpenGL functionalities in the OS
#include <glad/glad.h>
// Library for windowing and using OpenGL
#include <GLFW/glfw3.h>


// forward declaration for functions defined later 
// function to allow constant window resizing 
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// function to process input (keyboard/mouse) 
void processInput(GLFWwindow *window);


int main() {
  //Inititalising GLFW and setting OpenGL versions
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  // For MacOSX
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLTRUE);
  
  // Creating the window object
  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  
  //checking if the object is created or now
  if (window == NULL) {
    std::cout << "Failed to create GLFW Window\n";
    glfwTerminate();
    return EXIT_FAILURE;
  }
  
  // setting the created window as the current context
  glfwMakeContextCurrent(window);
  
  // checking if glad and the openGL function pointers have loaded correctly
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD\n";
    return EXIT_FAILURE;
  }
  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
 
  //Infinite Rendering Loop  
  while(!glfwWindowShouldClose(window)) {
    //Input 
    processInput(window);
    
    //Actual Rendering  
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // State setting function
    glClear(GL_COLOR_BUFFER_BIT); // State using function
    
    // check and call events and swap the buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  } 
  
  // function to clean the resources allocated to GLFW 
  glfwTerminate();
  // I am just happy I am using this macro I read in LearnCPP
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
