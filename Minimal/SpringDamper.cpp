//
//  SpringDamper.cpp
//  Project0
//
//  Created by Cain on 2/24/19.
//  Copyright © 2019 Cain. All rights reserved.
//

#include "SpringDamper.h"
#include <iostream>
#include "glm/ext.hpp"

void SpringDamper::computeForce() {
  glm::vec3 e = p2->getPosition() - p1->getPosition();
  float length = glm::length(e);
  // std::cout << restLength - length << std::endl;
  /*if (restLength - length < 0.000001) {
    return; 
  }*/
  if (length != 0) {
	  e = glm::normalize(e);
  }
  else {
	  e = glm::vec3(0, 0, -1); 
  }
  float fsd = - springConstant * (restLength - length) - dampingFactor * (glm::dot(e, p1->getVelocity()) - glm::dot(e, p2->getVelocity()));

  glm::vec3 f1 = fsd * e;
  glm::vec3 f2 = -f1;
  // std::cout << glm::to_string(f1) << std::endl;
  p1->applyForce(f1);
  p2->applyForce(f2);
}
