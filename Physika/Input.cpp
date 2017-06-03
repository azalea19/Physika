#include "Input.h"

int left_mouse_btn_down = 0;
int right_mouse_btn_down = 0;

float lastX;
float lastY;

vec3 translation;

void MouseMotion(int x, int y)
{
  if (left_mouse_btn_down)
  {
    translation.x = x - lastX;
    translation.y = y - lastY;
    printf("Down");
  }
  else
  {
    translation.x = 0;
    translation.y = 0;
  }
}


void MouseButton(int button, int state, int x, int y)
{
  // Respond to mouse button presses.
  // If button1 pressed, mark this state so we know in motion function.

  if (button == GLUT_LEFT_BUTTON)
  {
    left_mouse_btn_down = (state == GLUT_DOWN) ? TRUE : FALSE;
    if (left_mouse_btn_down)
    {
      lastX = (float)x;
      lastY = (float)y;
    }
  }
  if (button == GLUT_RIGHT_BUTTON)
  {
    right_mouse_btn_down = (state == GLUT_DOWN) ? TRUE : FALSE;
  }
}


void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
    //Exit when the escape key is pressed.
  case 27:
    exit(0);
    break;
  default:
    break;
  }
}


void arrow_keys(int a_keys, int x, int y)
{
  switch (a_keys)
  {
    //Switch in to full screen mode
  case GLUT_KEY_UP:
    glutFullScreen();
    break;
    //Switch to windowed mode
  case GLUT_KEY_DOWN:
    glutReshapeWindow(1280, 720);
    break;
  default:
    break;
  }
}