#ifndef Cloth_h__
#define Cloth_h__

#include "Particle.h"
#include "Constraint.h"
#include <stdlib.h>


#define CONSTRAINT_ITERATIONS 15

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

void cloth_Init(Cloth** const cloth)
{
  *cloth = (Cloth*)malloc(sizeof(Cloth));
} 

void cloth_Create(Cloth* const cloth, int width, int height, int a_numParticlesX, int a_numParticlesY)
{
  cloth->width = width;
  cloth->height = height;
  cloth->numParticlesX = a_numParticlesX;
  cloth->numParticlesY = a_numParticlesY;

  cloth->particles = (Particle*)malloc(sizeof(Particle)*cloth->numParticlesX*cloth->numParticlesY);

  int numConstraintsX = (cloth->numParticlesX * 2 - 3)*cloth->numParticlesY;
  int numConstraintY = (cloth->numParticlesY * 2 - 3)*cloth->numParticlesX;
  int numDiagonalConstraints = 2 * ((cloth->numParticlesX - 1) * (cloth->numParticlesY - 1) + (cloth->numParticlesX - 2) * (cloth->numParticlesY - 2));

  cloth->constraints = (Constraint*)malloc(sizeof(Constraint) * (numConstraintsX + numConstraintY + numDiagonalConstraints));
  cloth->numConstraints = numConstraintsX + numConstraintY + numDiagonalConstraints;
}

Particle* cloth_GetParticle(Cloth* const cloth, int x, int y)
{
  return &(cloth->particles[y*cloth->numParticlesX + x]);
}

void cloth_AddParticles(Cloth* cloth)
{
  float widthScale = (float)cloth->width / (float)(cloth->numParticlesX-1);
  float heightScale = (float)cloth->height / (float)(cloth->numParticlesY-1);

  for(int y = 0; y < cloth->numParticlesY; y++)
  {
    for (int x = 0; x < cloth->numParticlesX; x++)
    {
      vec3 position;
      position.x = x*widthScale;
      position.y = y*heightScale;
      position.z = 0;
      particle_Init(&(cloth->particles[y*cloth->numParticlesX + x]));
      cloth->particles[y*cloth->numParticlesX + x].pos = position;
      cloth->particles[y*cloth->numParticlesX + x].old_pos = position;
    }
  }

  for (int x = 0; x < cloth->numParticlesX; ++x)
  {
    cloth_GetParticle(cloth, x, cloth->numParticlesY - 1)->moveable = false;
  }
}


void cloth_AddConstraints(Cloth* cloth)
{
  int count = 0;
  //Diagonal distance is sqrt(2) away
  //Connecting immediate neighbor particles with constraints (distance 1 and sqrt(2) in the grid)
  for (int x = 0; x < cloth->numParticlesX; x++)
  {
    for (int y = 0; y < cloth->numParticlesY; y++)
    {
      if (x < cloth->numParticlesX - 1)
      {
        cloth->constraints[count] = constraint_Make(cloth_GetParticle(cloth, x, y), cloth_GetParticle(cloth, x + 1, y), ((float)cloth->width / ((float)cloth->numParticlesX - 1)));
        count++;
      }     
      if (y < cloth->numParticlesY - 1)
      {
        cloth->constraints[count] = constraint_Make(cloth_GetParticle(cloth, x, y), cloth_GetParticle(cloth, x, y + 1), ((float)cloth->height / ((float)cloth->numParticlesY - 1)));
        count++;
      }       
      if (x < cloth->numParticlesX - 1 && y < cloth->numParticlesY - 1)
      {
        cloth->constraints[count] = constraint_Make(cloth_GetParticle(cloth, x, y), cloth_GetParticle(cloth, x + 1, y + 1), (float)sqrt((float)pow((float)cloth->width / (float)(cloth->numParticlesX - 1), 2) + (float)pow((float)cloth->height / (float)(cloth->numParticlesY - 1), 2)));
        count++;
      }       
      if (x < cloth->numParticlesY - 1 && y < cloth->numParticlesY - 1)
      {
        cloth->constraints[count] = constraint_Make(cloth_GetParticle(cloth, x + 1, y), cloth_GetParticle(cloth, x, y + 1), (float)sqrt((float)pow((float)cloth->width / (float)(cloth->numParticlesX - 1), 2) + (float)pow((float)cloth->height / (float)(cloth->numParticlesY - 1), 2)));
        count++;
      }       
    }
  }

  //Connecting secondary neighbors with constraints (distance 2 and sqrt(4) in the grid)
  for (int x = 0; x < cloth->numParticlesX; x++)
  {
    for (int y = 0; y < cloth->numParticlesY; y++)
    {
      if (x < cloth->numParticlesX - 2)
      {
        cloth->constraints[count] = constraint_Make(cloth_GetParticle(cloth, x, y), cloth_GetParticle(cloth, x + 2, y), ((float)cloth->width / ((float)cloth->numParticlesX - 1)) * 2);
        count++;
      }       
      if (y < cloth->numParticlesY - 2)
      {
        cloth->constraints[count] = constraint_Make(cloth_GetParticle(cloth, x, y), cloth_GetParticle(cloth, x, y + 2), ((float)cloth->height / ((float)cloth->numParticlesY - 1)) * 2);
        count++;
      }        
      if (x < cloth->numParticlesX - 2 && y < cloth->numParticlesY - 2)
      {
        cloth->constraints[count] = constraint_Make(cloth_GetParticle(cloth, x, y), cloth_GetParticle(cloth, x + 2, y + 2), (float)sqrt((float)pow((float)cloth->width / (float)(cloth->numParticlesX - 1), 2) + (float)pow((float)cloth->height / (float)(cloth->numParticlesY - 1), 2)) * 2);
        count++;
      }       
      if (x < cloth->numParticlesX - 2 && y < cloth->numParticlesY - 2)
      {
        cloth->constraints[count] = constraint_Make(cloth_GetParticle(cloth, x + 2, y), cloth_GetParticle(cloth, x, y + 2), (float)sqrt((float)pow((float)cloth->width / (float)(cloth->numParticlesX - 1), 2) + (float)pow((float)cloth->height / (float)(cloth->numParticlesY - 1), 2)) * 2);
        count++;
      }       
    }
  }
}

vec3 cloth_CalculateFlatNormal(Particle const* p1, Particle const* p2, Particle const* p3)
{
  //Get the vector from p1 to p2
  vec3 a = vec3_Subtract(p2->pos, p1->pos);
  //Get the vector from p1 to p3
  vec3 b = vec3_Subtract(p3->pos, p1->pos);

  //Get the normal by taking the cross product of these two vectors (two edges of the triangle)
  return vec3_Cross(a, b);
}

void cloth_AddWindForceOnTriangle(Particle* p1, Particle* p2, Particle* p3, vec3 direction)
{
  //The normal of the triangle
  vec3 normal = cloth_CalculateFlatNormal(p1, p2, p3);
  //The unit normal of the triangle
  vec3 unitNormal = vec3_Normalize(normal);

  
  float unitForce = vec3_Dot(unitNormal, direction);
  //Multiply the force by the regular normal
  //This works because the normal is equal to the normalized normal multiplied by half the area of the triangle
  //We multiply the force by the area of the triangle because a larger triangle will be more affected by a wind force
  vec3 force = vec3_Multiply(normal, unitForce);

  particle_AddForce(p1, force);
  particle_AddForce(p2, force);
  particle_AddForce(p3, force);
}

void cloth_DrawTriangle(Particle const*  p1, Particle const* p2, Particle const* p3, vec3 color)
{
  glColor3f(color.x, color.y, color.z);

  vec3 normal = vec3_Normalize(p1->accumulated_normal);
  glNormal3f(normal.x,normal.y,normal.z);
  glVertex3f(p1->pos.x, p1->pos.y, p1->pos.z);

  normal = vec3_Normalize(p2->accumulated_normal);
  glNormal3f(normal.x, normal.y, normal.z);
  glVertex3f(p2->pos.x, p2->pos.y, p2->pos.z);

  normal = vec3_Normalize(p3->accumulated_normal);
  glNormal3f(normal.x, normal.y, normal.z);
  glVertex3f(p3->pos.x, p3->pos.y, p3->pos.z);

}

void cloth_DrawShaded(Cloth* cloth)
{
  //Reset normals
  for (int y = 0; y < cloth->numParticlesY; y++)
  {
    for(int x=0; x < cloth->numParticlesX; x++)
    {
      cloth_GetParticle(cloth, x, y)->accumulated_normal.x = 0;
      cloth_GetParticle(cloth, x, y)->accumulated_normal.y = 0;
      cloth_GetParticle(cloth, x, y)->accumulated_normal.z = 0;
    }
  }

  //Create smooth per particle normals by adding up all the (hard) triangle normals that each particle is part of
  for (int x = 0; x < cloth->numParticlesX - 1; x++)
  {
    for (int y = 0; y < cloth->numParticlesY - 1; y++)
    {
      vec3 normal = cloth_CalculateFlatNormal(cloth_GetParticle(cloth,x + 1, y), cloth_GetParticle(cloth,x, y), cloth_GetParticle(cloth,x, y + 1));
      cloth_GetParticle(cloth, x + 1, y)->accumulated_normal = vec3_Add(cloth_GetParticle(cloth, x + 1, y)->accumulated_normal, vec3_Normalize(normal));
      cloth_GetParticle(cloth, x, y)->accumulated_normal = vec3_Add(cloth_GetParticle(cloth, x, y)->accumulated_normal, vec3_Normalize(normal));
      cloth_GetParticle(cloth, x, y + 1)->accumulated_normal = vec3_Add(cloth_GetParticle(cloth, x, y + 1)->accumulated_normal, vec3_Normalize(normal));

      normal = cloth_CalculateFlatNormal(cloth_GetParticle(cloth, x + 1, y + 1), cloth_GetParticle(cloth, x + 1, y), cloth_GetParticle(cloth, x, y + 1));
      cloth_GetParticle(cloth, x + 1, y + 1)->accumulated_normal = vec3_Add(cloth_GetParticle(cloth, x + 1, y + 1)->accumulated_normal, vec3_Normalize(normal));
      cloth_GetParticle(cloth, x + 1, y)->accumulated_normal = vec3_Add(cloth_GetParticle(cloth, x + 1, y)->accumulated_normal, vec3_Normalize(normal));
      cloth_GetParticle(cloth, x, y + 1)->accumulated_normal = vec3_Add(cloth_GetParticle(cloth, x, y + 1)->accumulated_normal, vec3_Normalize(normal));
    }
  }

  glBegin(GL_TRIANGLES);
  for (int x = 0; x < cloth->numParticlesX - 1; x++)
  {
    for (int y = 0; y < cloth->numParticlesY - 1; y++)
    {
      vec3 color;
      color.x = 0;
      color.y = 0;
      color.z = 0;

      //Red and white color is interleaved according to which column number
      if ((x / 10) % 2)
      {
        color.x = 0.6f;
        color.y = 0.2f;
        color.z = 0.2f;
      }
      else
      {
        color.x = 1.0f;
        color.y = 1.0f;
        color.z = 1.0f;
      }
      cloth_DrawTriangle(cloth_GetParticle(cloth,x + 1, y), cloth_GetParticle(cloth,x, y), cloth_GetParticle(cloth,x, y + 1), color);
      cloth_DrawTriangle(cloth_GetParticle(cloth,x + 1, y + 1), cloth_GetParticle(cloth,x + 1, y), cloth_GetParticle(cloth,x, y + 1), color);
    }
  }
  glEnd();
}

void cloth_BallCollision(Cloth* cloth, vec3 center, float radius)
{
  for (int x = 0; x < cloth->numParticlesX; x++)
  {
    for (int y = 0; y < cloth->numParticlesY; y++)
    {
      vec3 v = vec3_Subtract(cloth_GetParticle(cloth, x, y)->pos, center);
      float l = vec3_Magnitude(v);
      if (l < radius)
      {
        particle_OffsetPosition(cloth_GetParticle(cloth, x, y), vec3_Multiply(vec3_Normalize(v), radius - l));
      }

    }
  }

  //std::vector<Particle>::iterator particle;
  //for (particle = particles.begin(); particle != particles.end(); particle++)
  //{
  //  Vec3 v = (*particle).getPos() - center;
  //  float l = v.length();
  //  if (v.length() < radius) // if the particle is inside the ball
  //  {
  //    (*particle).offsetPos(v.normalized()*(radius - l)); // project the particle to the surface of the ball
  //  }
  //}
}

void cloth_TimeStep(Cloth* cloth, vec3 ballPos, float ballRadius)
{
  for (int i = 0; i < CONSTRAINT_ITERATIONS; i++) // iterate over all constraints several times
  {
    for (int j = 0; j < cloth->numConstraints; j++)
    {
      constraint_Satisfy(&(cloth->constraints[j]));
    }
    cloth_BallCollision(cloth, ballPos, ballRadius);
  }

  for (int y = 0; y < cloth->numParticlesY; y++) // iterate over all constraints several times
  {
    for (int x = 0; x < cloth->numParticlesX; x++)
    {
      particle_TimeStep(cloth_GetParticle(cloth, x, y), 0.016f, 0.016f, 0.1f);
    }
  }
}

void cloth_AddForce(Cloth* cloth, vec3 direction)
{
  for (int y = 0; y < cloth->numParticlesY; y++)
  {
    for (int x = 0; x < cloth->numParticlesX; x++)
    {
      particle_AddForce(cloth_GetParticle(cloth, x, y), direction);
    }
  }
}

void cloth_WindForce(Cloth* cloth, vec3 direction)
{
  for (int x = 0; x < cloth->numParticlesX - 1; x++)
  {
    for (int y = 0; y < cloth->numParticlesY - 1; y++)
    {
      cloth_AddWindForceOnTriangle(cloth_GetParticle(cloth,x + 1, y), cloth_GetParticle(cloth,x, y), cloth_GetParticle(cloth,x, y + 1), direction);
      cloth_AddWindForceOnTriangle(cloth_GetParticle(cloth,x + 1, y + 1), cloth_GetParticle(cloth,x + 1, y), cloth_GetParticle(cloth, x, y + 1), direction);
    }
  }
}

#endif // Cloth_h__
