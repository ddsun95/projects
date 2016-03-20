// $Id: shape.cpp,v 1.7 2014-05-08 18:32:56-07 - - $
// David Sun (ddsun@ucsc.edu) and Mithu Athreya (mathreya@ucsc.edu)

#include <typeinfo>
#include <unordered_map>
#include <cmath>
using namespace std;

#include "shape.h"
#include "util.h"
#include "graphics.h"

static unordered_map<void*,string> fontname {
   {GLUT_BITMAP_8_BY_13       , "Fixed-8x13"    },
   {GLUT_BITMAP_9_BY_15       , "Fixed-9x15"    },
   {GLUT_BITMAP_HELVETICA_10  , "Helvetica-10"  },
   {GLUT_BITMAP_HELVETICA_12  , "Helvetica-12"  },
   {GLUT_BITMAP_HELVETICA_18  , "Helvetica-18"  },
   {GLUT_BITMAP_TIMES_ROMAN_10, "Times-Roman-10"},
   {GLUT_BITMAP_TIMES_ROMAN_24, "Times-Roman-24"},
};

static unordered_map<string,void*> fontcode {
   {"Fixed-8x13"    , GLUT_BITMAP_8_BY_13       },
   {"Fixed-9x15"    , GLUT_BITMAP_9_BY_15       },
   {"Helvetica-10"  , GLUT_BITMAP_HELVETICA_10  },
   {"Helvetica-12"  , GLUT_BITMAP_HELVETICA_12  },
   {"Helvetica-18"  , GLUT_BITMAP_HELVETICA_18  },
   {"Times-Roman-10", GLUT_BITMAP_TIMES_ROMAN_10},
   {"Times-Roman-24", GLUT_BITMAP_TIMES_ROMAN_24},
};

/* returns the vertex list for rectangles */
vertex_list rectVertices(GLfloat width, GLfloat height){
   vertex botleft, botright, topright, topleft;
   botleft.xpos  = 0.0;     botleft.ypos  = 0.0;
   botright.xpos = width;   botright.ypos = 0.0;
   topright.xpos = width;   topright.ypos = height;
   topleft.xpos  = 0.0;     topleft.ypos  = height;
   vertex_list rect{botleft, botright, topright, topleft};
   return rect;
}

/* returns the verted list for diamond */
vertex_list diamondVertices(GLfloat width, GLfloat height){
   vertex midright, topmid, midleft, botmid;
   midright.xpos = 0;         midright.ypos = height / 2;
   topmid.xpos = width / 2;   topmid.ypos = height;
   midleft.xpos = width;      midleft.ypos = height / 2;
   botmid.xpos = width / 2;   botmid.ypos = 0;
   vertex_list diamond{midright, botmid, midleft, topmid};
   return diamond;
}

/* returns the verted list for a right triangle */
vertex_list rightTriangleVertices(GLfloat width, GLfloat height){
   vertex bottom, bottomleft, top;
   bottom.xpos = 0; bottom.ypos = 0;
   bottomleft.xpos = width; bottomleft.ypos = 0;
   top.xpos = 0; top.ypos = height;
   vertex_list rt{bottom, bottomleft, top};
   return rt;
}

/* returns the vertex list for an isosceles triangle */
vertex_list isoscelesVertices(GLfloat width, GLfloat height){
   vertex botleft, botright, topmid;
   botleft.xpos = 0; botleft.ypos = 0;
   botright.xpos = width; botright.ypos = 0;
   topmid.xpos = width / 2; topmid.ypos = height;
   vertex_list isovert{botleft, botright, topmid};
   return isovert;
}

/* returns the vertex list for an equilateral triangle */
vertex_list equilateralVertices(GLfloat width){
   vertex botleft, botright, topmid;
   botleft.xpos = 0; botleft.ypos = 0;
   botright.xpos = width; botright.ypos = 0;
   topmid.xpos = width / 2; topmid.ypos = (int)(sqrt(3) * (width / 2));
   vertex_list equilvert {botleft, botright, topmid};
   return equilvert;
}

ostream& operator<< (ostream& out, const vertex& where) {
   out << "(" << where.xpos << "," << where.ypos << ")";
   return out;
}

shape::shape() {
   DEBUGF ('c', this);
}

text::text (void* glut_bitmap_font, const string& textdata):
      glut_bitmap_font(glut_bitmap_font), textdata(textdata) {
   DEBUGF ('c', this);
}

ellipse::ellipse (GLfloat width, GLfloat height):
                  dimension ({width, height}) {
   DEBUGF ('c', this);
}

circle::circle (GLfloat diameter): ellipse (diameter, diameter) {
   DEBUGF ('c', this);
}


polygon::polygon (const vertex_list& vertices): vertices(vertices) {
   DEBUGF ('c', this);
}

rectangle::rectangle (GLfloat width, GLfloat height): 
                      polygon(rectVertices(width, height)) {
   DEBUGF ('c', this << "(" << width << "," << height << ")");
}

square::square (GLfloat width): rectangle (width, width) {
   DEBUGF ('c', this);
}

/* diamond ctor */
diamond::diamond(const GLfloat width, const GLfloat height):
                    polygon(diamondVertices(width, height)){
   DEBUGF ('c', this << "(" << width << "," << height << ")");
}

/* triangle ctor */
triangle::triangle(const vertex_list& vertices): polygon(vertices){}

/* right triangle ctor */
RightTriangle::RightTriangle(const GLfloat width, const GLfloat height):
                        triangle(rightTriangleVertices(width, height)){}

/* isosceles triangle ctor */
isosceles::isosceles(const GLfloat width, const GLfloat height):
                     triangle(isoscelesVertices(width, height)){}

/* equilateral tirangle ctor */
equilateral::equilateral(const GLfloat width):
                         triangle(equilateralVertices(width)){}

//Code text draw here
void text::draw (const vertex& center, const rgbcolor& color) const {
   DEBUGF ('d', this << "(" << center << "," << color << ")");
   void* font = glut_bitmap_font;
   glClearColor(0.25, 0.25, 0.25, 1.0);
   glColor3ubv(color.ubvec);
   glRasterPos2f(center.xpos, center.ypos);
   glutBitmapString(font, (const unsigned char*)textdata.c_str());
   glEnd();
}

/** we do not need to implement border or object number for text ******/
void text::drawborder(const vertex& center, const rgbcolor& color)
                                                            const{
   DEBUGF ('d', this << "(" << center << "," << color << ")");
   return;
}

void text::displayNr(const vertex& center, const rgbcolor& color, 
                                                   char c) const{
   DEBUGF ('d', this << "(" << center << "," << 
                   c << "," << color << ")");
   return;
}
/**********************************************************************/

void ellipse::draw (const vertex& center, const rgbcolor& color) const {
   DEBUGF ('d', this << "(" << center << "," << color << ")");
   glClearColor(0.25, 0.25, 0.25, 1.0);
   glBegin(GL_POLYGON);
   glEnable(GL_LINE_SMOOTH);
   glColor3ubv(color.ubvec);
   const float delta = 2 * M_PI / 32;
   float width  = dimension.xpos;
   float height = dimension.ypos;
   for(float theta = 0; theta < 2 * M_PI; theta += delta){
      float xpos = width  * cos(theta) + center.xpos;
      float ypos = height * sin(theta) + center.ypos;
      glVertex2f(xpos, ypos);
   }
   glEnd();
}

/* draws the border for an ellipse */
void ellipse::drawborder(const vertex& center, const rgbcolor& color)
                                                               const{
   DEBUGF ('d', this << "(" << center << "," << color << ")");
   glClearColor(0.25, 0.25, 0.25, 1.0);
   glLineWidth(window::borderPix);
   glBegin(GL_LINE_LOOP);
   glEnable(GL_LINE_SMOOTH);
   glColor3ubv(color.ubvec);
   const float delta = 2 * M_PI / 32;
   float width  = dimension.xpos;
   float height = dimension.ypos;
   for(float theta = 0; theta < 2 * M_PI; theta += delta){
      float xpos = width  * cos(theta) + center.xpos;
      float ypos = height * sin(theta) + center.ypos;
      glVertex2f(xpos, ypos);
   }
   glEnd();
}

/* displays the objects number */
void ellipse::displayNr(const vertex& center, const rgbcolor& color, 
                                                      char c) const{
   DEBUGF ('d', this << "(" << center << "," << ")");
   void* font = GLUT_BITMAP_HELVETICA_10;
   string character = "0";
   glClearColor(0.25, 0.25, 0.25, 1.0);
   rgbcolor inverted = color;
   inverted.ubvec[0] = inverted.ubvec[0] xor 0xFF;
   inverted.ubvec[1] = inverted.ubvec[1] xor 0xFF;
   inverted.ubvec[3] = inverted.ubvec[3] xor 0xFF;
   int wdt = glutBitmapLength(font, (GLubyte*) character.c_str());
   int hgt = glutBitmapHeight(font);
   float adjx = center.xpos - wdt / 2.0;
   float adjy = center.ypos - hgt / 4.0;
   glColor3ubv(inverted.ubvec);
   glRasterPos2f(adjx, adjy);
   glutBitmapCharacter(font, c);
   glEnd();

}

void polygon::draw (const vertex& center, const rgbcolor& color) const {
   DEBUGF ('d', this << "(" << center << "," << color << ")");
   glClearColor(0.25, 0.25, 0.25, 1.0);
   glBegin(GL_POLYGON);
   glEnable(GL_LINE_SMOOTH);
   glColor3ubv(color.ubvec);
   float xorig = 0.0;
   float yorig = 0.0;
   for(auto i = vertices.begin(); i != vertices.end(); ++i){
      xorig += i->xpos / vertices.size();
      yorig += i->ypos / vertices.size();
   }
   float xdiff = xorig - center.xpos;
   float ydiff = yorig - center.ypos;
   for(auto i = vertices.begin(); i != vertices.end(); ++i)
      glVertex2f(i->xpos - xdiff, i->ypos - ydiff);
   glEnd();
}

/* draws the polygon's border */
void polygon::drawborder (const vertex& center, const rgbcolor& color)
                                                                const{
   DEBUGF ('d', this << "(" << center << "," << color << ")");
   glClearColor(0.25, 0.25, 0.25, 1.0);
   glLineWidth(window::borderPix);
   glBegin(GL_LINE_LOOP);
   glEnable(GL_LINE_SMOOTH);
   glColor3ubv(color.ubvec);
   float xorig = 0.0;
   float yorig = 0.0;
   for(auto i = vertices.begin(); i != vertices.end(); ++i){
      xorig += i->xpos / vertices.size();
      yorig += i->ypos / vertices.size();
   }
   float xdiff = xorig - center.xpos;
   float ydiff = yorig - center.ypos;
   for(auto i = vertices.begin(); i != vertices.end(); ++i)
      glVertex2f(i->xpos - xdiff, i->ypos - ydiff);
   glEnd();
}

/* displays the polygon's number */
void polygon::displayNr(const vertex& center, const rgbcolor& color, 
                                                      char c) const{
   DEBUGF ('d', this << "(" << center << "," << ")");
   void* font = GLUT_BITMAP_HELVETICA_10;
   string character = "0";
   glClearColor(0.25, 0.25, 0.25, 1.0);
   rgbcolor inverted = color;
   inverted.ubvec[0] = inverted.ubvec[0] xor 0xFF;
   inverted.ubvec[1] = inverted.ubvec[1] xor 0xFF;
   inverted.ubvec[3] = inverted.ubvec[3] xor 0xFF;
   int wdt = glutBitmapLength(font, (GLubyte*) character.c_str());
   int hgt = glutBitmapHeight(font);
   float adjx = center.xpos - wdt / 2.0;
   float adjy = center.ypos - hgt / 4.0;
   glColor3ubv(inverted.ubvec);
   glRasterPos2f(adjx, adjy);
   glutBitmapCharacter(font, c);
   glEnd();
}

void shape::show (ostream& out) const {
   out << this << "->" << demangle (*this) << ": ";
}

void text::show (ostream& out) const {
   shape::show (out);
   out << glut_bitmap_font << "(" << fontname[glut_bitmap_font]
       << ") \"" << textdata << "\"";
}

void ellipse::show (ostream& out) const {
   shape::show (out);
   out << "{" << dimension << "}";
}

void polygon::show (ostream& out) const {
   shape::show (out);
   out << "{" << vertices << "}";
}

ostream& operator<< (ostream& out, const shape& obj) {
   obj.show (out);
   return out;
}
