// $Id: interp.cpp,v 1.18 2015-02-19 16:50:37-08 - - $

#include <memory>
#include <string>
#include <vector>
#include <map>
using namespace std;

#include <GL/freeglut.h>

#include "debug.h"
#include "interp.h"
#include "shape.h"
#include "util.h"

unordered_map<string,interpreter::interpreterfn>
interpreter::interp_map {
   {"define" , &interpreter::do_define },
   {"draw"   , &interpreter::do_draw   },
   {"moveby" , &interpreter::doMoveBy  },
   {"border" , &interpreter::doBorder  },
};

unordered_map<string,interpreter::factoryfn>
interpreter::factory_map {
   {"text"           , &interpreter::make_text           },
   {"ellipse"        , &interpreter::make_ellipse        },
   {"circle"         , &interpreter::make_circle         },
   {"polygon"        , &interpreter::make_polygon        },
   {"rectangle"      , &interpreter::make_rectangle      },
   {"square"         , &interpreter::make_square         },
   {"equilateral"    , &interpreter::makeEquilateral     },
   {"isosceles"      , &interpreter::makeIsosceles       },
   {"right_triangle" , &interpreter::makeRightTriangle   },
   {"diamond"        , &interpreter::makeDiamond         },
   {"triangle"       , &interpreter::makeTriangle        },
};

interpreter::shape_map interpreter::objmap;

static unordered_map<string,void*> fontcode {
   {"Fixed-8x13"    , GLUT_BITMAP_8_BY_13       },
   {"Fixed-9x15"    , GLUT_BITMAP_9_BY_15       },
   {"Helvetica-10"  , GLUT_BITMAP_HELVETICA_10  },
   {"Helvetica-12"  , GLUT_BITMAP_HELVETICA_12  },
   {"Helvetica-18"  , GLUT_BITMAP_HELVETICA_18  },
   {"Times-Roman-10", GLUT_BITMAP_TIMES_ROMAN_10},
   {"Times-Roman-24", GLUT_BITMAP_TIMES_ROMAN_24},
};

interpreter::~interpreter() {
   for (const auto& itor: objmap) {
      cout << "objmap[" << itor.first << "] = "
           << *itor.second << endl;
   }
}

void interpreter::interpret (const parameters& params) {
   DEBUGF ('i', params);
   param begin = params.cbegin();
   string command = *begin;
   auto itor = interp_map.find (command);
   if (itor == interp_map.end()) throw runtime_error ("syntax error");
   interpreterfn func = itor->second;
   func (++begin, params.cend());
}

void interpreter::doMoveBy(param begin, param end){
   DEBUGF ('f', range (begin, end));
   if(end - begin != 1) throw runtime_error("syntax error");
   int pixels = stoi(*begin);
   if(pixels < 0) throw runtime_error("positive move value");
   window::setMovePix(pixels);
}

void interpreter::doBorder(param begin, param end){
   DEBUGF ('f', range (begin, end));
   if(end - begin != 2) throw runtime_error("syntax error");
   string borderColor = *begin++;
   int borderWidth = stoi(*begin);
   if(borderWidth < 0) throw runtime_error("positive border value");
   window::setBorderColor(borderColor);
   window::setBorderPix(borderWidth);
}

void interpreter::do_define (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   string name = *begin;
   objmap.emplace (name, make_shape (++begin, end));
}


void interpreter::do_draw (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   if (end - begin != 4) throw runtime_error ("syntax error");
   string name = begin[1];
   shape_map::const_iterator itor = objmap.find (name);
   if (itor == objmap.end())
      throw runtime_error (name + ": no such shape");
   vertex where {from_string<GLfloat> (begin[2]),
                 from_string<GLfloat> (begin[3])};
   rgbcolor color {begin[0]};
   object draw;
   draw.setShape(itor->second);
   draw.setCenter(where);
   draw.setColor(color);
   window::push_back(draw);
}

shape_ptr interpreter::make_shape (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   string type = *begin++;
   auto itor = factory_map.find(type);
   if (itor == factory_map.end()) {
      throw runtime_error (type + ": no such shape");
   }
   factoryfn func = itor->second;
   return func (begin, end);
}

shape_ptr interpreter::make_text (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   if(fontcode.find(*begin) == fontcode.end())
      throw runtime_error("no such font");
   void* font = fontcode.find(*begin++)->second;
   string data;
   for(auto i = begin; i != end; ++i) data += *i + " ";
   if(data.size() > 0 && data.back() == ' ') data.pop_back();
   return make_shared<text>(font, data);
}

shape_ptr interpreter::make_ellipse (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   if(end - begin != 2)
      throw runtime_error("specify only length, width");
   float width  = stof(*begin++) / 2;
   float length = stof(*begin) / 2;
   return make_shared<ellipse> (width, length);
}

shape_ptr interpreter::make_circle (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   float radius = stof(*begin) / 2;
   return make_shared<circle> (radius);
}

shape_ptr interpreter::make_polygon (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   if(end - begin < 6) 
      throw runtime_error("specify at least 3 points");
   else if((end - begin) % 2 != 0) 
      throw runtime_error("must have even number of points");
   vertex_list vertices;
   while(begin != end){
      vertex v;
      v.xpos = stof(*begin++);
      v.ypos = stof(*begin++);
      vertices.push_back(v);
   }
   return make_shared<polygon> (vertices);
}

shape_ptr interpreter::make_rectangle (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   if(end - begin != 2)
      throw runtime_error("specify only length, width");
   float width  = stof(*begin++);
   float length = stof(*begin);
   return make_shared<rectangle> (width, length);
}

shape_ptr interpreter::make_square (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   if(end - begin != 1)
      throw runtime_error("specify only length");
   return make_shared<square> (stof(*begin));
}


shape_ptr interpreter::makeTriangle(param begin, param end){
   DEBUGF ('f', range (begin, end));
   if(end - begin != 6) throw runtime_error("syntax error");
   vertex_list vertices;
   while(begin != end){
      vertex v;
      v.xpos = stof(*begin++);
      v.ypos = stof(*begin++);
      vertices.push_back(v);
   }
   return make_shared<triangle>(vertices);
}

shape_ptr interpreter::makeEquilateral(param begin, param end){
   DEBUGF ('f', range (begin, end));
   if(end - begin != 1) throw runtime_error("syntax error");
   float width = stof(*begin);
   return make_shared<equilateral>(width);
}

shape_ptr interpreter::makeIsosceles(param begin, param end){
   DEBUGF ('f', range (begin, end));
   if(end - begin != 2) throw runtime_error("syntax error");
   float width = stof(*begin++);
   float height = stof(*begin);
   return make_shared<isosceles>(width, height);
}

shape_ptr interpreter::makeRightTriangle(param begin, param end){
   if(end - begin != 2) throw runtime_error("syntax error");
   float width = stof(*begin++);
   float height = stof(*begin);
   return make_shared<RightTriangle>(width, height);
}

shape_ptr interpreter::makeDiamond(param begin, param end){
   DEBUGF ('f', range (begin, end));
   if(end - begin != 2) throw runtime_error("specify width, height");
   float width = stof(*begin++);
   float height = stof(*begin);
   return make_shared<diamond>(width, height);
}

