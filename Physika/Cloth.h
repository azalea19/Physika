#ifndef Cloth_h__
#define Cloth_h__

#include "Particle.h"
#include "Constraint.h"
#include <stdlib.h>


typedef struct Cloth
{
  int numParticlesX;
  int numParticlesY;

  int width;
  int height;

  Particle* particles;
  Constraint* constraints;

}Cloth;

void cloth_Init(Cloth* const cloth, int width, int height, int a_numParticlesX, int a_numParticlesY)
{
  cloth->numParticlesX = a_numParticlesX;
  cloth->numParticlesY = a_numParticlesY;
}

void cloth_Create(Cloth* const cloth)
{
  cloth->particles = (Particle*)malloc(sizeof(Particle*)*cloth->numParticlesX*cloth->numParticlesY);

  int numConstraintsX = (cloth->numParticlesX - 1)*cloth->numParticlesY;
  int numConstraintY = (cloth->numParticlesY - 1)*cloth->numParticlesX;
  int numDiagonalConstraints = 2 * numConstraintsX*numConstraintY;

  cloth->constraints = (Constraint*)malloc(sizeof(Constraint*) * (numConstraintsX + numConstraintY + numDiagonalConstraints));
}

Particle* cloth_GetParticle(Cloth* const cloth, int x, int y)
{
  return &cloth->particles[y*cloth->numParticlesX + x];
}

void cloth_AddConstraints(Cloth* cloth)
{

}

#endif // Cloth_h__
