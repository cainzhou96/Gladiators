//
//  Particle.cpp
//  Project0
//
//  Created by Cain on 2/24/19.
//  Copyright © 2019 Cain. All rights reserved.
//

#include "Particle.h"
#include "glm/ext.hpp"
#include <iostream>
#define ELASTICITY 0.1
#define FRICTION 0.2

void Particle::update(float deltaTime) {
  float y0 = 1.0f;
  if (!fixed) {
    glm::vec3 accel = (1.0f/mass) * force;
    
    velocity += accel * deltaTime;
	if (glm::length(velocity) > 50) { // increase stability
		velocity = glm::normalize(velocity); 
	}
    position += velocity * deltaTime;

    force = glm::vec3(0.0f);
  }
  glm::vec3 temp = glm::vec3(position.x, 0, position.z); 
  if (position.y < y0 && glm::length(temp) <= 20.1f) {
    position.y += y0 - position.y;
    velocity.y += -ELASTICITY * velocity.y;
    velocity.x = (1-FRICTION) * velocity.x; // cheezy
    velocity.z = (1-FRICTION) * velocity.z; // cheezy
  }
}

void Particle::applyForce(glm::vec3 &f) {
  if (!fixed) {
    force += f;
  }
}

glm::vec3 Particle::getPosition() {
  return glm::vec3(toWorld * glm::vec4(position, 1));
}

void Particle::setPosition(glm::vec3 v) {
	position = v; 
}


glm::vec3 Particle::getVelocity() {
  return velocity; 
}

float Particle::getMass() {
  return mass;
}

glm::vec3 Particle::getNormal() {
  return normal;
}

void Particle::setNormal(glm::vec3 n) {
  normal = n; 
}

void Particle::movePosition(glm::vec3 dist) {
  position += dist; 
}
