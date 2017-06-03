#ifndef Input_h__
#define Input_h__


#include <stdio.h>
#include "vec3.h"
#include <GL/freeglut.h>
#include <GL/glut.h> 


/*******************************************************************************************************************************
* @file Input.h
* @author Maddisen Topaz , Matt Smooker, Caroline Chua
* @date S1, 2017
* @brief The input class contains all of the functions related to input handling.
* @description
*
*******************************************************************************************************************************/

void drawMenu(int num);

void lightingMenu(int num);

void mainMenu(int num);

void createMenu(void);

void MouseMotion(int x, int y);

void MouseButton(int button, int state, int x, int y);

void keyboard(unsigned char key, int x, int y);

void arrow_keys(int a_keys, int x, int y);

#endif // Input_h__
