/*
 *  oilWaterNode.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 03/02/2010.
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

#pragma once
#include "ofMain.h"
#include "baseNode.h"
#include "stackText.h"
#include "textStackFromXml.h"

class oilParticle: public baseElement{
	public:
	
	oilParticle(){
		texPtr = NULL;
		friction = 1.0;
		radius = ofRandom(8, 15);
		type = ofRandom(0,1.999);
	}
	
	void setTexturePtr( ofTexture * tex ){
		texPtr = tex;
	}
		
	void setFriction(float frictionAmnt){
		friction = frictionAmnt;
	}

	void setPosition(ofPoint newPos){
		pos = newPos;
	}
		
	void setPosition(float X, float Y, float Z){
		pos.set(X,Y,Z);
	}

	void setVelocity(float x, float y){
		setVelocity(ofVec2f(x, y));
	}

	void setVelocity(ofVec2f velocity){
		vel = velocity;
	}

	void update(){
		vel *= friction;	
		pos += vel;
	}

	void addForce(float x, float y){
		addForce(ofVec2f(x, y));
	}

	void addForce(ofVec2f force){
		vel += force;
	}
		
	void draw(){
		if( texPtr == NULL ){
			return;
		}
		
		ofEnableAlphaBlending();

		ofPoint t1 = texPtr->getCoordFromPercent(0.03, 0.03);
		ofPoint t2 = texPtr->getCoordFromPercent(0.97, 0.97);

		float brightness = 0.6 + 0.6 * ofNoise( radius * 100.0 , 0.5 );
		float rad = 1.2 * radius * extraScale;
		
		texPtr->bind();			
		glBegin(GL_QUADS);

			glColor4f(brightness, brightness, brightness, 1.0);
			
			glTexCoord2f(t1.x, t1.y);
			glVertex3f(pos.x - rad, pos.y - rad, pos.z);

			glTexCoord2f(t2.x, t1.y);
			glVertex3f(pos.x + rad, pos.y - rad, pos.z);

			glTexCoord2f(t2.x, t2.y);
			glVertex3f(pos.x + rad, pos.y + rad, pos.z);

			glTexCoord2f(t1.x, t2.y);	
			glVertex3f(pos.x - rad, pos.y + rad, pos.z);
			
		
		glEnd();
		texPtr->unbind();
	
	}
	
	ofVec2f vel;
	float friction;
	float radius;
	int type;
	float extraScale;
	ofTexture * texPtr;
};

class oilWaterNode : public baseNode{


	stackText nodeText;

	float repulsionDist;
	float repulsionForce;
	vector <oilParticle> balls;
	float nodeRadius;
	float nPct;
	ofImage tex1, tex2;	
	
	public:

	oilWaterNode();

	//--------------------------------------------------------------
	void setup(ofPoint position, float radius);

	//If you want to return this element (So that all drawing happens inside this class's draw() do this
	//otherwise return your elements vector so that each element is depth sorted and drawn seperately
	vector <baseElement *> getElements();
	bool shouldPersonWait();
	void checkState(float targetX, float targetY, float targetZ);
	void update();
	void updateInactive();
	void updateActive();
	void draw();
	
	vector <string> oilWaterText;
	int currentText;
	bool bPrevFrameRiseFall;

	bool textDone;

};