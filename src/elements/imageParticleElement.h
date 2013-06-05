/*
 *  imageParticleElement.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 29/01/2010.
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
#include "baseElement.h"
#include "colorLoader.h"

class imageParticleElement : public baseElement{


	public: 
	
	imageParticleElement(){
		img = NULL;
	}	
	

	//------------------------------------------------------
	void setup(ofPoint position, ofPoint velocity, ofPoint gravity, float dragIn, float radius){
		pos		= position;
		vel		= velocity;
		gra		= gravity;
		drag	= dragIn;
		rad		= radius;
		home	= pos;
		unique  = ofRandom(0, 1000);
	}
	
	//------------------------------------------------------
	void setImagePtr(ofBaseDraws * imgPtr, bool bFlip = true, bool bHasAlpha = true){		
		img = imgPtr;
		flip = bFlip;
		hasAlpha = bHasAlpha;
	}	
	
	//------------------------------------------------------
	void updateToHome(float attactionForce){
		ofVec3f delta = home - pos;
		delta.normalize();
			
		vel *= drag;
		vel += delta * attactionForce;		
		vel += gra;
		pos += vel;
	}

	//------------------------------------------------------
	void updateToPosition(ofPoint posT, float attactionForce){
		ofVec3f delta = posT - pos;
		delta.normalize();
		
		vel *= drag;
		vel += delta * attactionForce;
		vel += gra;
		pos += vel;
	}
	
	//------------------------------------------------------
	void updateForce(ofPoint force){		
		vel *= drag;
		vel += force;
		vel += gra;
		pos += vel;
	}
	

	//------------------------------------------------------
	void update(){
		vel *= drag;
		vel += gra;
		pos += vel;
	}
	
	//-----------------------------------------------------
	void draw(){
		if( img == NULL )return;
		if( !visible){
			//printf("warning not visible\n");
			return;
		}
		ofPushStyle();
		//ofSetColor(255, 255, 255);
		
			if( hasAlpha ){
				ofEnableAlphaBlending();
			}
			
			ofSetColor(color.r, color.g, color.b, color.a);
			
			ofPushMatrix();
				ofTranslate(pos.x+shift.x, pos.y+shift.y, pos.z+shift.z);
				if( rot.x != 0.0 )ofRotate(rot.x, 1, 0, 0);
				if( rot.y != 0.0 )ofRotate(rot.y, 0, 1, 0);
				if( rot.z != 0.0 )ofRotate(rot.z, 0, 0, 1);
				
				if( flip ){
					img->draw(0, 0, rad * 2.0 * scale.x, rad * 2.0 * -scale.y);
				}else{
					img->draw(0, 0, rad * 2.0 * scale.x, rad * 2.0 * scale.y);				
				}
				
			ofPopMatrix();
		
		ofPopStyle();
		
	}

	float unique;
	bool flip;
	ofBaseDraws * img;
	simpleColor color;		

	ofPoint shift;

	float rad;
	float drag;
	ofVec3f vel;
	ofVec3f gra;
	ofVec3f home;
};
