#pragma once

#include "ofMain.h"
//------------------------------
//------------------------------
static bool InsidePolygon( vector <ofPoint> &polygon, ofPoint &p){
  int counter = 0;
  int i;
  double xinters;
  ofPoint p1,p2;
  
  int N = polygon.size();

  p1 = polygon[0];
  for (i=1;i<=N;i++) {
    p2 = polygon[i % N];
    if (p.y > MIN(p1.y,p2.y)) {
      if (p.y <= MAX(p1.y,p2.y)) {
        if (p.x <= MAX(p1.x,p2.x)) {
          if (p1.y != p2.y) {
            xinters = (p.y-p1.y)*(p2.x-p1.x)/(p2.y-p1.y)+p1.x;
            if (p1.x == p2.x || p.x <= xinters)
              counter++;
          }
        }
      }
    }
    p1 = p2;
  }

  if (counter % 2 == 0) return false;
  else return true;
}

//------------------------------
//------------------------------
static bool InsideRect(ofRectangle &rect, ofPoint &p){
	if( p.x < rect.x || p.y < rect.y || p.x > rect.x + rect.width || p.y > rect.y + rect.height ){
		return false;
	}
	return true;
}
//------------------------------
//------------------------------
static float polygonArea(vector <ofPoint> & poly){

	if( poly.size() == 4 ){		
		return 0.5 * ( (poly[1].y-poly[3].y)*poly[0].x+(poly[2].y-poly[0].y)*poly[1].x+(poly[3].y-poly[1].y)*poly[2].x+(poly[0].y-poly[2].y)*poly[3].x );
	}

	return 0.0;
}
