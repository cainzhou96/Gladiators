//
//  SpringDamper.h
//  Project0
//
//  Created by Cain on 2/24/19.
//  Copyright © 2019 Cain. All rights reserved.
//

#ifndef SpringDamper_h
#define SpringDamper_h

#include <stdio.h>
#include "Particle.h"

class SpringDamper {
  float springConstant,dampingFactor;
  float restLength;
  Particle *p1,*p2;
public:
  SpringDamper(float springConstant, float dampingFactor, float restLength, Particle *p1, Particle *p2) : springConstant(springConstant), dampingFactor(dampingFactor), restLength(restLength), p1(p1), p2(p2) {}
  ~SpringDamper();
  void computeForce();
}; 

#endif /* SpringDamper_h */
