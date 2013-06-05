#pragma once

#include "chipmunk.h"

#include "ofxChipmunkBaseShape.h"
#include "ofMain.h"


//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------

class ofxChipmunkLine : public ofxChipmunkBaseShape{
	
	public:

		//-----------------------------------------------------------------------------------------			
		ofxChipmunkLine(){
			
		}

		//-----------------------------------------------------------------------------------------
		virtual void setup(float x1, float y1, float x2, float y2, float shapeMass, float thickness, float elasticity, float fricton, bool bShapeStatic){
			setup(x1, y1, x2, y2, shapeMass, thickness, bShapeStatic);
			setElasticity(elasticity);
			setFriction(fricton);
		}
				
		//-----------------------------------------------------------------------------------------
		ofPoint getPosition(){
			if(body != NULL)return ofPoint(body->p.x, body->p.y);
			return ofPoint();
		}
		
		//-----------------------------------------------------------------------------------------
		virtual void draw(){
		
			cpBody *body = shape->body;
			cpSegmentShape *seg = (cpSegmentShape *)shape;
			cpVect a = cpvadd(body->p, cpvrotate(seg->a, body->rot));
			cpVect b = cpvadd(body->p, cpvrotate(seg->b, body->rot));
			
			ofLine( a.x, a.y, b.x, b.y);
			
		

		}
		
	protected:
		//-----------------------------------------------------------------------------------------
		virtual void setup(float x1, float y1, float x2, float y2, float shapeMass, float thickness, bool bShapeStatic){
						
			mass	 = shapeMass;
			isStatic = bShapeStatic;
			
			if( isStatic ){
				body = cpBodyNew(INFINITY, INFINITY);
			}else{
				body = cpBodyNew(mass, cpMomentForSegment(mass, cpv(x1, y1), cpv(x2, y2)));
			}
			
			shape = cpSegmentShapeNew(body, cpv(x1,y1), cpv(x2,y2), thickness);

		}
		
};
