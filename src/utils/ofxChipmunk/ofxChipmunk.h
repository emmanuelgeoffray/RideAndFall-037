#pragma once

#include "chipmunk.h"
#include "ofxChipmunkBaseShape.h"
#include "ofxChipmunkCircle.h"
#include "ofxChipmunkRect.h"
#include "ofxChipmunkLine.h"
#include "ofxChipmunkPoly.h"

#include "ofMain.h"
#include "del_interface.hpp"


static bool bChipMunkInited = false;

class ofxChipmunk {
	
public: 
	
	//----------------------------------------------------------	
	ofxChipmunk() {
		space = NULL;
	}
	
	//---------------------------------------------------------		
	virtual cpSpace* getSpace(){
		return space;
	}
	
	//---------------------------------------------------------
	virtual void setup(int iterations, ofPoint gravity ){
		//check for non zero values			
		assert(iterations);
		
		//init chipmunk - do we need to make sure this is only called once per thread?
		ofxChipmunk::initChipmunk();
		
		
		//create a new space
		space = cpSpaceNew();
		space->iterations = iterations;
		space->gravity = cpv(gravity.x, gravity.y);
		
		//a default hash size - tweaking this can have a big improvement on fps
		//cpSpaceResizeStaticHash(space, 15.0f, 2000);
		//cpSpaceResizeActiveHash(space, 15.0f, 2000);
		
		staticShape.setup(0, 0, 2, 2, 0, 0, 0, true);
		addShape(&staticShape);
		
		//should we do this for multiple instances?
		//cpResetShapeIdCounter();
	}	
	
	//---------------------------------------------------------			
	void setGravity(ofPoint gravity){
		if(space != NULL)space->gravity = cpv(gravity.x, gravity.y);
		else printf("setGravity() - need to call setup first");
	}
	
	//---------------------------------------------------------
	void update(int numSteps = 6, float forceFramerate = 0.0){
		
		//from the docs
		//  void cpSpaceStep(cpSpace *space, cpFloat dt)
		//
		//  Update the space for the given time step. 
		//  Using a fixed time step is highly recommended. 
		//  Doing so will increase the efficiency of the contact persistence, 
		//  requiring an order of magnitude fewer iterations to resolve the collisions in the usual case.
		
		cpFloat dt;
		if( forceFramerate == 0.0 ){
			dt = (1.0f/(ofGetFrameRate() + 1.0)) / numSteps;
		}else{
			dt = (1.0/forceFramerate) / numSteps;
		}
		
		for(int i = 0; i < numSteps; i++){	
			cpSpaceStep(space, dt);
		}
	}
	
	//---------------------------------------------------------
	void setStaticHashSize(float dimensions, int count){
		//check for non zero values
		assert(dimensions);
		assert(count);
		//cpSpaceResizeStaticHash(space, dimensions, count);
	}
	
	//---------------------------------------------------------		
	void setActiveHashSize(float dimensions, int count){
		//check for non zero values
		assert(dimensions);
		assert(count);
		//cpSpaceResizeActiveHash(space, dimensions, count);
	}	
	
	//--------------------------------------------------------- add shape		
	virtual ofxChipmunkBaseShape * addShape(ofxChipmunkBaseShape *baseShape){
	
		if(!baseShape->isShapeStatic()){
			cpSpaceAddBody(space, baseShape->body);			
			cpSpaceAddShape(space, baseShape->shape);
		}else{
			cpSpaceAddShape(space, baseShape->shape);
		}
		baseShape->setSpace(space);
		
		return baseShape;
	}

	//---------------------------------------------------------		
	virtual ofxChipmunkPoly * addPolyShape(ofxChipmunkPoly *poly){
		
		if(!poly->isShapeStatic()){
			cpSpaceAddBody(space, poly->body);			
			for(int i= 0; i < poly->polyShapes.size(); i++)cpSpaceAddShape(space, poly->polyShapes[i]);
		}else{
			for(int i= 0; i < poly->polyShapes.size(); i++)cpSpaceAddShape(space, poly->polyShapes[i]);
		}
		poly->setSpace(space);
		
		return poly;
	}	

	//---------------------------------------------------------	add joint
	virtual void addCollisionFunction(int collisionIdA, int collisionIdB, cpCollisionBeginFunc begin){
		if( space == NULL )return;		
		cpSpaceAddCollisionHandler(space, collisionIdA, collisionIdB, begin, NULL, NULL, NULL, NULL);
	}
	
	
	
	//---------------------------------------------------------		
	virtual void addPivotJoint(ofxChipmunkBaseShape *shapeA, ofxChipmunkBaseShape *shapeB, ofPoint pivotPoint) {
		if( shapeA == NULL )shapeA = &staticShape;
		if( shapeB == NULL )shapeB = &staticShape;		
		
		cpVect pivot;
		
		pivot.x = pivotPoint.x;
		pivot.y = pivotPoint.y;		
		
		cpConstraint * joint  = cpPivotJointNew(shapeA->body, shapeB->body, pivot);
		cpSpaceAddConstraint(space, joint);	
		
		jointPointer * ptr = new jointPointer();
		ptr->setup(joint);
					
		shapeA->joints.push_back(ptr);
		shapeB->joints.push_back(ptr);
		
		shapeA->joints.back()->increaseCount();
		shapeB->joints.back()->increaseCount();
		
	}
	
	
	
//---------------------------------------------------------		
	virtual void addRotaryLimitJoint(ofxChipmunkBaseShape *shapeA, ofxChipmunkBaseShape *shapeB, float minAngle, float maxAngle) {
		if( shapeA == NULL )shapeA = &staticShape;
		if( shapeB == NULL )shapeB = &staticShape;		
						
		cpConstraint * joint = cpRotaryLimitJointNew(shapeA->body, shapeB->body, minAngle, maxAngle);
		cpSpaceAddConstraint(space, joint);	
		
		jointPointer * ptr = new jointPointer();
		ptr->setup(joint);
					
		shapeA->joints.push_back(ptr);
		shapeB->joints.push_back(ptr);
		
		shapeA->joints.back()->increaseCount();
		shapeB->joints.back()->increaseCount();
		
	}
	
	//---------------------------------------------------------		
	virtual void addGear(ofxChipmunkBaseShape *shapeA, ofxChipmunkBaseShape *shapeB, float phase, float ratio) {
		
		if( shapeA == NULL )shapeA = &staticShape;
		if( shapeB == NULL )shapeB = &staticShape;
		
		cpConstraint * joint  = cpGearJointNew(shapeA->body, shapeB->body, phase, ratio);
		cpSpaceAddConstraint(space, joint);	
		
		jointPointer * ptr = new jointPointer();
		ptr->setup(joint);
					
		shapeA->joints.push_back(ptr);
		shapeB->joints.push_back(ptr);
		
		shapeA->joints.back()->increaseCount();
		shapeB->joints.back()->increaseCount();
		
	}

	//---------------------------------------------------------		
	virtual void addRachet(ofxChipmunkBaseShape *shapeA, ofxChipmunkBaseShape *shapeB, float phase, float clickDistance) {
		
		if( shapeA == NULL )shapeA = &staticShape;
		if( shapeB == NULL )shapeB = &staticShape;
		
		cpConstraint * joint  = cpRatchetJointNew(shapeA->body, shapeB->body, phase, clickDistance);
		cpSpaceAddConstraint(space, joint);	
		
		jointPointer * ptr = new jointPointer();
		ptr->setup(joint);
					
		shapeA->joints.push_back(ptr);
		shapeB->joints.push_back(ptr);
		
		shapeA->joints.back()->increaseCount();
		shapeB->joints.back()->increaseCount();
		
	}

	//---------------------------------------------------------		
	virtual void addGrooveJoint(ofxChipmunkBaseShape *shapeA, ofxChipmunkBaseShape *shapeB, ofPoint grooveABodyA, ofPoint grooveBBodyA, ofPoint pivotBodyB){
		
		if( shapeA == NULL )shapeA = &staticShape;
		if( shapeB == NULL )shapeB = &staticShape;
		
		cpVect a1, a2, p;
		a1.x = grooveABodyA.x;
		a1.y = grooveABodyA.y;

		a2.x = grooveBBodyA.x;
		a2.y = grooveBBodyA.y;	
		
		p.x = pivotBodyB.x;
		p.y = pivotBodyB.y;
				
		cpConstraint * joint = cpGrooveJointNew(shapeA->body, shapeB->body, a1, a2, p);
		cpSpaceAddConstraint(space, joint);	
		
		jointPointer * ptr = new jointPointer();
		ptr->setup(joint);
					
		shapeA->joints.push_back(ptr);
		shapeB->joints.push_back(ptr);
		
		shapeA->joints.back()->increaseCount();
		shapeB->joints.back()->increaseCount();
		
	}

	//---------------------------------------------------------		
	virtual void addSpring(ofxChipmunkBaseShape *shapeA, ofxChipmunkBaseShape *shapeB, ofPoint anchor1, ofPoint anchor2, float restLength, float stiffness, float damping) {
		
		if( shapeA == NULL )shapeA = &staticShape;
		if( shapeB == NULL )shapeB = &staticShape;
				
		cpConstraint * joint = cpDampedSpringNew(shapeA->body, shapeB->body, cpv(anchor1.x, anchor1.y), cpv(anchor2.x, anchor2.y), restLength, stiffness, damping);
		cpSpaceAddConstraint(space, joint);	
		
		jointPointer * ptr = new jointPointer();
		ptr->setup(joint);
					
		shapeA->joints.push_back(ptr);
		shapeB->joints.push_back(ptr);
		
		shapeA->joints.back()->increaseCount();
		shapeB->joints.back()->increaseCount();
		
	}

	//---------------------------------------------------------		
	virtual void addSlideJoint(ofxChipmunkBaseShape *shapeA, ofxChipmunkBaseShape *shapeB, ofPoint anchor1, ofPoint anchor2, float minDist, float maxDist) {
		
		if( shapeA == NULL )shapeA = &staticShape;
		if( shapeB == NULL )shapeB = &staticShape;
				
		cpConstraint * joint = cpSlideJointNew(shapeA->body, shapeB->body, cpv(anchor1.x, anchor1.y), cpv(anchor2.x, anchor2.y), minDist, maxDist);
		cpSpaceAddConstraint(space, joint);	
				
		jointPointer * ptr = new jointPointer();
		ptr->setup(joint);
					
		shapeA->joints.push_back(ptr);
		shapeB->joints.push_back(ptr);
		
		shapeA->joints.back()->increaseCount();
		shapeB->joints.back()->increaseCount();
		
	}


	//---------------------------------------------------------		
	virtual void addRotarySpring(ofxChipmunkBaseShape *shapeA, ofxChipmunkBaseShape *shapeB, float restAngle, float stiffness, float damping) {
		
		if( shapeA == NULL )shapeA = &staticShape;
		if( shapeB == NULL )shapeB = &staticShape;
		
		cpConstraint * joint = cpDampedRotarySpringNew(shapeA->body, shapeB->body, restAngle, stiffness, damping);
		cpSpaceAddConstraint(space, joint);	
		
		jointPointer * ptr = new jointPointer();
		ptr->setup(joint);
					
		shapeA->joints.push_back(ptr);
		shapeB->joints.push_back(ptr);
		
		shapeA->joints.back()->increaseCount();
		shapeB->joints.back()->increaseCount();
		
	}


	//---------------------------------------------------------		
	virtual void addMotor(ofxChipmunkBaseShape *shapeA, ofxChipmunkBaseShape *shapeB, float rate) {
		
		if( shapeA == NULL )shapeA = &staticShape;
		if( shapeB == NULL )shapeB = &staticShape;
		
		cpConstraint * joint  = cpSimpleMotorNew(shapeA->body, shapeB->body, rate);
		cpSpaceAddConstraint(space, joint);	
		
		jointPointer * ptr = new jointPointer();
		ptr->setup(joint);
					
		shapeA->joints.push_back(ptr);
		shapeB->joints.push_back(ptr);
		
		shapeA->joints.back()->increaseCount();
		shapeB->joints.back()->increaseCount();
		
	}


	//---------------------------------------------------------		
	virtual void addPinJoint(ofxChipmunkBaseShape *shapeA, ofxChipmunkBaseShape *shapeB, ofPoint anchor1 = 0, ofPoint anchor2 = 0) {
		
		if( shapeA == NULL )shapeA = &staticShape;
		if( shapeB == NULL )shapeB = &staticShape;
				
		cpVect a1, a2;
		a1.x = anchor1.x;
		a1.y = anchor1.y;

		a2.x = anchor2.x;
		a2.y = anchor2.y;		
		
		cpConstraint * joint  = cpPinJointNew(shapeA->body, shapeB->body, a1, a2);
		cpSpaceAddConstraint(space, joint);	
		
		jointPointer * ptr = new jointPointer();
		ptr->setup(joint);
					
		shapeA->joints.push_back(ptr);
		shapeB->joints.push_back(ptr);
		
		shapeA->joints.back()->increaseCount();
		shapeB->joints.back()->increaseCount();
		
	}
	

	
//protected:

	//----------------------------------------------------------	
	static void initChipmunk(){
		if( !bChipMunkInited ){
			cpInitChipmunk();
			printf("chipmunk initialized\n");
			bChipMunkInited = true;
		}
	}
	
	ofxChipmunkRect staticShape;
	
	//----------------------------------------------------------
	cpSpace *space;
};

