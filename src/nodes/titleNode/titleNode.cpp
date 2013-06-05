/*
 *  titleNode.cpp
 *  ofxFernCameraDemo
 *
 *  Created by theo on 09/02/2010.
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

#include "titleNode.h"
#include "ofxFutureUtils.h"

//--------------------------------------

void titleNode::setup(ofPoint position){
	setupName("titleNode");
	setPosition(position.x, position.y, position.z);

	fadeInPct	= 1.0;
	pct			= 0.0;
	state		= NODE_INACTIVE;
	
	magTrackingAngle = 0.0;
	magTrackingFade  = 0.0;
	
	timeToShowCheck		= 0.0;
	needToAdvance		= false;	
	bReleasePerson		= false;
			
	mode			= 0;
	completedMode	= 0;
	completedTime   = 0.0;
	
	stepString.push_back("Hold the front cover of the magazine up\nto the camera as shown.");
	stepString.push_back("Rotate the magazine 180 degrees to\nswitch from rising to falling.");
	stepString.push_back("Rotate the magzine back 180 degrees \nto return to rising.");
	stepString.push_back("Tracking callibration is complete. You \nmay now begin the story");

	magTrack.loadImage("assets/title/misc/intro/greyCover.jpg", ofPoint(0.5, 0.5), true);
	float imageH    = magTrack.img.height;
	if( imageH == 0.0 ){
		imageH = 400;
	}
	
	float newH      = (float)ofGetHeight() * 0.45;
	float scaleMag  = newH / imageH;

	float magW = magTrack.img.width  * scaleMag * 0.5;
	float magH = newH;
	
	float magVShift = -120;

	magTrack.setPosition(pos.x + magW*1.07, pos.y+magVShift, 0);
	magTrack.setScale(scaleMag, scaleMag, 1.0);
	magTrack.color.a = 0.0;

	mag.loadImage("assets/title/misc/intro/colorCover.jpg", ofPoint(0.5, 0.5), true);
	mag.setPosition(pos.x-magW*1.07, pos.y+magVShift, 0);
	mag.setScale(scaleMag, scaleMag, 1.0);
	mag.color.a = 255.0;	

	seqMan.loadSequence("assets/title/misc/stepSeq/", ofPoint(0.5, 0.5), true);
	seqMan.setPosition(pos.x, (ofGetHeight()/2)-110, pos.z+1.5);
	seqMan.play();
	
	textBox.loadImage("assets/title/misc/intro/introText.png", ofPoint(0.5, 0.0), true);	
	textBox.setPosition(pos.x, seqMan.pos.y-38, pos.z+1.0);
	
	textBlock.setup(globalType.getFontPtr("DIN_REG_M"), stepString.front(), NULL, ofPoint(0.5, 0.0), true);
	textBlock.setPosition(pos.x,  seqMan.pos.y-78, pos.z+2.0);	
	textBlock.color.a = 0;
	textBlock.setTypeColor(globalColors.getColor("darkFall"));
		
	handImg.loadImage("assets/title/misc/intro/handString.png");
	handImg.setAnchorPercent(0.0, 0.265);
	
	handLeft.setImageReference(handImg, true);
	handLeft.setPosition(pos.x -ofGetWidth()/2, seqMan.pos.y, pos.z+2.0);

	handRight.setImageReference(handImg, true);
	handRight.setPosition(pos.x +ofGetWidth()/2, seqMan.pos.y, pos.z+2.0);
	handRight.setScale(-1, 1, 1);
	
	redLinePosY = pos.y + seqMan.pos.y;
	redLineMinX = pos.x - ofGetWidth()/2 + handImg.width*0.95;
	redLineMaxX = pos.x + ofGetWidth()/2 - handImg.width*0.95;
	
	elements.clear();
	elements.push_back( &mag );
	elements.push_back( &magTrack );
	elements.push_back( &seqMan );
	elements.push_back( &textBox );
	elements.push_back( &textBlock );
	elements.push_back( &handLeft );
	elements.push_back( &handRight );
	elements.push_back( this );

}


//--------------------------------------

//If you want to return this element (So that all drawing happens inside this class's draw() do this
//otherwise return your elements vector so that each element is depth sorted and drawn seperately
vector <baseElement *> titleNode::getElements(){
	return elements;
} 
		
//--------------------------------------
void titleNode::setState(nodeState stateIn){
	if( state != stateIn){
		state = stateIn;
		fireFirstTimeStateReached(state);
	}
}

//--------------------------------------
void titleNode::updateAngle(float angleIn){
	magTrackingAngle = ofLerpDegrees(magTrackingAngle, angleIn, 0.2);
	magTrackingFade += 0.6;
	magTrackingFade = ofClamp(magTrackingFade, 0, 1);
	
	if( mode == completedMode ){
		if(mode == 0 && timeInMode() >= 4.0 ){
			taskCompleted();
			completedMode++;
		}
		if(mode == 1 && fabs( angleIn - 180 ) < 60 &&  timeInMode() >= 5.0 ){
			taskCompleted();
			completedMode++;		
		}
		if(mode == 2 && ( angleIn > 280 || angleIn < 60 ) && timeInMode() >= 5.0 ){
			taskCompleted();
			completedMode++;		
		}
	}
	
}

void titleNode::forceNext(){
	if( ofGetElapsedTimef() > timeToShowCheck ){
		taskCompleted();
		completedMode++;
	}
}

//--------------------------------------
bool titleNode::introAllDone(){
	return bReleasePerson;
}

//--------------------------------------
void titleNode::update(){
//
//	magTrackingFade *= 0.95;
//	
	if( mode >= 3 && ofGetElapsedTimef()-completedTime > 4.0 ){
		bReleasePerson = true;
	}

	if( state == NODE_DEACTIVATING ){		
		if( camPos.y - pos.y > ofGetHeight() || camPos.y - pos.y < -ofGetHeight() ){
			setState(NODE_INACTIVE);
		}

	}

	seqMan.update();

	if( needToAdvance && ofGetElapsedTimef() > timeToShowCheck ){
		nextStep();
	}

	magTrack.rot.z   = -magTrackingAngle;
	
	if( fabs(magTrack.rot.z+180) < 20 ||  fabs(magTrack.rot.z-180) < 20 ){
		magTrack.rot.z = 180;
	}

	if( fabs(magTrack.rot.z) < 20 || fabs(magTrack.rot.z-360) < 20){
		magTrack.rot.z = 0;
	}	
	
	if( mode >= 1 ){
		magTrack.color.a = ofLerp(magTrack.color.a, 200, 0.15);
	}
	
	//mag.color.a = fadeInPct * 255.0;
	
	if( mode == 1 ){
		mag.rot.z = ofLerpDegrees(mag.rot.z, -180, 0.3);
	}	

	if( mode == 2 ){
		mag.rot.z = ofLerpDegrees(mag.rot.z, -359, 0.3);
	}
	
}


//----------------
void titleNode::checkState(float targetX, float targetY, float targetZ){

}

//---------------- 
bool titleNode::shouldPersonWait(){
	return false;
}


//--------------------------------------
float titleNode::timeInMode(){
	return ofGetElapsedTimef() - timeModeStarted;
}

//--------------------------------------
void titleNode::taskCompleted(){
	timeToShowCheck = ofGetElapsedTimef() + 1.5;
	needToAdvance  	= true;
	seqMan.next();
	
	globalSnd.play("INTRO_CHECK_COMPLETED", 0.8);
	
}

//--------------------------------------
void titleNode::nextStep(){
	
	needToAdvance = false;
	seqMan.next();	
	
	if( mode < 3 ){
		mode++;
		
		if( mode < stepString.size() ){
			textBlock.setText(stepString[mode]);
		}
		
		if( mode == 3 ){
			completedTime = ofGetElapsedTimef();
		}else{
			timeModeStarted = ofGetElapsedTimef();
		}
	}
}

//--------------------------------------
void titleNode::draw(){

	ofPushStyle();
		ofSetLineWidth(3.0);
		globalColors.setColor("red");
		ofLine(redLineMinX, redLinePosY, redLineMaxX, redLinePosY); 
	ofPopStyle();
}	