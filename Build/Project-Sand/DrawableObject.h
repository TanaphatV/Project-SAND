#pragma once
#include "glm.hpp"

using namespace std;

class DrawableObject
{ 
  
public:

  virtual void render(glm::mat4 transform, unsigned int time)=0;
};

