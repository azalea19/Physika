#ifndef vec3_h__
#define vec3_h__


/*******************************************************************************************************************************
* @file vec3.h
* @author Matt Smooker
* @date S1, 2017
* @brief This vec3 class contains all of the functions related to the vec3 structure.
* @description 
*
*******************************************************************************************************************************/

typedef struct vec3
{
  float x, y, z;
}vec3;


void vec3_Create(vec3** const a_vectorPointer);


void vec3_Init(vec3* const a_vector);


float vec3_Magnitude(vec3 a_vector);


vec3 vec3_Normalize(vec3 a_vector);


//Adding is commutative.
vec3 vec3_Add(vec3 a_vector1, vec3 a_vector2);


//Not commutative!
vec3 vec3_Subtract(vec3 a_vector1, vec3 a_vector2);


float vec3_Dot(vec3 a_vector1, vec3 a_vector2);


//Cross product is commutative
// a x b = -(b x a)
vec3 vec3_Cross(vec3 a_vector1, vec3 a_vector2);


vec3 vec3_Multiply(vec3 a_vector, float constant);


vec3 vec3_Divide(vec3 a_vector, float constant);

#endif // vec3_h__
