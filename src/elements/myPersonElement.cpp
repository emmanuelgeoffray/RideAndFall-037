/*
 *  myPersonElement.cpp
 *  ofxFernCameraDemo
 *
 *  Created by theo on 07/02/2010.
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

#include "myPersonElement.h"

extern bool bRise;

//------------------------------------------------------
void myPersonElement::setup(ofPoint position, ofPoint velocity, ofPoint gravity, float dragIn, float radius){
	pos		= position;
	vel		= velocity;
	gra		= gravity;
	drag	= dragIn;
	rad		= radius;
	home	= pos;
	
	setScale(0.75, 0.75, 1.0);
	
	which   = 0;
	deviceScale = 1.0;
	targetScale = 1.0;
	burstScale = 1.0;
	burstPercent = 0.0;
	
	minDeviceScale = 0.2;
	maxDeviceScale = 1.7;
	
	hand[0].loadImage("assets/global/misc/person/riseHand.png");
	hand[1].loadImage("assets/global/misc/person/fallHand.png");
	
	hand[0].setAnchorPercent(0.5, 0.0);
	hand[1].setAnchorPercent(0.5, 0.0);
	
	device[0].loadImage("assets/global/misc/person/balloon.png");
	device[1].loadImage("assets/global/misc/person/umbrella.png");
	
	device[0].setAnchorPercent(0.5, 1.0);
	device[1].setAnchorPercent(0.5, 1.0);	
	
	hasAlpha = true;
	flip     = true;
}

void myPersonElement::increaseScale(float amnt, float duration){
	scaleTillTime = ofGetElapsedTimef() + duration;
	
	targetScale += amnt;
	if( targetScale > maxDeviceScale ){
		targetScale = maxDeviceScale;
	}
}

void myPersonElement::decreaseScale(float amnt, float duration){
	scaleTillTime = ofGetElapsedTimef() + duration;
	
	targetScale -= amnt;
	if( targetScale < minDeviceScale ){
		targetScale = minDeviceScale;
	}
}

//------------------------------------------------------
void myPersonElement::update(ofPoint gravity, float dragAmnt, float updateSpeed){
	
	if( scaleTillTime < ofGetElapsedTimef() ){
		targetScale = 1.0;
	}
	
	deviceScale = ofLerp(deviceScale, targetScale, 0.1);
	

	if( blowPercent > 0.0 ){
		
		blowPercent *= 0.945;
		if( blowPercent <= 0.01 ){
			blowPercent = 0.0;
		}
		
	}
		
	if( burstPercent > 0.0 ){
		
		burstPercent *= 0.97;
		if( burstPercent <= 0.01 ){
			burstPercent = 0.0;
		}
		
		burstScale = ofMap( burstPercent, 0.4, 0.0, 0.0, 1.0, true );
	}
	
	if( isBursting() && burstScale < 0.7 ){
		if( gravity.y > 0 ){
			gravity.y *= -1.5;
		}
	}

	if( isBlowing() ){
		if( gravity.y < 0 ){
			gravity.y *= -1.5;
		}
	}


	float scaleSpeed = 1.0;
	if( bRise ){
		if( deviceScale >= 1.0 ){
			scaleSpeed = ofMap(deviceScale, 1.0, maxDeviceScale, 1.0, 1.8, true); 
		}else{
			scaleSpeed = ofMap(deviceScale, 1.0, minDeviceScale, 1.0, 0.5, true); 
		}
	}else{
		if( deviceScale >= 1.0 ){
			scaleSpeed = ofMap(deviceScale, 1.0, maxDeviceScale, 1.0, 0.5, true); 
		}else{
			scaleSpeed = ofMap(deviceScale, 1.0, minDeviceScale, 1.0, 1.8, true); 
		}
	}
	
	vel *= drag * dragAmnt;
	vel += gravity * updateSpeed * scaleSpeed;
	
	float screenEdgeX = 0.95 * (float)( ofGetWidth()/2 );
	
	if( ( pos.x + vel.x*30 < -screenEdgeX ) ){
		vel.x += 2.0;
	}
	if( ( pos.x + vel.x*30 > screenEdgeX )  ){
		vel.x -= 2.0;
	}
	
	if( ( pos.x + vel.x > -screenEdgeX ) && ( pos.x + vel.x < screenEdgeX) ){
		pos.x += vel.x;		
	}
	
	pos.y += vel.y * scaleSpeed;	
	pos.z += vel.z;	

}

//------------------------------------------------------
bool myPersonElement::isBursting(){
	return ( burstPercent > 0.0 );
}

//------------------------------------------------------
bool myPersonElement::isBlowing(){
	return ( blowPercent > 0.0 );
}

//------------------------------------------------------
void myPersonElement::burstBalloon(){
	if( !isBursting() && vel.y > 0 ){
		burstPercent = 1.0;
		vel.y *= -2.0;
		if( vel.y > -10.0 ){
			vel.y = -10.0;
		}
	}
}

//------------------------------------------------------
void myPersonElement::blowBalloon(){
	if( !isBlowing() && vel.y < 0 ){
		blowPercent = 1.0;
		vel.y *= -2.0;
		if( vel.y < 10.0 ){
			vel.y = 10.0;
		}
	}
}

//-----------------------------------------------------
void myPersonElement::drawAtScreenPos(float x, float y, float z){
	if( !visible ){
		//printf("warning not visible\n");
		return;
	}
	
	if( isBursting() ){
		which = 0;
	}
	
	ofPushStyle();
	//ofSetColor(255, 255, 255);
	
		if( hasAlpha ){
			ofEnableAlphaBlending();
		}
		
		ofSetColor(color.r, color.g, color.b, color.a);
		
		ofPushMatrix();
			ofTranslate(x+shift.x, y+shift.y, z+shift.z);
			if( rot.x != 0.0 )ofRotate(-rot.x, 1, 0, 0);
			if( rot.y != 0.0 )ofRotate(-rot.y, 0, 1, 0);
			if( rot.z != 0.0 )ofRotate(-rot.z, 0, 0, 1);
			

			hand[which].draw(0, 0, (float)hand[which].width * scale.x, (float)hand[which].height * scale.y);				
			device[which].draw(0, 5, (float)device[which].width * scale.x * deviceScale * burstScale, (float)device[which].height * scale.y * deviceScale * burstScale);				
			
		ofPopMatrix();
	
	ofPopStyle();
	
}


//-----------------------------------------------------
void myPersonElement::draw(){
	if( !visible ){
		//printf("warning not visible\n");
		return;
	}
	
	if( isBursting() ){
		which = 0;
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
				hand[which].draw(0, 0, (float)hand[which].width * scale.x, (float)hand[which].height * -scale.y);
				device[which].draw(0, -5, (float)device[which].width * scale.x * deviceScale * burstScale, (float)device[which].height * -scale.y  * deviceScale * burstScale);
			}else{
				hand[which].draw(0, 0, (float)hand[which].width * scale.x, (float)hand[which].height * scale.y);				
				device[which].draw(0, -5, (float)device[which].width * scale.x * deviceScale * burstScale, (float)device[which].height * scale.y * deviceScale * burstScale);				
			}
			
		ofPopMatrix();
	
	ofPopStyle();
	
}

