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

void constraint_Satisfy(Constraint* const constraint);

Constraint constraint_Make(Particle* p1, Particle* p2, float restDistance);

#endif // Constraint_h__
