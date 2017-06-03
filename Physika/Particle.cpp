#include "Particle.h"
#include <malloc.h>



void particle_Init(Particle* const a_particle)
{
  a_particle->pos.x = 0;
  a_particle->pos.y = 0;
  a_particle->pos.z = 0;

  a_particle->old_pos.x = 0;
  a_particle->old_pos.y = 0;
  a_particle->old_pos.z = 0;

  particle_ResetAcceleration(&(a_particle->acceleration));

  a_particle->accumulated_normal.x = 0;
  a_particle->accumulated_normal.y = 0;
  a_particle->accumulated_normal.z = 0;

  a_particle->mass = 1;
  a_particle->moveable = true;
}

void particle_Create(Particle** const a_particlePointer)
{
  *a_particlePointer = (Particle*)malloc(sizeof(Particle));
  particle_Init(*a_particlePointer);
}

void particle_AddForce(Particle* const a_particle, vec3 a_vector)
{
  a_particle->acceleration = vec3_Add(a_particle->acceleration, vec3_Divide(a_vector, a_particle->mass));
}


void particle_TimeStep(Particle* const a_particle, float dt, float last_dt, float damping)
{
  if (a_particle->moveable)
  {
    vec3 temp = a_particle->pos;

    vec3 sourceVelocity = vec3_Subtract(a_particle->pos, a_particle->old_pos);
    float frameRatio = (dt / last_dt);
    vec3 normalisedVelocity = vec3_Multiply(sourceVelocity, frameRatio);

    vec3 dampedVelocity = vec3_Multiply(normalisedVelocity, 1 - damping);
    vec3 ATSquared = vec3_Multiply(a_particle->acceleration, TIME_STEP2);

    if (dampedVelocity.x != 0 || dampedVelocity.y != 0 || dampedVelocity.z != 0)
    {
      dampedVelocity = dampedVelocity;
    }
    vec3 finalPos = vec3_Add(a_particle->pos, dampedVelocity);
    finalPos = vec3_Add(finalPos, ATSquared);

    a_particle->old_pos = temp;
    a_particle->pos = finalPos;
    particle_ResetAcceleration(&(a_particle->acceleration));
  }
}

void particle_ResetAcceleration(vec3* a_vector)
{
  a_vector->x = 0;
  a_vector->y = 0;
  a_vector->z = 0;
}

void particle_Move(Particle* const a_particle, vec3 a_vector)
{
  vec3 newPos;
  vec3_Init(&newPos);
  newPos = a_particle->pos;

  newPos = vec3_Add(a_particle->pos, a_vector);

  a_particle->pos = newPos;
}

void particle_MakeUnmoveable(Particle* const a_particle)
{
  a_particle->moveable = false;
}

void particle_OffsetPosition(Particle* const a_particle, vec3 position)
{
  if (a_particle->moveable)
  {
    a_particle->pos = vec3_Add(a_particle->pos, position);
  }
}
