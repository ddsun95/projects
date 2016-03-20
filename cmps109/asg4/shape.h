// $Id: shape.h,v 1.7 2014-06-05 16:11:09-07 - - $
// David Sun (ddsun@ucsc.edu) and Mithu Athreya (mathreya@ucsc.edu)

#ifndef __SHAPE_H__
#define __SHAPE_H__

#include <iomanip>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>
using namespace std;

#include "rgbcolor.h"

//
// Shapes constitute a single-inheritance hierarchy, summarized
// here, with the superclass listed first, and subclasses indented
// under their immediate superclass.
//
// shape
//    text
//    ellipse
//       circle
//    polygon
//       rectangle
//          square
//       diamond
//       triangle
//          right_triangle
//          isosceles
//          equilateral
//

class shape;
struct vertex {
   GLfloat xpos; 
   GLfloat ypos;
};
using vertex_list = vector<vertex>;
using shape_ptr = shared_ptr<shape>; 

//
// Abstract base class for all shapes in this system.
//

class shape {
   friend ostream& operator<< (ostream& out, const shape&);
   private:
      shape (const shape&) = delete; // Prevent copying.
      shape& operator= (const shape&) = delete; // Prevent copying.
      shape (shape&&) = delete; // Prevent moving.
      shape& operator= (shape&&) = delete; // Prevent moving.
   protected:
      inline shape(); // Only subclass may instantiate.
   public:
      virtual ~shape() {}
      virtual void draw (const vertex&, const rgbcolor&) const = 0;
      virtual void drawborder (const vertex&, const rgbcolor&) 
                                                    const = 0;
      virtual void displayNr(const vertex&, const rgbcolor&, char) 
                                                        const = 0;
      virtual void show (ostream&) const;
};


//
// Class for printing text.
//

class text: public shape {
   protected:
      void* glut_bitmap_font = nullptr;
      string textdata;
   public:
      text (void* glut_bitmap_font, const string& textdata);
      virtual void draw (const vertex&, const rgbcolor&) const override;
      virtual void drawborder (const vertex&, const rgbcolor&)
                                               const override;
      virtual void displayNr(const vertex&, const rgbcolor&, char) 
                                                   const override;
      virtual void show (ostream&) const override;
};

//
// Classes for ellipse and circle.
//

class ellipse: public shape {
   protected:
      vertex dimension;
   public:
      ellipse (GLfloat width, GLfloat height);
      virtual void draw (const vertex&, const rgbcolor&) const override;
      virtual void drawborder (const vertex&, const rgbcolor&) 
                                               const override;
      virtual void displayNr(const vertex&, const rgbcolor&, char) 
                                                   const override;
      virtual void show (ostream&) const override;
};

class circle: public ellipse {
   public:
      circle (GLfloat diameter);
};

//
// Class polygon.
//

class polygon: public shape {
   protected:
      const vertex_list vertices;
   public:
      polygon (const vertex_list& vertices);
      virtual void draw (const vertex&, const rgbcolor&) const override;
      virtual void drawborder (const vertex&, const rgbcolor&) 
                                               const override;
      virtual void displayNr(const vertex&, const rgbcolor&, char) 
                                                   const override;
      virtual void show (ostream&) const override;
};



//
// Classes rectangle, square, etc.
//

class rectangle: public polygon {
   public:
      rectangle (GLfloat width, GLfloat height);
};

class square: public rectangle {
   public:
      square (GLfloat width);
};

//
// Class diamond
//

class diamond: public polygon {
   public:
      diamond (const GLfloat width, const GLfloat height);
};

//
// Classes triangle, right triangle, and isosceles
//


class triangle: public polygon {
   public:
      triangle(const vertex_list&);
};


class RightTriangle: public triangle {
   public:
      RightTriangle(const GLfloat width, const GLfloat height);
};

class isosceles: public triangle {
   public:
      isosceles(const GLfloat width, const GLfloat height);
};

class equilateral: public triangle {
   public:
      equilateral(const GLfloat width);
};


ostream& operator<< (ostream& out, const shape&);

#endif

