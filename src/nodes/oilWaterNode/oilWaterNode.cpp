/*
 *  oilWaterNode.cpp
 *  ofxFernCameraDemo
 *
 *  Created by theo on 17/02/2010.
 *
 * Copyright Theodore Watson 2010
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 */

#include "oilWaterNode.h"
#include "handTextManager.h"

extern handTextManager leftHand1;
extern handTextManager rightHand1;

static bool xSort(oilParticle pA, oilParticle pB){
	if( pA.pos.x < pB.pos.x ){
		return true;
	}
	
	return false;
}

static bool rSort(oilParticle pA, oilParticle pB){
	if( pA.radius < pB.radius ){
		return true;
	}
	
	return false;
}


float lastTimeTriggered;

//-----------------------------------
oilWaterNode::oilWaterNode(){
	maxBoundsY = 300;
	minBoundsY = -300;
}

//--------------------------------------------------------------
void oilWaterNode::setup(ofPoint position, float radius){	

	//TODO: try the horizontal line with particles
	//TODO: texture variety for particles
	//TODO: move drawing to individual particles

	setupName("oilNode");
	setPosition(position.x, position.y, position.z);
	
	state = NODE_INACTIVE;
	killState = NODE_UNKILLABLE;

	nodeRadius      = radius;
	int numBalls	= 1500;
	repulsionDist	= 29.0; 
	repulsionForce	= 1.0;
	scale			= ofVec3f(1.0,1,1);
	nPct			= 0.0;
	
	lastTimeTriggered = 0.0;
	
	textDone		= false;
	
	oilWaterText.clear();
	oilWaterText.push_back("Two groups, as one.\nfight to be two.\n");
	oilWaterText.push_back("Their differences subtle\nas two shades of blue.\n");
	oilWaterText.push_back("We force them together\nand drive them apart.\n");
	oilWaterText.push_back("Stuck in their ways\nthey struggle to part.\n");
	currentText = 0;
	bPrevFrameRiseFall = bRise;
	
//	vector <riseFallString> str = textStackFromXml::loadFromXml("assets/text/riseFallText.xml", "oilWaterNode");
//	nodeText.setup( ofPoint(pos.x-400, pos.y-200, pos.z - 30), str);
	
	tex1.loadImage("assets/nodes/node07-oil/both/particleTex_01.png");
	tex2.loadImage("assets/nodes/node07-oil/both/particleTex_02.png");
	
	for(int i = 0; i < numBalls; i++){
		oilParticle tmpBall;

		tmpBall.radius = powf( (ofRandom(0.0, 100.0)/100.0), 2) * 9.0 + 5.0;
		float zShift   = ofMap(tmpBall.radius, 15, 3, 0, 1);
	
		tmpBall.setPosition( pos.x + ofRandom(-radius, radius ), pos.y + ofRandom(-radius, radius), pos.z + zShift );
		tmpBall.setVelocity( ofRandom(-2, 2), ofRandom(-2, 2) );
		tmpBall.setFriction(0.97485);
		tmpBall.type = ( i%2 );
		
		if( tmpBall.type == 0 ){
			tmpBall.setTexturePtr(&tex2.getTextureReference());
		}else{
			tmpBall.setTexturePtr(&tex1.getTextureReference());
		}
		
		//add the new ball into our array (vector) 
		balls.push_back(tmpBall);
	}	

}

//-----------------------------------

//If you want to return this element (So that all drawing happens inside this class's draw() do this
//otherwise return your elements vector so that each element is depth sorted and drawn seperately
vector <baseElement *> oilWaterNode::getElements(){
	vector <baseElement *> elementList;
		
	for(int k = 0; k < balls.size(); k++){
		elementList.push_back(&balls[k]);
	}
	
	return elementList;
} 
	
//---------------- 
bool oilWaterNode::shouldPersonWait(){
	return (state == NODE_ACTIVATING);
}

//----------------
void oilWaterNode::checkState(float targetX, float targetY, float targetZ){
	ofVec3f dist(targetX-pos.x, targetY-pos.y, 0);
	
	float checkY = 0.0;
	if( pos.y > myPerson.pos.y ){
		checkY = -ofGetHeight()/1.4;
	}else{
		checkY = ofGetHeight()/1.4;
	}
	
	bool bFirst = (currentText == 0);
	bool nextText = false;
	
	float vDist = fabs( dist.y );

	if( fabs( targetY - (pos.y+checkY) ) < 100 && bFirst && state == NODE_INACTIVE  ){
		nextText = true;
	}else if( vDist < 500 && state == NODE_ACTIVE && currentText == 1){
		nextText = true;
	}else if(  vDist < 500 && state != NODE_INACTIVE && currentText > 0 && bRise != bPrevFrameRiseFall ){
		nextText = true;
	}
	
	if( nextText && currentText < oilWaterText.size() ){
	
		if( currentText%2 == 0 ){
			//leftHand1.setPosY( ofRandom(-200+ofGetHeight()/2, 200+ofGetHeight()/2) );
			leftHand1.triggerSideText(oilWaterText[currentText], 4.0);
		}else{
			//leftHand1.setPosY( ofRandom(-200+ofGetHeight()/2, 200+ofGetHeight()/2) );		
			rightHand1.triggerSideText(oilWaterText[currentText], 4.0);
		}
		
		currentText++;
		textDone = true;
	}
	
	if( fabs( dist.y ) < 80 ){
		if( state == NODE_INACTIVE ){
			setState(NODE_ACTIVATING);
			nodeText.activate();
		}
	}
	
	bPrevFrameRiseFall = bRise;
}	
	
//----------------
void oilWaterNode::update(){

	repulsionDist	    = 29.0; 
	if( bRise ){
		repulsionDist	= 30.0; 
	}else{
		repulsionDist	= 30.0; 
	}

//	nodeText.update(0.04);	
	//printf("state is %s\n", nodeStateToStr(state).c_str());
	
	if(state == NODE_ACTIVATING){
		nPct += 0.03;
		if( nPct > 1.0 ){
			nPct = 1.0;
			state = NODE_ACTIVE;
			globalSnd.play("OIL_WATER_BURST", 0.9, 1.0, 0.0);
		}
	}
	
	if( nPct >= 0.9 ){
		updateActive();
	}else{
		updateActive();		
		updateInactive();
	}
	
}


//--------------------------------------------------------------
void oilWaterNode::updateInactive(){
			
		float radSize = nodeRadius*0.6;

		ofVec2f pos2d = ofVec2f(pos.x, pos.y);

		for(int i = 0; i < balls.size(); i++){
			float distFromCenterSq = ofVec2f( balls[i].pos - pos2d ).lengthSquared() +  balls[i].radius*balls[i].radius;
			
			if( distFromCenterSq > radSize*radSize){
				float dist = sqrt( distFromCenterSq );
				
				float amntOver = dist - radSize;
				ofVec3f norm = pos2d-balls[i].pos;
				norm.normalize();
				balls[i].pos += norm * amntOver;
				
				balls[i].vel *= 0.3;
				balls[i].vel += norm;
				
			}
			balls[i].update();
		}
}

//--------------------------------------------------------------
void oilWaterNode::updateActive(){
	
	//TODO: Figure put when particles are not on screen
	//IF NONE OUR - PAUSE THE UPDATING OF PARTICLES
	
	//TODO: AVOID [] operator - for each create a reference and work with that
	//8% CPU!!!
	
	//TODO: fabs using 2% CPU
	
	//for particle vs particle each particle needs to check itself against every other particle
	//this is very slow as for 10 particles you have 100 calculations for 100 particles you have 10,000 calculations
	//it is called an n-squared problem
	
	//so we try to be smart - we sort our vector of particles by X
	//so they go in order from smallest x pos to largest pos.
	//we then will check the difference in x pos between our particle and the one to be checked if it is greater than a certain amount we can stop checking all other particles
	//as we know that they will be getting further and further away from us
	
	sort(balls.begin(), balls.end(), xSort);
	
	//lets create some vars here which we will use a lot in the loop
	float ppDist;
	float frcPct; 
	ofVec2f repulsionAmnt;
	ofVec2f deltaVec;
	
	bool soundFired = false;
	
	//we will work with squared values as much as possible - as the sqrt in ofVec2f::length() is slow
	float repulsionDistTmp = repulsionDist;
	
	float repulsionDistSquared =  repulsionDist*repulsionDist;
	float ppDistSquared;
	
	float maxVel = 8.0;
	
	//OUR REPEL POINT
	ofVec2f mousePt(myPerson.pos.x, myPerson.pos.y);
	if( bRise ){
		mousePt.y += 100.0;
	}else{
		mousePt.y -= 100.0;
	}
		
	//NOTE: this is a performance hack - don't update particles that are far from the person
	float vDistToPerson;
	int frameNo = ofGetFrameNum();
	
	for(int i = 0; i < balls.size(); i++){
	
		oilParticle & pI = balls[i];

		//NOTE: this is a performance hack - don't update particles that are far from the person		
		vDistToPerson = fabs(pI.pos.y - myPerson.pos.y);
		if( vDistToPerson > ofGetHeight() && frameNo % 4 != 0 )continue;
	
//		if( pI.type ){
			ofVec2f delta = pos - pI.pos;
			pI.vel += delta.normalized() * 0.35;
//		}else{
//			ofVec2f delta = ofPoint(ofGetWidth()/2, 50 + ofGetHeight()/2) - pI.pos;
//			pI.vel += delta.normalized() * 0.05;
//		}
		
		for(int j = i; j >= 0; j--){
			
			//check 1 -- we shouldn't check against ourself
			if( j == i )continue;
			
			oilParticle & pJ = balls[j];
			
			repulsionDistTmp = MAX(repulsionDist, 1.6 * (pI.radius + pJ.radius));
			repulsionDistSquared = repulsionDistTmp*repulsionDistTmp;
			
			//check 2 -- once the x distance between us and the particle we are checking is greater than our repel distance we can stop checking all other particles
			//we can do this because the particles are sorted by x - so the next particle will be even further away.
			if( fabs( pJ.pos.x - pI.pos.x ) > repulsionDistTmp ){
				break; //this means stop the current loop we are in
			}
			//check 3 -- we can also see if the difference in y is greater than our repulsion dist and if so skip the check
			if( fabs( pJ.pos.y - pI.pos.y ) > repulsionDistTmp ){
				continue;
			}
					
			deltaVec = pJ.pos - pI.pos;
			ppDistSquared  = deltaVec.lengthSquared();
			
			if( pI.type != (int)bRise && pJ.type != (int)bRise ){
				ppDistSquared *= 3.5;
			}
			
			//check 3 -- we check the squared distances as this is much faster than calling square root
			if( ppDistSquared <= repulsionDistSquared ){
				
				//now we sqrt it as we know it will be needed
				ppDist = sqrt(ppDistSquared);
				
				//make the repulsion force get stronger as the particles get closer
				frcPct = 1.0 - (ppDist/repulsionDistTmp); 
				
				if( pI.type && !pJ.type ){
					repulsionAmnt = deltaVec.normalized() * frcPct * repulsionForce;
					
					ofPoint force;
					
					ofVec2f angVec;
					angVec = repulsionAmnt;
					float angle = angVec.angle(pI.vel);
					
					if( angle < 0 ){
						 force.set(repulsionAmnt.y * 0.5 + repulsionAmnt.x* 0.5, -repulsionAmnt.x * 0.5 + repulsionAmnt.y * 0.5);
					}else{
						force.set(-repulsionAmnt.y * 0.5 + repulsionAmnt.x* 0.5, repulsionAmnt.x * 0.5 + repulsionAmnt.y * 0.5);
					}
					
					if( fabs(myPerson.pos.y - pI.pos.y) < 500 && ofGetElapsedTimef() - lastTimeTriggered > ofRandom(0.06, 0.23) ){
//						if(  ofRandom(0, 200) > 100 ){
//							globalSnd.play("OIL_1", ofRandom(0.2, 0.7), 1.0, ofRandom(-0.6, 0.6));
//						}else{
							
							float volPct = ofMap(fabs(myPerson.pos.y - pI.pos.y), 200, ofGetHeight(), 1.0, 0.0, true); 
							globalSnd.play("OIL_WATER_COLLISION", ofRandom(0.2, 0.7) * volPct, ofRandom(0.4, 1.8), ofRandom(-0.6, 0.6));
//						}
						lastTimeTriggered = ofGetElapsedTimef();
					}
					
					//no we apply the force to the two particles being checked - they will have opposite forces so one will be positive and one will be negative. 
					pI.vel -= force * 1.2;
					pJ.vel += repulsionAmnt*1.2;

				}else{
					repulsionAmnt = deltaVec.normalized() * frcPct * repulsionForce;
					
					//no we apply the force to the two particles being checked - they will have opposite forces so one will be positive and one will be negative. 
					pI.vel -= repulsionAmnt;
					pJ.vel += repulsionAmnt;
				}
			}
		}
	
		//POINT REPULSION 
		//for each particle lets look at the distance between it and the mouse
		//if the distance is less than a certain amount lets add a repulsive force that gets stronger with proximity
		ofVec2f distVec = mousePt - pI.pos;
		
		//to know how close we are to the particle we need the straight line distance - or the length of the distVec vector
		float straightDist = distVec.length();
		
		float repelDist = 240.0;
		
		int curType = 0;
		if( myPerson.vel.y < 0 ){
			curType = 1;
		}
		
		if( straightDist < repelDist && pI.type == curType){
			//get the dist as a 0-1 value
			float pct = straightDist/repelDist;
			
//			//so we make a value for the amount of force to apply
//			//we make this inverse to the distance between the mouse and the ball
//			//ie: smaller the distance = greater the force
			float repulsionStrength = (1.0 - pct) * 2.6; 

//			
//			if( straightDist < 16.0 ){
//				globalSnd.play("OIL_WATER_COLLISION_WITH_PERSON", ofRandom(0.1, 0.6), ofRandom(0.4, 1.8), ofRandom(-0.4, 0.4));
//			}
			
			//we now need a vector that describes the direction to move in.
			//because we want to move AWAY from the mouse we can just take the distVec and make it negative
			
			//we want it normalized so we can apply strength to it 
			ofVec2f repulVec = -distVec.normalized();
			
			float repelX = repulVec.x * repulsionStrength * 0.35;
			float repelY = repulVec.y * repulsionStrength;
			
			//finaly add the force to our balls velocity;
			pI.vel.x += repelX;
			pI.vel.y += repelY;
			
		}
		
		if( pI.vel.lengthSquared() > maxVel*maxVel ){
			pI.vel.normalize();
			pI.vel *= maxVel;
		}
		
		if( pI.type == 1 && bRise ){
			pI.extraScale = 1.2;
		}else{
			pI.extraScale = 1.0;
		}

		if( pI.type == 0 && !bRise ){
			pI.extraScale = 1.2;
		}else{
			pI.extraScale = 1.0;
		}
		
		pI.update();
	}
	
}

//--------------------------------------------------------------
void oilWaterNode::draw(){

}
