/*
 *  chargeNode.cpp
 *  ofxFernCameraDemo
 *
 *  Created by theo on 15/02/2010.
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

#include "chargeNode.h"


//-----------------------------------
chargeNode::chargeNode(){
	maxBoundsY = 500;
	minBoundsY = -500;
}

//-----------------------------------
void chargeNode::setup(ofPoint position){
	
	elements.clear();
	
	setupName("chargeNode");
	setPosition(position.x, position.y, position.z);
	
	//TODO: check all vectors in setup for clear.
	elements.clear();
	
	vector <riseFallString> str = textStackFromXml::loadFromXml("assets/text/riseFallText.xml", "chargeNode");
	
	if( bRise )
		nodeText.setup( ofPoint(pos.x, pos.y-100, pos.z - 30), str, false, true);
	else{
		nodeText.setup( ofPoint(pos.x, pos.y-100, pos.z - 30), str, true, true);	
	}
	
	elements.push_back(&nodeText);
	
//	chargeSeqR.loadSequence(globalSeqLoader.getSequencePtr("CHARGE_POS"), ofPoint(0.5, 0.5), true);
//	chargeSeqR.setPosition(pos.x, pos.y, pos.z);
//	chargeSeqR.color = globalColors.getColor("darkBlue");
//	chargeSeqR.color.a = 0.0;
//	chargeSeqR.setFrameRate(20);
//
//	chargeSeqF.loadSequence(globalSeqLoader.getSequencePtr("CHARGE_NEG"), ofPoint(0.5, 0.5), true);
//	chargeSeqF.setPosition(pos.x, pos.y, pos.z);
//	chargeSeqF.color = globalColors.getColor("lightRise");
//	chargeSeqF.color.a = 0.0;
//	chargeSeqF.color.modulateRGB(2.0);
//	chargeSeqF.setFrameRate(20);
//
//	elements.push_back(&chargeSeqR);
//	elements.push_back(&chargeSeqF);

	
//	guard.loadImage("assets/nodes/node17-charge/both/chargeSide.png", ofPoint(0.8, 0.5));
//	guard.setPosition(pos.x-ofGetWidth()/1.5, pos.y, pos.z+20);
//	elements.push_back(&guard);
	
	float totalH = 1200;
	
	int numRods	   = 200;
	float spacing  = totalH/(float)numRods;
	float hTotalH  = totalH * 0.5;
	
	rodsLeft.assign(numRods, chargeAttractRod());
	for(int i= 0; i < rodsLeft.size(); i++){
		rodsLeft[i].setAngle(0);
		rodsLeft[i].setDrag(ofRandom(0.9145, 0.927) );
		rodsLeft[i].setPosition(pos.x-ofGetWidth()/1.5, pos.y-hTotalH + i * spacing + ofRandom(-3, 3), pos.z);
		rodsLeft[i].setFlip(false);	
		
		rodsLeft[i].color     = globalColors.getColor("midBlue");
		
		bool bThick		= false;
		float thickness = 1.0;
		float len = ofRandom(0.7, 1.0);
		float bright = 1.0;
//		
//		if( i % 3 == 0 ){
//			bThick = true;
//			thickness = 9.0;
//			len *= 0.33;
//			bright = ofRandom(0.3, 0.5);
//			rodsLeft[i].pos.z += 5.0;
//		}

		float val  = ofRandom(0.7, 1.0)*bright;
		
		rodsLeft[i].color.modulateRGB(val);
		rodsLeft[i].thickness = thickness;
		rodsLeft[i].setLength(ofGetWidth() * 0.54 * (0.2 + len ) );

		rodsLeft[i].colorBall = globalColors.getColor("darkFall");
		
		elements.push_back(&rodsLeft[i]);	
	}

	rodsRight.assign(numRods, chargeAttractRod());
	for(int i= 0; i < rodsRight.size(); i++){
		rodsRight[i].setAngle(0);
		rodsRight[i].setDrag(ofRandom(0.9145, 0.927) );
		rodsRight[i].setPosition(pos.x+ofGetWidth()/1.5, pos.y-hTotalH + i * spacing + ofRandom(-3, 3), pos.z);
		rodsRight[i].setFlip(true);
		
		rodsRight[i].color     = globalColors.getColor("midBlue");
		
		bool bThick		= false;
		float thickness = 1.0;
		float len = ofRandom(0.7, 1.0);
		float bright = 1.0;
		
//		if( i % 3 == 0 ){
//			bThick = true;
//			thickness = 9.0;
//			len *= 0.33;
//			bright = ofRandom(0.3, 0.5);
//			rodsRight[i].pos.z += 5.0;
//		}

		float val  = ofRandom(0.7, 1.0) * bright;
		
		rodsRight[i].color.modulateRGB(val);
		rodsRight[i].thickness = thickness;
		rodsRight[i].setLength(ofGetWidth() * 0.54 * (0.2 + len ) );

				
		rodsRight[i].colorBall = globalColors.getColor("darkFall");
				
		elements.push_back(&rodsRight[i]);			
	}
		
	state     = NODE_INACTIVE;
	killState = NODE_UNKILLABLE;
}

//-----------------------------------
//If you want to return this element (So that all drawing happens inside this class's draw() do this
//otherwise return your elements vector so that each element is depth sorted and drawn seperately
vector <baseElement *> chargeNode::getElements(){
	return elements;
} 

//-----------------------------------
void chargeNode::update(){

	if( nodeText.getState() == TEXT_STACK_INACTIVE && fabs(myPerson.pos.y - pos.y) < 100 ){
		nodeText.activate();
	}
	
	if( fabs(myPerson.pos.y - pos.y)  > ofGetHeight()*4 ){
		return;
	}
	
//	float alpha = ofMap(fabs(myPerson.pos.y-pos.y),  200, 700, 255.0, 0.0, true);
//	chargeSeqR.color.a = alpha;
//	chargeSeqF.color.a = alpha;
//		
//	if( bRise ){
//		chargeSeqF.color.a *= 0.0;
//	}else{
//		chargeSeqR.color.a *= 0.0;	
//	}	
//	
//	if( chargeSeqF.color.a <= 0.0 ){
//		chargeSeqF.stop();
//		chargeSeqF.hide();
//	}else{
//		chargeSeqF.show();
//		chargeSeqF.play();
//		ofPoint p = myPerson.pos + ofPoint(0, -90, -5);
//		chargeSeqF.setPosition(p.x, p.y, p.z);		
//	}
//	
//	if( chargeSeqR.color.a <= 0.0 ){
//		chargeSeqR.stop();
//		chargeSeqR.hide();
//	}else{
//		chargeSeqR.show();
//		chargeSeqR.play();
//		ofPoint p = myPerson.pos + ofPoint(0, -90, -5);
//		chargeSeqR.setPosition(p.x, p.y, p.z);		
//	}
//	
//	chargeSeqR.update();
//	chargeSeqF.update();

	
	nodeText.update(0.04);
	
	float forceDist = 400;

	for(int i= 0; i < rodsLeft.size(); i++){
		
		float dx = myPerson.pos.x - rodsLeft[i].pos.x;
		float dy = myPerson.pos.y - rodsLeft[i].pos.y;
		
		if( fabs(dy) < forceDist ){
			float angle = atan2(dy, dx);
			
			if( bRise) rodsLeft[i].updateToTarget( -ofMap( fabs(dy), 0, 400, angle*2.5, angle*0.125) );
			else rodsLeft[i].updateToTarget( ofMap( fabs(dy), 0, forceDist, angle, angle*0.5) );
			
		}else{
			rodsLeft[i].updateToHome();
		}
		
		float force = ofMap(fabs(dy), 250, 0, 0.0, 1.0, true);
		if( bRise ){
			force *= -1.0;
		}

		rodsLeft[i].update();
		rodsLeft[i].setForce(force*0.002);
		rodsLeft[i].updateForce();
		

//		if( !bRise ){
			rodsLeft[i].scale = ofLerp(rodsLeft[i].scale, ofMap( fabs(dy), 70, 600, 1.0, 0.4, true), 0.1);
//		}else{
//			rodsLeft[i].scale = ofLerp(rodsLeft[i].scale, ofMap( fabs(dy), 10, 300, 0.7, 1.15, true), 0.1);
//		}
	}

	for(int i= 0; i < rodsRight.size(); i++){
		
		float dx = rodsRight[i].pos.x-myPerson.pos.x;
		float dy = rodsRight[i].pos.y-myPerson.pos.y;
		
		if( fabs(dy) < forceDist ){
			float angle = atan2(dy, dx);
			
			if( bRise) rodsRight[i].updateToTarget( -ofMap( fabs(dy), 0, forceDist, angle*2.5, angle*0.125) );
			else rodsRight[i].updateToTarget( ofMap( fabs(dy), 0, forceDist, angle, angle*0.5) );
			
		}else{
			rodsRight[i].updateToHome();
		}

		float force = ofMap(fabs(dy), 250, 0, 0.0, 1.0, true);
		if( bRise ){
			force *= -1.0;
		}

		rodsRight[i].update();
		rodsRight[i].setForce(force*0.002);
		rodsRight[i].updateForce();

//		if( !bRise ){
			rodsRight[i].scale = ofLerp(rodsRight[i].scale, ofMap( fabs(dy), 70, 600, 1.0, 0.4, true), 0.1);
//		}else{
//			rodsRight[i].scale = ofLerp(rodsRight[i].scale, ofMap( fabs(dy), 10, 300, 0.7, 1.15, true), 0.1);
//		}
	}
}

//-----------------------------------
void chargeNode::draw(){

}

//----------------
void chargeNode::checkState(float targetX, float targetY, float targetZ){
	ofVec3f dist(targetX-pos.x, targetY-pos.y, 0);
	if( dist.length() < 300 ){
		if( state == NODE_INACTIVE && bRise ){
			setState(NODE_ACTIVATING_RISE);
		}
		if( state == NODE_RISE && !bRise ){
			setState(NODE_ACTIVATING_FALL);
		}	
		if( state == NODE_FALL && bRise ){
			setState(NODE_ACTIVATING_RISE);
		}				
	}
}

//---------------- 
bool chargeNode::shouldPersonWait(){
	return false;// (state == NODE_ACTIVATING || state == NODE_ACTIVATING_FALL || state == NODE_ACTIVATING_RISE );
}