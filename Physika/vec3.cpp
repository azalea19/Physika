#include "vec3.h"
#include <stdlib.h>
#include <math.h>

void vec3_Init(vec3* const a_vector)
{
  a_vector->x = 0;
  a_vector->y = 0;
  a_vector->z = 0;
}

void vec3_Create(vec3** const a_vectorPointer)
{
  *a_vectorPointer = (vec3*)malloc(sizeof(vec3));
  vec3_Init(*a_vectorPointer);
}


float vec3_Magnitude(vec3 a_vector)
{
  return (float)sqrt(a_vector.x*a_vector.x + a_vector.y*a_vector.y + a_vector.z*a_vector.z);
}

vec3 vec3_Normalize(vec3 a_vector)
{
  float length;
  length = vec3_Magnitude(a_vector);

  a_vector.x /= length;
  a_vector.y /= length;
  a_vector.z /= length;

  return a_vector;
}

vec3 vec3_Add(vec3 a_vector1, vec3 a_vector2)
{
  vec3 sum;
  vec3_Init(&sum);

  sum.x = a_vector1.x + a_vector2.x;
  sum.y = a_vector1.y + a_vector2.y;
  sum.z = a_vector1.z + a_vector2.z;

  return sum;
}

vec3 vec3_Subtract(vec3 a_vector1, vec3 a_vector2)
{
  vec3 sum;
  vec3_Init(&sum);

  sum.x = a_vector1.x - a_vector2.x;
  sum.y = a_vector1.y - a_vector2.y;
  sum.z = a_vector1.z - a_vector2.z;

  return sum;
}

float vec3_Dot(vec3 a_vector1, vec3 a_vector2)
{
  vec3 result;
  vec3_Init(&result);

  result.x = a_vector1.x * a_vector2.x;
  result.y = a_vector1.y * a_vector2.y;
  result.z = a_vector1.z * a_vector2.z;

  return result.x + result.y + result.z;
}

vec3 vec3_Cross(vec3 a_vector1, vec3 a_vector2)
{
  vec3 cross;
  vec3_Init(&cross);
                      
  cross.x = (a_vector1.y*a_vector2.z - a_vector1.z*a_vector2.y);
  cross.y = (a_vector1.x*a_vector2.z - a_vector1.z*a_vector2.x);
  cross.z = (a_vector1.x*a_vector2.y - a_vector1.y*a_vector2.x);

  return cross;
}

vec3 vec3_Multiply(vec3 a_vector, float constant)
{
  a_vector.x *= constant;
  a_vector.y *= constant;
  a_vector.z *= constant;
  return a_vector;
}

vec3 vec3_Divide(vec3 a_vector, float constant)
{
  a_vector.x /= constant;
  a_vector.y /= constant;
  a_vector.z /= constant;
  return a_vector;
}
