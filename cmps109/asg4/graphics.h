// $Id: graphics.h,v 1.9 2014-05-15 16:42:55-07 - - $

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <memory>
#include <vector>
using namespace std;

#include <GL/freeglut.h>

#include "rgbcolor.h"
#include "shape.h"

class object {
   private:
      shared_ptr<shape> pshape;
      vertex center;
      rgbcolor color;
   public:
      // Default copiers, movers, dtor all OK.
      void setShape(shared_ptr<shape> ptr) {pshape = ptr;}
      void setCenter(vertex cent) {center = cent;}
      void setColor(rgbcolor col) {color = col;}
      void draw() { pshape->draw (center, color); }
      void drawborder(const rgbcolor& bordercolor){ 
         pshape->drawborder(center, bordercolor); 
      }
      void move (GLfloat delta_x, GLfloat delta_y) {
         center.xpos += delta_x;
         center.ypos += delta_y;
      }
      void displayNr(char number){
         pshape->displayNr(center, color, number);
      }
      void setx(float x) { center.xpos = x; }
      void sety(float y) { center.ypos = y; }
      float getx() { return center.xpos; }
      float gety() { return center.ypos; }
};

class mouse {
      friend class window;
   private:
      int xpos {0};
      int ypos {0};
      int entered {GLUT_LEFT};
      int left_state {GLUT_UP};
      int middle_state {GLUT_UP};
      int right_state {GLUT_UP};
   private:
      void set (int x, int y) { xpos = x; ypos = y; }
      void state (int button, int state);
      void draw();
};


class window {
      friend class mouse;
   private:
      static int width;         // in pixels
      static int height;        // in pixels
      static int moveByPix;
      static vector<object> objects;
      static size_t currObj;
      static mouse mus;
      static string borderColor;
   private:
      static void close();
      static void entry (int mouse_entered);
      static void display();
      static void reshape (int width, int height);
      static void keyboard (GLubyte key, int, int);
      static void special (int key, int, int);
      static void motion (int x, int y);
      static void passivemotion (int x, int y);
      static void mousefn (int button, int state, int x, int y);
   public:
      static int borderPix;
      static int getwidth() { return width; }
      static int getheight() { return height; }
      static void push_back (const object& obj) {
                  objects.push_back (obj); }
      static void setwidth (int width_) { width = width_; }
      static void setheight (int height_) { height = height_; }
      static void setMovePix (int pix) { moveByPix = pix; }
      static void moveObject (size_t curr, int deltx, int delty);
      static void setBorderPix (int pix) { borderPix = pix; }
      static void selectCurrentObject (size_t currentIndex);
      static void setBorderColor (string color) { borderColor = color; }
      static void main();
};

#endif

