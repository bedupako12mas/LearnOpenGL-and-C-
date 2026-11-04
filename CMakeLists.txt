#This is the single CMake file I will be using to learn OpenGL and graphics API

cmake_minimum_required(VERSION 3.20)
project(LearnOpenGL LANGUAGES C CXX)

add_subdirectory(src)
add_subdirectory(include)

# Creating GLFW
add_library(GLFW SHARED IMPORTED)

set_target_properties(GLFW PROPERTIES
  IMPORTED_LOCATION "/home/arun/graphics_work/LearnOpenGL/glfw/build/src/libglfw.so"
)

# Linking the libraries
target_link_libraries(output PRIVATE GLAD)
target_link_libraries(output PRIVATE GLFW)
