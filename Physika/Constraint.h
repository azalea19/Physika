#ifndef Constraint_h__
#define Constraint_h__

#include "Particle.h"

typedef struct Constraint
{
  //The two particles that are connected through this constraint
  Particle* p1;
  Particle* p2;

  float restDistance;

}Constraint;

void constraint_Init(Constraint* const constraint);

void constraint_Satisfy(Particle* const p1, Particle* const p2);


#endif // Constraint_h__
