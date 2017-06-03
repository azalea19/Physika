#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glut.h> 
#include <math.h>
#include "Cloth.h"
#include "vec3.h"
#include "Input.h"
#include "GL/glui.h"

//GLUI setup
GLUI *glui;
GLUI_Spinner* xSpinner;
GLUI_Spinner* ySpinner;
GLUI_Spinner* zSpinner;
GLUI_Checkbox* checkbox;
GLUI_Spinner* gravitySpinner;
GLUI_Listbox* listBox;
GLUI_Button* btn;
GLUI_RadioGroup* radioGroup;
GLUI_RadioGroup* colorRadioGroup;

int window_id;

//Cloth settings
float windX = 0.3f;
float windY = 0;
float windZ = 0;
int lighting = 1;
int clothChoice = 1;
float gravity = -0.03f;
int clothColor = 0;


//Cloth setup
Cloth* cloth;

//Ball setup
vec3 ball_pos = { 50.f, 30.f, 0.f };
float ball_radius = 5.f; 
float ball_time = 0;

//Camera setup 
vec3 startPos = { -40, -40, -120.f };

extern vec3 translation;

void ResetEvent(GLUI_Control* button)
{
  xSpinner->set_float_val(0);
  ySpinner->set_float_val(0);
  zSpinner->set_float_val(0);
  checkbox->set_int_val(1);
  gravitySpinner->set_float_val(-0.3f);
}

void ClothSelectEvent(GLUI_Control* listbox)
{
  cloth_ChangeType(cloth, clothChoice);
}

void InitUI()
{
  glui = GLUI_Master.create_glui_subwindow(window_id);

  GLUI_Panel* mainPanel = glui->add_panel("Options", 1);

  glui->add_statictext_to_panel(mainPanel,"Wind Force");
 
  xSpinner = glui->add_spinner_to_panel(mainPanel,"Direction X", GLUI_SPINNER_FLOAT, &windX);
  xSpinner->set_speed(.5f);

  ySpinner = glui->add_spinner_to_panel(mainPanel, "Direction Y", GLUI_SPINNER_FLOAT, &windY);
  ySpinner->set_speed(.5f);

  zSpinner = glui->add_spinner_to_panel(mainPanel, "Direction Z", GLUI_SPINNER_FLOAT, &windZ);
  zSpinner->set_speed(.5f);

  glui->add_separator_to_panel(mainPanel);

  glui->add_statictext_to_panel(mainPanel,"Lighting");

  checkbox = glui->add_checkbox_to_panel(mainPanel,"Lighting", &lighting);

  glui->add_separator_to_panel(mainPanel);

  glui->add_statictext_to_panel(mainPanel, "Gravity");

  gravitySpinner = glui->add_spinner_to_panel(mainPanel, "Gravity", GLUI_SPINNER_FLOAT, &gravity);
  gravitySpinner->set_speed(.5f);
  gravitySpinner->set_float_limits(-1, 1, GLUI_LIMIT_CLAMP);

  glui->add_separator_to_panel(mainPanel);

  glui->add_statictext_to_panel(mainPanel, "Cloth Type");
  radioGroup = glui->add_radiogroup_to_panel(mainPanel, &clothChoice, -1, ClothSelectEvent);
  glui->add_radiobutton_to_group(radioGroup, "Banner");
  glui->add_radiobutton_to_group(radioGroup, "Curtain");

  glui->add_separator_to_panel(mainPanel);

  glui->add_statictext_to_panel(mainPanel, "Cloth Color");
  colorRadioGroup = glui->add_radiogroup_to_panel(mainPanel, &clothColor, -1, ClothSelectEvent);
  glui->add_radiobutton_to_group(colorRadioGroup, "Black");
  glui->add_radiobutton_to_group(colorRadioGroup, "Blue");
  glui->add_radiobutton_to_group(colorRadioGroup, "Magenta");


  glui->add_separator_to_panel(mainPanel);
  btn = glui->add_button_to_panel(mainPanel, "Reset All", -1, ResetEvent);

  glui->set_main_gfx_window(window_id);
}

void InitSimulation()
{
  //Initialize the cloth with a width, height and the number of particles on X and Y
  cloth_Init(&cloth);
  //Create the constraints between the cloth particles
  cloth_Create(cloth, 100, 100, 80, 80);
  //Add particles to the cloth
  cloth_AddParticles(cloth );
  //Add the constraints between particles
  cloth_AddConstraints(cloth);

}

void display(void)
{

  glui->show();
  //Clears the color and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);

  //Loading the identity matrix is a way to initialize your matrix to the right state 
  //before you multiply further matrices into the matrix stack
  glLoadIdentity();

  //If enabled, use the current lighting parameters to
  //compute the vertex color.
  //Otherwise, simply associate the current color with each vertex
  glEnable(GL_LIGHTING);

  startPos = vec3_Add(vec3_Multiply(translation,0.01f), startPos);
  glTranslatef(startPos.x, startPos.y, startPos.z);

  ball_time++;
  ball_pos.z = (float)cos(ball_time / 50.0) * 60;
  ball_pos.x = (float)sin(ball_time / 50.0) * 20 + 50;

  vec3 direction = { 0,gravity,0.f };
  vec3 windForce = { windX,windY,windZ };

  cloth_AddForce(cloth,direction);
  cloth_WindForce(cloth, windForce);
  cloth_TimeStep(cloth, ball_pos, ball_radius);
  

  glPushMatrix(); // to draw the ball we use glutSolidSphere, and need to draw the sphere at the position of the ball
  glTranslatef(ball_pos.x, ball_pos.y, ball_pos.z); // hence the translation of the sphere onto the ball position
  glColor3f(0.4f, 0.8f, 0.5f);
  glutSolidSphere(ball_radius - 0.1, 50, 50); // draw the ball, but with a slightly lower radius, otherwise we could get ugly visual artifacts of cloth penetrating the ball slightly
  glPopMatrix();

  cloth_DrawShaded(cloth, lighting);

  //glutSwapBuffers swaps the buffers of the current window if double buffered
  glutSwapBuffers();
  //glutPostRedisplay marks the current window as needing to be redisplayed
  glutPostRedisplay();
}

void reshape(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (h == 0)
    gluPerspective(60, (float)w, 1.0, 5000.0);
  else
    gluPerspective(60, (float)w / (float)h, 1.0, 5000.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void init(GLvoid)
{
  //GL primitives can have either flat or smooth shading.
  //Smooth shading,the default, causes the computed colors of vertices to be interpolated as the
  //primitive is rasterized, typically assigning different colors to each resulting pixel fragment.
  glShadeModel(GL_SMOOTH);

  //Specify the red, green, blue, and alpha values used when the color buffers are cleared.
  glClearColor(0.2f, 0.2f, 0.4f, 0.5f);
  glClearDepth(1.0f);

  //If enabled, do depth comparisons and update the depth buffer.
  glEnable(GL_DEPTH_TEST);

  //glDepthFunc specifies the function used to compare each incoming pixel depth value 
  //with the depth value present in the depth buffer. 
  //The comparison is performed only if depth testing is enabled.
  glDepthFunc(GL_LEQUAL);

  //If enabled, have one or more material parameters track the current color.
  glEnable(GL_COLOR_MATERIAL);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  GLfloat lightPos[4] = { -1.0f,1.0f,0.5f,0.0f };
  glLightfv(GL_LIGHT0, GL_POSITION, (GLfloat *)&lightPos);

  glEnable(GL_LIGHT1);
  GLfloat lightAmbient1[4] = { 0.2f,0.2f,0.2f,0.0f };
  GLfloat lightPos1[4] = { 1.0f,0.0f,-0.2f,0.0f };
  GLfloat lightDiffuse1[4] = { 0.5f,0.5f,0.3f,0.0f };

  glLightfv(GL_LIGHT1, GL_POSITION, (GLfloat *)&lightPos1);
  glLightfv(GL_LIGHT1, GL_AMBIENT, (GLfloat *)&lightAmbient1);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, (GLfloat *)&lightDiffuse1);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

  InitSimulation();
  InitUI();
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(1280, 720);

  window_id = glutCreateWindow("Cloth Simulator");

  init();

  GLUI_Master.set_glutKeyboardFunc(keyboard);
  GLUI_Master.set_glutSpecialFunc(arrow_keys);
  GLUI_Master.set_glutMouseFunc(MouseButton);
  GLUI_Master.set_glutReshapeFunc(reshape);
 
  glutDisplayFunc(display);
  glutMotionFunc(MouseMotion);

  GLUI_Master.set_glutIdleFunc(NULL);
  glutMainLoop();
}



