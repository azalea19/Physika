#ifndef Cloth_h__
#define Cloth_h__

#include "Particle.h"
#include "Constraint.h"
#include <stdlib.h>
#include "Input.h"


/*******************************************************************************************************************************
* @file Cloth.h
* @author Maddisen Topaz , Matt Smooker, Caroline Chua
* @date S1, 2017
* @brief The cloth class contains all of the functions related to the cloth structure.
* @description
*
*******************************************************************************************************************************/

#define CONSTRAINT_ITERATIONS 15


extern int CLOTH_BANNER;
extern int CLOTH_CURTAIN;

static int lastCloth = 1;

typedef struct Cloth
{
  int numParticlesX;
  int numParticlesY;

  int width;
  int height;

  int numConstraints;

  Particle* particles;
  Constraint* constraints;

}Cloth;

void cloth_Init(Cloth** const cloth); 

void cloth_Create(Cloth* const cloth, int width, int height, int a_numParticlesX, int a_numParticlesY);

Particle* cloth_GetParticle(Cloth* const cloth, int x, int y);

void cloth_AddParticles(Cloth* cloth);

void cloth_ChangeType(Cloth* cloth, int clothType);

void cloth_AddConstraints(Cloth* cloth);

vec3 cloth_CalculateFlatNormal(Particle const* p1, Particle const* p2, Particle const* p3);

void cloth_AddWindForceOnTriangle(Particle* p1, Particle* p2, Particle* p3, vec3 direction);

void cloth_DrawTriangle(Particle const*  p1, Particle const* p2, Particle const* p3, vec3 color);

void cloth_DrawShaded(Cloth* cloth, int lighting);

void cloth_BallCollision(Cloth* cloth, vec3 center, float radius);

void cloth_TimeStep(Cloth* cloth, vec3 ballPos, float ballRadius);

void cloth_AddForce(Cloth* cloth, vec3 direction);

void cloth_WindForce(Cloth* cloth, vec3 direction);

#endif // Cloth_h__
