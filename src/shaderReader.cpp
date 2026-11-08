#include <iostream>
#include <string>
#include <fstream>

#include "shaderReader.h"

shaderReader::shaderReader(const std::string& filename) {
  this->fileName = filename;
}

void shaderReader::loadShader() {
  std::ifstream shader{fileName};
  if(!shader) {
    std::cerr << "File could not be opened for reading\n";
  }
  std::string lineBuffer{};
  while(std::getline(shader, lineBuffer)) {
    finalShader += lineBuffer
  }  
};

const std::string& shaderReader::getShader() {
  return finalShader;
};



