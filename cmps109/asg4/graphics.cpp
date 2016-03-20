// $Id: graphics.cpp,v 1.11 2014-05-15 16:42:55-07 - - $

#include <iostream>
using namespace std;

#include <GL/freeglut.h>

#include "graphics.h"
#include "util.h"

int window::width = 640; // in pixels
int window::height = 480; // in pixels
int window::moveByPix = 4; // in pixels
int window::borderPix = 4; // in pixels
string window::borderColor = "red";
vector<object> window::objects;
size_t window::currObj = 0;
mouse window::mus;

// Executed when window system signals to shut down.
void window::close() {
   DEBUGF ('g', sys_info::execname() << ": exit ("
           << sys_info::exit_status() << ")");
   exit (sys_info::exit_status());
}

// Executed when mouse enters or leaves window.
void window::entry (int mouse_entered) {
   DEBUGF ('g', "mouse_entered=" << mouse_entered);
   window::mus.entered = mouse_entered;
   if (window::mus.entered == GLUT_ENTERED) {
      DEBUGF ('g', sys_info::execname() << ": width=" << window::width
           << ", height=" << window::height);
   }
   glutPostRedisplay();
}

// Called to display the objects in the window.
void window::display() {
   glClear (GL_COLOR_BUFFER_BIT);
   char num = '0';
   for(size_t i = 0; i < window::objects.size(); ++i){
      object& curr = window::objects.at(i);
      curr.draw();
      if(i == currObj) curr.drawborder(rgbcolor{borderColor});
      if(num <= '9') curr.displayNr(num);
      ++num;
   }
   num = '0';
   mus.draw();
   glutSwapBuffers();
}

// Called when window is opened and when resized.
void window::reshape (int width, int height) {
   DEBUGF ('g', "width=" << width << ", height=" << height);
   window::width = width;
   window::height = height;
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D (0, window::width, 0, window::height);
   glMatrixMode (GL_MODELVIEW);
   glViewport (0, 0, window::width, window::height);
   glClearColor (0.25, 0.25, 0.25, 1.0);
   glutPostRedisplay();
}

void window::moveObject(size_t curr, int deltx, int delty){
   object& current = window::objects.at(curr);
   current.move(deltx, delty);
   if(current.getx() > window::getwidth()) current.setx(0.0);
   if(current.getx() < 0) current.setx(window::getwidth());
   if(current.gety() > window::getheight()) current.sety(0.0);
   if(current.gety() < 0) current.sety(window::getheight());
}


// Executed when a regular keyboard key is pressed.
enum {BS=8, TAB=9, ESC=27, SPACE=32, DEL=127};
void window::keyboard (GLubyte key, int x, int y) {
   DEBUGF ('g', "key=" << (unsigned)key << ", x=" << x << ", y=" << y);
   window::mus.set (x, y);
   switch (key) {
      case 'Q': case 'q': case ESC:{
         window::close();
         break;
      }

      //move selected object left
      case 'H': case 'h':{
         window::moveObject(window::currObj, -moveByPix, 0);
         break;
      }

     //move selected object down
      case 'J': case 'j':{
         window::moveObject(window::currObj, 0, -moveByPix);
         break;
      }

      //move selected object up
      case 'K': case 'k':{
         window::moveObject(window::currObj, 0, moveByPix);
         break;
      }

      //move object right
      case 'L': case 'l': {
         window::moveObject(window::currObj, moveByPix, 0);
         break;
      }

      //select next object
      case 'N': case 'n': case SPACE: case TAB:{
         currObj = (currObj + 1) % window::objects.size();
         break;
      }

      //select previous object
      case 'P': case 'p': case BS: {
         window::currObj = window::currObj == 0 ? 
                     window::objects.size() - 1 : currObj - 1;
         break;
      }

      //select current object
      case '0'...'9':{
         selectCurrentObject((unsigned long) key - '0');
         break;
      }
      default:
         cerr << (unsigned)key << ": invalid keystroke" << endl;
         break;
   }
   glutPostRedisplay();
}

void window::selectCurrentObject(size_t currentIndex){
   if(currentIndex < window::objects.size()) currObj = currentIndex;
}


// Executed when a special function key is pressed.
void window::special (int key, int x, int y) {
   DEBUGF ('g', "key=" << key << ", x=" << x << ", y=" << y);
   window::mus.set (x, y);
   switch (key) {
      case GLUT_KEY_LEFT: //move_selected_object (-1, 0); break;
      case GLUT_KEY_DOWN: //move_selected_object (0, -1); break;
      case GLUT_KEY_UP: //move_selected_object (0, +1); break;
      case GLUT_KEY_RIGHT: //move_selected_object (+1, 0); break;
      case GLUT_KEY_F1: //select_object (1); break;
      case GLUT_KEY_F2: //select_object (2); break;
      case GLUT_KEY_F3: //select_object (3); break;
      case GLUT_KEY_F4: //select_object (4); break;
      case GLUT_KEY_F5: //select_object (5); break;
      case GLUT_KEY_F6: //select_object (6); break;
      case GLUT_KEY_F7: //select_object (7); break;
      case GLUT_KEY_F8: //select_object (8); break;
      case GLUT_KEY_F9: //select_object (9); break;
      case GLUT_KEY_F10: //select_object (10); break;
      case GLUT_KEY_F11: //select_object (11); break;
      case GLUT_KEY_F12: //select_object (12); break;
      default:
         cerr << (unsigned)key << ": invalid function key" << endl;
         break;
   }
   glutPostRedisplay();
}


void window::motion (int x, int y) {
   DEBUGF ('g', "x=" << x << ", y=" << y);
   window::mus.set (x, y);
   glutPostRedisplay();
}

void window::passivemotion (int x, int y) {
   DEBUGF ('g', "x=" << x << ", y=" << y);
   window::mus.set (x, y);
   glutPostRedisplay();
}

void window::mousefn (int button, int state, int x, int y) {
   DEBUGF ('g', "button=" << button << ", state=" << state
           << ", x=" << x << ", y=" << y);
   window::mus.state (button, state);
   window::mus.set (x, y);
   glutPostRedisplay();
}

void window::main () {
   cout << __func__ << "()" << endl;
   static int argc = 0;
   glutInit (&argc, nullptr);
   glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE);
   glutInitWindowSize (window::width, window::height);
   glutInitWindowPosition (128, 128);
   glutCreateWindow (sys_info::execname().c_str());
   glutCloseFunc (window::close);
   glutEntryFunc (window::entry);
   glutDisplayFunc (window::display);
   glutReshapeFunc (window::reshape);
   glutKeyboardFunc (window::keyboard);
   glutSpecialFunc (window::special);
   glutMotionFunc (window::motion);
   glutPassiveMotionFunc (window::passivemotion);
   glutMouseFunc (window::mousefn);
   DEBUGF ('g', "Calling glutMainLoop()");
   glutMainLoop();
}


void mouse::state (int button, int state) {
   switch (button) {
      case GLUT_LEFT_BUTTON: left_state = state; break;
      case GLUT_MIDDLE_BUTTON: middle_state = state; break;
      case GLUT_RIGHT_BUTTON: right_state = state; break;
   }
}

void mouse::draw() {
   static rgbcolor color ("green");
   ostringstream text;
   text << "(" << xpos << "," << window::height - ypos << ")";
   if (left_state == GLUT_DOWN) text << "L"; 
   if (middle_state == GLUT_DOWN) text << "M"; 
   if (right_state == GLUT_DOWN) text << "R"; 
   if (entered == GLUT_ENTERED) {
      void* font = GLUT_BITMAP_HELVETICA_18;
      glColor3ubv (color.ubvec);
      glRasterPos2i (10, 10);
      glutBitmapString (font, (GLubyte*) text.str().c_str());
   }
}

