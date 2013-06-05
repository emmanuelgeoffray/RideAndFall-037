/*
 *  handTextManager.cpp
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

#include "handTextManager.h"

//-------------------------------------------------------

void handTextManager::setup(ofPoint position, ofTrueTypeFont * ttfPtr, simpleColor typeColor, string image, ofPoint anchor, ofPoint textBoxAnchor, bool bLeftReveal ){
			
	pos.set(position.x, position.y, position.z);

	string tmpStr = "Two groups, as one.\nfight to be two.\n\nTheir differences subtle\nas two shades of blue,\n\nWe force them together\nand drive them apart.\n\nStuck in their ways\nthey struggle to part.";

	bLeft = bLeftReveal;

	if( bLeft ){
		hidePosX = -500;
	}else{
		hidePosX = ofGetWidth()+500;
	}
	
	shiftPosX = hidePosX;

	leftHand.setColor(typeColor);
	leftHand.setTypeFace(ttfPtr);
			
	leftHand.setText(tmpStr);
	leftHand.setPosition(pos.x, pos.y, 0);

	leftHand.loadImage(image, anchor, textBoxAnchor, !bLeft);
	leftHand.setScale(0.7, 0.7, 1.0);

//	leftHand.loadImage(image, ofPoint(0, 1), ofPoint(0.747, 0.719) );
//	leftHand.setScale(0.7, 0.7, 1.0);
//	
	leftState = TEXT_TRIGGER_HIDDEN;	
	leftTriggerTime = 0.0;

}

//-------------------------------------------------------		
bool handTextManager::isTextBusy(){
	return (leftState != TEXT_TRIGGER_HIDDEN);
}

//-------------------------------------------------------		
void handTextManager::triggerSideText(string text, float duration){
	leftHand.setText(text);
	
	leftState = TEXT_TRIGGER_TIME;	
	leftTriggerTime = ofGetElapsedTimef() + duration;
	
	if( bLeft ){
		globalSnd.play("TEXT_HAND_SHOW_LEFT", ofRandom(0.8, 0.9), 1.0, -0.9);
	}else{
		globalSnd.play("TEXT_HAND_SHOW_RIGHT", ofRandom(0.8, 0.9), 1.0, 0.9);
	}	
}

//-------------------------------------------------------
void handTextManager::triggerSideText(string text, float minY, float maxY){
	leftHand.setText(text);
	
	leftState = TEXT_TRIGGER_POS;	
	minTriggerY = minY;
	maxTriggerY = maxY;
	
	if( bLeft ){
		globalSnd.play("TEXT_HAND_SHOW_LEFT", ofRandom(0.8, 0.9), 1.0, -0.9);
	}else{
		globalSnd.play("TEXT_HAND_SHOW_RIGHT", ofRandom(0.8, 0.9), 1.0, 0.9);
	}
}		

//-------------------------------------------------------
void handTextManager::update(){

	if( bLeft ){
		hidePosX = -600;
	}else{
		hidePosX = ofGetWidth()+600;
	}
	

	if( leftState != TEXT_TRIGGER_HIDDEN ){
	
		//TODO: ALLOW FOR MORE RANDOM PLACEMENT
		float yShift = myPerson.pos.y - camPos.y;
		
		textTriggerState preState = leftState;
		
		if( leftState == TEXT_TRIGGER_POS ){
			if( myPerson.pos.y > maxTriggerY || myPerson.pos.y < minTriggerY ){
				leftState = TEXT_TRIGGER_HIDDING;
			}
		}

		if( leftState == TEXT_TRIGGER_TIME ){
			if( ofGetElapsedTimef() > leftTriggerTime ){
				leftState = TEXT_TRIGGER_HIDDING;
			}
		}
			
		if( leftState == TEXT_TRIGGER_HIDDING ){
			shiftPosX = ofLerp(shiftPosX, hidePosX, 0.16);
			
			if( fabs(shiftPosX - hidePosX) < 2.0 ){
				leftState = TEXT_TRIGGER_HIDDEN;
			}
		}else{
			shiftPosX = ofLerp(shiftPosX, pos.x, 0.15);
		}
		
		if( preState != TEXT_TRIGGER_HIDDING && leftState == TEXT_TRIGGER_HIDDING ){
			if( bLeft ){
				globalSnd.play("TEXT_HAND_HIDE_LEFT", ofRandom(0.8, 0.9), 1.0, -0.9);
			}else{
				globalSnd.play("TEXT_HAND_HIDE_RIGHT", ofRandom(0.8, 0.9), 1.0, 0.9);
			}
		}

		leftHand.setPosition(shiftPosX, 0.5 * yShift + 20 * ofSignedNoise(ofGetElapsedTimef() * 0.4, 0) + pos.y, 0);	
	}
	
}


//-------------------------------------------------------
void handTextManager::updateCamToScreenCoords(){
	
//	GLdouble model_view[16];
//	glGetDoublev(GL_MODELVIEW_MATRIX, model_view);
//
//	GLdouble projection[16];
//	glGetDoublev(GL_PROJECTION_MATRIX, projection);
//
//	GLint viewport[4];
//	glGetIntegerv(GL_VIEWPORT, viewport);
//		
//	GLdouble X, Y, Z;		
//	
//	if( seq.currentSection.name != "close"  ){
//		storePersonPos = myPerson.pos;
//	}
//	
//	if( seq.bPlay ){
//		gluProject(storePersonPos.x - 50, storePersonPos.y + 45, storePersonPos.z, model_view, projection, viewport, &X, &Y, &Z);
//		myPersonScreenPos.set(X, ofGetHeight()-Y, Z);
//		seq.setPosition(myPersonScreenPos.x, myPersonScreenPos.y, 0);		
//	}
//	
//	puller.checkScreenCoords();
	
}

//-------------------------------------------------------
void handTextManager::draw(){
	ofPushStyle();
		
		if( leftState != TEXT_TRIGGER_HIDDEN ){
			leftHand.draw();
		}
		
	ofPopStyle();
}		
