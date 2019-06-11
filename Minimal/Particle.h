//
//  Particle.h
//  Project0
//
//  Created by Cain on 2/24/19.
//  Copyright © 2019 Cain. All rights reserved.
//

#ifndef Particle_h
#define Particle_h

#include <stdio.h>
#include <glm/mat4x4.hpp>

class Particle {
  float mass; // Constant
  glm::vec3 position; // Evolves frame to frame
  glm::vec3 velocity; // Evolves frame to frame
  glm::vec3 normal;
  glm::vec3 force; // Reset and re-computed each frame
  bool fixed;
  glm::mat4 toWorld;
public:
  Particle(float mass, bool fixed, glm::vec3 position) : mass(mass), fixed(fixed), position(position), toWorld(glm::mat4(1.0f)), velocity(glm::vec3(0.0f)), normal(glm::vec3(0.0f, 0.0f, 1.0f)) {}
  void update(float deltaTime);
  void applyForce(glm::vec3 &f);
  glm::vec3 getPosition();
  glm::vec3 getVelocity();
  float getMass();
  glm::vec3 getNormal();
  void setNormal(glm::vec3 n);
  void setPosition(glm::vec3 v); 
  void movePosition(glm::vec3 dist);
};

#endif /* Particle_h */
