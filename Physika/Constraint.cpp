#include "Constraint.h"



Constraint constraint_Make(Particle* p1, Particle* p2, float restDistance)
{
  Constraint c;
  c.p1 = p1;
  c.p2 = p2;
  c.restDistance = restDistance;
  return c;
}

void constraint_Init(Constraint* const constraint)
{
  vec3 vec = vec3_Subtract(constraint->p1->pos, constraint->p2->pos);
  constraint->restDistance = vec3_Magnitude(vec);
}

void constraint_Satisfy(Constraint* const constraint)
{
  //Vector between p1 and p2
  vec3 p1_to_p2 = vec3_Subtract(constraint->p2->pos, constraint->p1->pos);
  //Distance between p1 and p2
  float currentDistance = vec3_Magnitude(p1_to_p2);
  
  //Must be done in constructor
  //float restDistance = vec3_Magnitude(vec3_Subtract(p1->pos, p2->pos));
  
  vec3 correctionVector = vec3_Multiply(p1_to_p2, 1 - constraint->restDistance / currentDistance);
  //Make it half the length so we can move p1 and p2
  vec3 correctionVectorHalf = vec3_Multiply(correctionVector, 0.5);
  //correctionVectorHalf is pointing from p1 to p2
  if(constraint->p1->moveable)
    particle_Move(constraint->p1, correctionVectorHalf);
  //We move p2 in the direction of -correctionVectorHalf as it is pointing from p2 to p1 
  if(constraint->p2->moveable)
    particle_Move(constraint->p2, vec3_Multiply(correctionVectorHalf, -1));
}
