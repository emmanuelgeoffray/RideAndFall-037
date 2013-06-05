#pragma once

#include "chipmunk.h"
#include "ofxChipmunkBaseShape.h"
#include "ofMain.h"


//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------

class ofxChipmunkRect : public ofxChipmunkBaseShape{
	
	public:

		//-----------------------------------------------------------------------------------------			
		ofxChipmunkRect(){
			
		}

		//-----------------------------------------------------------------------------------------
		virtual void setup(float x, float y, float w, float h, float shapeMass, float elasticity, float fricton, bool bShapeStatic){
			setup(x, y, w, h, shapeMass, bShapeStatic);
			setElasticity(elasticity);
			setFriction(fricton);
		}

		//-----------------------------------------------------------------------------------------
		virtual void draw() {
		
			cpBody *body = shape->body;
			cpPolyShape *poly = (cpPolyShape *)shape;
	
			int num = poly->numVerts;
			cpVect *verts = poly->verts;
			
			GLfloat pts[num * 2];
			int k = 0;
			for(int i=0; i<num; i++){
				cpVect v = cpvadd(body->p, cpvrotate(verts[i], body->rot));
				pts[k] = v.x;
				pts[k+1] = v.y;
				k+=2;
			}
				
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, 0, &pts[0]);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		}
		
	protected:
		//-----------------------------------------------------------------------------------------
		virtual void setup(float x, float y, float w, float h, float shapeMass, bool bShapeStatic){
				
			float hw = w/2;
			float hh = h/2;
		
			int num = 4;
			cpVect verts[] = {
				cpv(-hw,-hh),
				cpv(-hw, hh),
				cpv( hw, hh),
				cpv( hw,-hh),
			};
			
			mass	 = shapeMass;
			isStatic = bShapeStatic;
			
			if( isStatic ){
				body = cpBodyNew(INFINITY, INFINITY);
			}else{
				body = cpBodyNew(mass, cpMomentForPoly(mass, num, verts, cpv(0,0) ) );
			}
			
			body->p = cpv(x, y);
			shape = cpPolyShapeNew(body, num, verts, cpv(0,0));
		}
		
};
