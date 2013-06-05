#pragma once

#include "chipmunk.h"
#include "ofMain.h"

class jointPointer{
	public:
		jointPointer(){
			count = 0;
			dead  = false;
		}
		
		void setup(jointPointer * theJoint){
			joint = theJoint->joint;
			count = theJoint->count;
			dead  = theJoint->dead;
			increaseCount();
		}
		
		void setup(cpConstraint * jointIn){
			joint = jointIn;
			count = 0;
			dead = false;
		}
		
		jointPointer * increaseCount(){
			count++;
			//printf("count is %i\n", count);
			return this;
		}
		
		void kill(){
			if( count > 0 )count--;
				//printf("count is %i \n", count);
			if( count == 0 && dead == false){
				dead = true;
				//printf("about to free\n");
				cpConstraintFree(joint);
				joint = NULL;
				//printf("killing\n");
			}
		}
		
		cpConstraint * joint;
		bool dead;
		int count;
};

static const GLfloat springVAR[] = {
	0.00f, 0.0f,
	0.20f, 0.0f,
	0.25f, 3.0f,
	0.30f,-6.0f,
	0.35f, 6.0f,
	0.40f,-6.0f,
	0.45f, 6.0f,
	0.50f,-6.0f,
	0.55f, 6.0f,
	0.60f,-6.0f,
	0.65f, 6.0f,
	0.70f,-3.0f,
	0.75f, 6.0f,
	0.80f, 0.0f,
	1.00f, 0.0f,
};
static const int springVAR_count = sizeof(springVAR)/sizeof(GLfloat)/2;

class ofxChipmunkBaseShape{
	
public:
	
	ofxChipmunkBaseShape(){
		shape		= NULL;
		body		= NULL;
		space		= NULL;
		
		isStatic	= false;
		dead		= false;
		alive		= false;
		mass		= 1.0;
		e			= 0.0;
		f			= 1.0;
	}
	
	~ofxChipmunkBaseShape() {
		
	}
	
	virtual void kill() {
	
		//printf("\njoints is %i\n", joints.size());
		if(space != NULL && joints.size()){
			
			for(int i =0; i < joints.size(); i++){
				if( joints[i]->joint != NULL){
					cpSpaceRemoveConstraint(space, joints[i]->joint);
					joints[i]->kill();
					//cpJointFree(joints[i]);
					//joints[i] = NULL;
					//printf("freeing joint %i\n", i);
				}else{
					//printf("joint %i already freed\n", i);
				}
			}
			joints.clear();
		}
		
		if(space != NULL && shape != NULL){
			cpSpaceRemoveShape(space, shape);
			cpShapeFree(shape);
			//printf("freeing shape \n");

			shape = NULL;
		}
		
		if(space != NULL && body != NULL){
			cpSpaceRemoveBody(space, body);
			cpBodyFree(body);
			//printf("freeing body \n");
			
			body == NULL;
		}
		
		if( alive ){
			alive = false;
			dead  = true;
		}
	}
	
	virtual bool isShapeStatic(){
		return isStatic;
	}	
	
	virtual void setRotation(float angleDeg){
		if( body!= NULL){
			float angleRad = angleDeg * DEG_TO_RAD;
			cpVect angle;
			angle.x = cos(angleRad);
			angle.y = sin(angleRad);
			body->rot = angle;
		}
	}
	
	virtual void setCollisionID(int id){
		if(shape != NULL){
			shape->collision_type = id;
		}	
	}

	virtual void setLayerID(int id){
		if(shape != NULL){
			shape->layers = id;
		}	
	}

	virtual void setGroupID(int id){
		if(shape != NULL){
			shape->group = id;
		}	
	}
	
	virtual void setSpace(cpSpace* spaceToUse){
		space = spaceToUse;
	}
	
	virtual void setElasticity(float elasticity){
		e = elasticity;
		if(shape != NULL){
			shape->e = e;
		}
	}
	
	virtual void setFriction(float friction){
		f = friction;
		if(shape != NULL){
			shape->u = f;
		}
	}
	
	virtual ofPoint getPosition() {
		if(body != NULL)return ofPoint(body->p.x, body->p.y);
		return ofPoint();
	}
	
	
	virtual void drawSpring(cpDampedSpring *spring, cpBody *body_a, cpBody *body_b){
		cpVect a = cpvadd(body_a->p, cpvrotate(spring->anchr1, body_a->rot));
		cpVect b = cpvadd(body_b->p, cpvrotate(spring->anchr2, body_b->rot));

		glPointSize(5.0f);
		glBegin(GL_POINTS); {
			glVertex2f(a.x, a.y);
			glVertex2f(b.x, b.y);
		} glEnd();

		cpVect delta = cpvsub(b, a);

		glVertexPointer(2, GL_FLOAT, 0, springVAR);
		glPushMatrix(); {
			GLfloat x = a.x;
			GLfloat y = a.y;
			GLfloat cos = delta.x;
			GLfloat sin = delta.y;
			GLfloat s = 1.0f/cpvlength(delta);

			const GLfloat matrix[] = {
				 cos,  sin, 0.0f, 0.0f,
				-sin*s,  cos*s, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 1.0f,
					 x,    y, 0.0f, 1.0f,
			};
			
			glMultMatrixf(matrix);
			glDrawArrays(GL_LINE_STRIP, 0, springVAR_count);
		} glPopMatrix();
	}	
	
	virtual void drawConstraint(cpConstraint *constraint){
		cpBody *body_a = constraint->a;
		cpBody *body_b = constraint->b;

		//const cpConstraintClass *klass = constraint->klass;
		const cpConstraintClass *klass = constraint->CP_PRIVATE(klass);
		if(klass == cpPinJointGetClass()){
			cpPinJoint *joint = (cpPinJoint *)constraint;
		
			cpVect a = cpvadd(body_a->p, cpvrotate(joint->anchr1, body_a->rot));
			cpVect b = cpvadd(body_b->p, cpvrotate(joint->anchr2, body_b->rot));

			glPointSize(5.0f);
			glBegin(GL_POINTS); {
				glVertex2f(a.x, a.y);
				glVertex2f(b.x, b.y);
			} glEnd();

			glBegin(GL_LINES); {
				glVertex2f(a.x, a.y);
				glVertex2f(b.x, b.y);
			} glEnd();
		} else if(klass == cpSlideJointGetClass()){
			cpSlideJoint *joint = (cpSlideJoint *)constraint;
		
			cpVect a = cpvadd(body_a->p, cpvrotate(joint->anchr1, body_a->rot));
			cpVect b = cpvadd(body_b->p, cpvrotate(joint->anchr2, body_b->rot));

			glPointSize(5.0f);
			glBegin(GL_POINTS); {
				glVertex2f(a.x, a.y);
				glVertex2f(b.x, b.y);
			} glEnd();

			glBegin(GL_LINES); {
				glVertex2f(a.x, a.y);
				glVertex2f(b.x, b.y);
			} glEnd();
		} else if(klass == cpPivotJointGetClass()){
			cpPivotJoint *joint = (cpPivotJoint *)constraint;
		
			cpVect a = cpvadd(body_a->p, cpvrotate(joint->anchr1, body_a->rot));
			cpVect b = cpvadd(body_b->p, cpvrotate(joint->anchr2, body_b->rot));

			glPointSize(10.0f);
			glBegin(GL_POINTS); {
				glVertex2f(a.x, a.y);
				glVertex2f(b.x, b.y);
			} glEnd();
		} else if(klass == cpGrooveJointGetClass()){
			cpGrooveJoint *joint = (cpGrooveJoint *)constraint;
		
			cpVect a = cpvadd(body_a->p, cpvrotate(joint->grv_a, body_a->rot));
			cpVect b = cpvadd(body_a->p, cpvrotate(joint->grv_b, body_a->rot));
			cpVect c = cpvadd(body_b->p, cpvrotate(joint->anchr2, body_b->rot));

			glPointSize(5.0f);
			glBegin(GL_POINTS); {
				glVertex2f(c.x, c.y);
			} glEnd();
			
			glBegin(GL_LINES); {
				glVertex2f(a.x, a.y);
				glVertex2f(b.x, b.y);
			} glEnd();
		} else if(klass == cpDampedSpringGetClass()){
			drawSpring((cpDampedSpring *)constraint, body_a, body_b);
		} else {
	//		printf("Cannot draw constraint\n");
		}
	}

	
	virtual void drawJoints(){
		if( joints.size() ){
			for(int i = 0; i < joints.size(); i++){

				cpConstraint * joint = (cpConstraint *)joints[i]->joint;
				
				if (joint != NULL){				
					drawConstraint(joint);
				}
				
//				cpPinJoint * joint = (cpPinJoint *)joints[i]->joint;
//				
//				if (joint != NULL){
//					//printf( pinJoint->joint
//												
//					cpBody * bodyA = joint->constraint.a;
//					cpBody * bodyB = joint->constraint.b;
//					
//					cpVect a = cpvadd(bodyA->p, cpvrotate(joint->anchr1, bodyA->rot));
//					cpVect b = cpvadd(bodyB->p, cpvrotate(joint->anchr2, bodyB->rot));
//										
//					//printf( "%f %f \n", pinJoint->joint.a->p.x,  pinJoint->joint.a->p.y );
//					//printf( "%f %f \n", pinJoint->joint.b->p.x,  pinJoint->joint.b->p.y );
//					
//					ofPushStyle();
//					ofSetColor(0xFF00FF);
//					ofLine(a.x, a.y, b.x, b.y);
//					ofPopStyle();
//				}
			}
		}
	}

	virtual void setPosition(float x, float y) {
		if(body != NULL){
			body->p.x = x;
			body->p.y = y;
		}
	}
	
	vector <jointPointer *> joints;
	cpShape * shape;	
	cpBody * body;
	cpSpace * space;
	
	bool dead, alive;
	
protected:
	bool isStatic;
	float mass;
	float e, f;
	
};
