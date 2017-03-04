#ifndef vec3_h__
#define vec3_h__


typedef struct vec3
{
  float x, y, z;
}vec3;

//The const* ensure the vector data is not modified
//The additional const ensures the vector pointer is not modified
//Preventing potential memory corruption within the function
//the const modifier applies to the thing to its immediate left

//Used like a constructor to initialize a vec3 pointer
//Pass the address (**) to allocate space
//Example:
//vec3* somevec;
//vec3_Create(&somevec);
void vec3_Create(vec3** const a_vectorPointer);


//This function can be called to initialize a
//vector on the stack
//vec3 somevec;
//vec3_Init(&somevec);
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
