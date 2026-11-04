#This makefile is simply to build the target library

add_library(GLAD STATIC
  src/glad.c
)

target_include_directories(GLAD PUBLIC
  include
)
