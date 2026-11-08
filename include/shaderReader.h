#ifndef SHADER_READER.h
#define SHADER_READER.h

// basic includes for building the reader class
#include <iostream>
#include <string> 

// minimal shader with no error handling

class shaderReader{
  public:
    shaderReader(const std::string& filname);
    void loadShader();
    const std::string& getShader(); 
  private:
    std::string fileName;
    std::string finalShader {};
};

#endif
