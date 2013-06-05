/*
 *  hairyNode.cpp
 *  ofxFernCameraDemo
 *
 *  Created by theo on 11/02/2010.
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

#include "hairyNode.h"


#include "handTextManager.h"

extern handTextManager leftHand1;
extern handTextManager rightHand1;

//-------------------------------------------------------
hairyNode::hairyNode(){
	maxBoundsY = 500;
	minBoundsY = -500;
}

//-------------------------------------------------------
hairyNode::~hairyNode(){
	particles.clear();
	base.kill();
}

//-----------------------------------
void hairyNode::setup(ofPoint position){
	
	float shiftXHack = 300;
	setupName("attractRepel");
	setPosition(position.x-shiftXHack, position.y, position.z+100);
	world.setup(8, ofPoint());
	
	textCount		= 0;
	
	radius			= 270.0;
	innerSize		= 0.30*radius;
	
	string assetPath = "assets/nodes/node16-beast/both/";

	vector <riseFallString> str = textStackFromXml::loadFromXml("assets/text/riseFallText.xml", "bushNode");
	nodeText.setup( ofPoint(pos.x+300, pos.y+60, pos.z + 10), str, false);
		
	base.setup(0, 0, innerSize-2, 0, 0, 0, true);
	world.addShape(&base);
	
	textBlocks.assign(4, imageElement());
	textBlocksPct.assign(4, 0.0);
	
//assets/nodes/node16-beast/text/txt00.jpg
//assets/nodes/node16-beast/text/txt00b.jpg
//assets/nodes/node16-beast/text/txt01.jpg
//assets/nodes/node16-beast/text/txt02.jpg
//assets/nodes/node16-beast/text/txt03.jpg
//assets/nodes/node16-beast/text/txt03b.jpg
//assets/nodes/node16-beast/text/txt04.jpg
//assets/nodes/node16-beast/text/txt05.jpg
//assets/nodes/node16-beast/text/txt05b.jpg
//	
	
	textBlocks[0].loadImage("assets/nodes/node16-beast/text/txt00b.jpg", ofPoint(0.0, 0.0), true );
	textBlocks[1].loadImage("assets/nodes/node16-beast/text/txt03b.jpg", ofPoint(1.0, 0.0), true);
	textBlocks[2].loadImage("assets/nodes/node16-beast/text/txt04.jpg", ofPoint(0.0, 1.0), true);
	textBlocks[3].loadImage("assets/nodes/node16-beast/text/txt05b.jpg", ofPoint(1.0, 1.0), true);
	
	for(int i = 0; i < textBlocks.size(); i++){
		textBlocks[i].setPosition( pos.x + shiftXHack + ofRandom(-(float)ofGetWidth()/3, (float)ofGetWidth()/3) , pos.y + ofRandom(-300, 300), pos.z -160 ); 
		textBlocks[i].setScale(0.0, 0.0, 1.0);
	}
	
	particles.assign(45, hairyStem());
	for(int i = 0; i < particles.size(); i++){
		float angle = ofMap(i, 0, particles.size(), 0, TWO_PI);
		
		float len = ofRandom(0.7, 1.0);
		
		float endX = cos(angle) * radius*len;
		float endY = sin(angle) * radius*len;

		float startX = cos(angle) * innerSize;
		float startY = sin(angle) * innerSize;
		
		particles[i].init(11, world, startX, startY, endX, endY);
	}
	
	//we do this to get the objects angle set correctly before attaching them together
	world.update(10);
	
	for(int i = 0; i < particles.size(); i++){
		particles[i].addPins(world);
	}
	
	hairys.assign(3, hairyBeast());
	hairys[0].setup( pos + ofPoint(0, -200, 90), 1.4, 0, innerSize,  assetPath+"hairy02.png", assetPath+"hairy01.png");
	hairys[1].setup( pos + ofPoint(850, 100, -150), 1.4, 120, innerSize, assetPath+"hairy02.png", assetPath+"hairy03.png");
	hairys[2].setup( pos + ofPoint(100, 380, -100), 1.1, -80, innerSize, assetPath+"hairy02.png", assetPath+"hairy01.png");
	
	
	for(int i = 0; i < hairys.size(); i++){
		hairys[i].setTentaclePointer(&particles);	
	}	

	killState   = NODE_UNKILLABLE;	
	state		= NODE_INACTIVE;
	lastTime    = ofGetElapsedTimef() + 20;
	
	prevState		= bRise;
	blockCount		= 0;
	minBlockCount	= 0;
}


//-----------------------------------

//If you want to return this element (So that all drawing happens inside this class's draw() do this
//otherwise return your elements vector so that each element is depth sorted and drawn seperately
vector <baseElement *> hairyNode::getElements(){
	vector <baseElement *> elementList;
	elementList.push_back(this);
	for(int i = 0; i < hairys.size(); i++){
		elementList.push_back(&hairys[i]);
	}

	for(int i = 0; i < textBlocks.size(); i++){
		elementList.push_back(&textBlocks[i]);
	}
	
	//elementList.push_back(&nodeText);
	return elementList;
} 

//-----------------------------------
void hairyNode::update(){
	
	//if we are far away from the screen we don't update as often
	if( state == NODE_ACTIVE && fabs(myPerson.pos.y - pos.y) > ofGetHeight()*2 && ofGetFrameNum() % 6 != 0){
		return;
	}
	
	
	//---- begin ghetto code ------ 
	
	for(int i = 0; i < textBlocksPct.size(); i++){
		textBlocks[i].setScale(textBlocksPct[i]*0.85, textBlocksPct[i]*0.85, 1);
	}
	
	for(int i = 0; i < blockCount; i++){
		if( i >= minBlockCount ){
			textBlocksPct[i] += 0.09;
			if( textBlocksPct[i] >= 1.0 ){
				textBlocksPct[i] = 1.0;
			}
		}else if( textBlocksPct[i] > 0.0 ){
			textBlocksPct[i] *= 0.9;
			if( textBlocksPct[i] <= 0.01 ){
				textBlocksPct[i] = 0.0;
			}
		}
	}
	
	if( bRise != prevState && blockCount < textBlocks.size() && fabs(myPerson.pos.y - textBlocks[blockCount].pos.y) < ofGetHeight() ){
		blockCount++;
		if( blockCount == textBlocks.size() ){
			lastTime = ofGetElapsedTimef() + 8.0;
		}	
		if( blockCount > 1 ){
			minBlockCount++;
		}
	}
	
	if( blockCount >= textBlocks.size() && minBlockCount <= textBlocks.size() && ofGetElapsedTimef() > lastTime ){
		minBlockCount++;
	}
	
	//---- end ghetto code ------ 
	
	prevState = bRise;
	
//	if( textCount == 0 && fabs(myPerson.pos.y - pos.y) < 600 ){
//		string text =    "A bush? a beast?\nFor some a feast?";
//		leftHand1.triggerSideText(text, 4.0);
//		textCount++;
//	}
//
//	if( textCount == 1 && fabs(myPerson.pos.y - pos.y) > 600 && !leftHand1.isTextBusy() ){
//		string text =    "A feathered spore?\nor a clever lure?";
//		rightHand1.triggerSideText(text, 4.0);
//		textCount++;
//	}

	if( textCount > 0 ){
		state = NODE_ACTIVE;
	}
	
	//TODO: STOP UPDATING WHEN NOT VISIBLE
	//TODO: CHECK Hash size
		
	ofVec3f person = myPerson.pos;
	
	for(int i = 0; i < hairys.size(); i++){
		hairys[i].setPersonPos(person.x, person.y, person.z);	
		hairys[i].update();
	}	
	

	for(int i = 0; i < particles.size(); i++){
		
		ofxChipmunkRect & c = particles[i].circle.back();
		ofxChipmunkRect & f = particles[i].circle.front();
		
		ofVec3f delta = ofPoint(f.body->p.x, f.body->p.y);
		
		ofVec3f delta2  = ( ofPoint(c.body->p.x, c.body->p.y) + pos) - (person);

		delta.normalize();
		delta *= 40.0;
		
		if( bRise ){
			 if( delta2.length() < 450 ){		
				delta2.normalize();
				delta2 *= -20.0;
				delta = delta2;
			}
		}else{
			 if( delta2.length() < 280 ){
				delta2.normalize();
				delta2 *= 20.0;	
				delta = delta2;
			 }
		}
		
		cpBodyApplyImpulse(c.body, cpv(delta.x*0.15, delta.y*0.15), cpvzero);
		//cpBodySetForce(;
		
	}

	world.update(6, 30.0);
	for(int i = 0; i < particles.size(); i++){
		particles[i].updatePoints();
	}
//	
//		ofVec3f delta = particles[i].pos - person;
//		if( delta.lengthSquared() <= 200 * 200 ){
//			delta.normalize();
//			particles[i].vel += delta * 2.0;
//		}
//	
//		particles[i].vel *= particles[i].drag;
//		particles[i].pos += particles[i].vel;
//		
//		norm = particles[i].pos - pos;
//		norm.normalize();
//		particles[i].pos = pos + radius * norm;
//	}
}


//-----------------------------------
void hairyNode::draw(){
	
//	ofPushStyle();
//		ofSetCircleResolution(64);
//	ofPushMatrix();
//
//		ofPoint beastPos(pos.x, pos.y, pos.z+90);	
//		ofTranslate(beastPos.x, beastPos.y, beastPos.z);	
//
//		ofScale(1.4, 1.4, 1);
//		
//		globalColors.setColor("darkBlue");		
//		for(int i = 0; i < particles.size(); i++){
//			particles[i].draw();
//		}
//
//		ofFill();
//		//globalColors.setColor("darkFall");				
//		//ofCircle(0, 0, innerSize);
//		imgL.draw();
//
//		ofVec2f vec(myPerson.pos.x-beastPos.x, myPerson.pos.y-beastPos.y);
//		vec.normalize();
//		drawEye(25, vec.x, vec.y);
//	
//	ofPopMatrix();
//	
//	ofPushMatrix();
//	
//		beastPos.set(pos.x+700, pos.y+300, pos.z-150);	
//		ofTranslate(beastPos.x, beastPos.y, beastPos.z);
//		ofScale(1.4, 1.4, 1);		
//		
//		ofPushMatrix();		
//		ofRotate(120);
//
//		globalColors.setColor("darkBlue");		
//		for(int i = 0; i < particles.size(); i++){
//			particles[i].draw();
//		}
//
//		ofFill();
//		globalColors.setColor("darkFall");				
//		ofCircle(0,0, innerSize);
//		imgR.draw();
//		
//		ofPopMatrix();
//	
//		vec.set(myPerson.pos.x-beastPos.x, myPerson.pos.y-beastPos.y);
//		vec.normalize();
//		drawEye(25, vec.x, vec.y);
//	
//	
//	ofPopMatrix();
//	
//	ofPushMatrix();
//		
//		beastPos.set(pos.x-50, pos.y+650, pos.z-100);
//		ofTranslate(beastPos.x, beastPos.y, beastPos.z);
//		ofScale(1.0, 1.0, 1);
//		
//		ofPushMatrix();
//		ofRotate(-80);
//		
//		globalColors.setColor("darkBlue");		
//		for(int i = 0; i < particles.size(); i++){
//			particles[i].draw();
//		}
//
//		ofFill();
//		//globalColors.setColor("darkFall");				
//		//ofCircle(0, 0, innerSize);
//		imgL.draw();
//		
//		ofPopMatrix();
//	
//		vec.set(myPerson.pos.x-beastPos.x, myPerson.pos.y-beastPos.y);
//		vec.normalize();
//		drawEye(25, vec.x, vec.y);
//		
//	ofPopMatrix();	
//	
//
////	glBegin(GL_LINE_STRIP);
////		glVertex3fv(myPerson.pos.v);
////		glVertex3fv(pos.v);		
////	glEnd();
//	
//	ofPopStyle();
}

//----------------
void hairyNode::checkState(float targetX, float targetY, float targetZ){
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
bool hairyNode::shouldPersonWait(){
	return false;// (state == NODE_ACTIVATING || state == NODE_ACTIVATING_FALL || state == NODE_ACTIVATING_RISE );
}