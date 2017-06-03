#ifndef Constraint_h__
#define Constraint_h__

#include "Particle.h"

/*******************************************************************************************************************************
* @file Constraint.h
* @author Maddisen Topaz
* @date S1, 2017
* @brief This constraint class contains all of the functions related to a constraint between two particles.
* @description
*
*******************************************************************************************************************************/


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
