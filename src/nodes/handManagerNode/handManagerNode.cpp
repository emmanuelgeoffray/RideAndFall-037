/*
 *  handManagerNode.cpp
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

#include "handManagerNode.h"

handTextManager leftHand1;
handTextManager rightHand1;

//-------------------------------------------------------

void handManagerNode::setup(ofPoint position){
			
	setupName("riseHand");
	setPosition(position.x, position.y, position.z);

	seqPin.loadSequence(globalSeqLoader.getSequencePtr("PIN_SEQ"), ofPoint(1.0, 0.5), false);
	seqPin.setPosition(position.x, position.y, position.z);
	seqPin.hide();
		
	seqFan.loadSequence(globalSeqLoader.getSequencePtr("FAN_SEQ"), ofPoint(0.0, 0.0), false);
	seqFan.setPosition(pos.x, pos.y, pos.z);
	seqFan.hide();
	
	vector <riseFallString> str = textStackFromXml::loadFromXml("assets/text/riseFallText.xml", "ruffle");

	string tmpStr = "Two groups, as one.\nfight to be two.\n\nTheir differences subtle\nas two shades of blue,\n\nWe force them together\nand drive them apart.\n\nStuck in their ways\nthey struggle to part.";

	string handPath = "assets/nodes/node14-handText/handTextStills/handTextL.png";
	leftHand1.setup(ofPoint(-20, ofGetHeight()/2), globalType.getFontPtr("DIN_REG_M"), globalColors.getColor("darkFall"), handPath, ofPoint(0, 1), ofPoint(0.747, 0.281), true);

	handPath = "assets/nodes/node14-handText/handTextStills/handTextR.png";
	rightHand1.setup(ofPoint(ofGetWidth()+20, ofGetHeight()/2), globalType.getFontPtr("DIN_REG_M"), globalColors.getColor("darkFall"), handPath, ofPoint(1, 1), ofPoint(0.245, 0.281), false);

	lastTimeBurst = 0.0;
	
	state = NODE_INACTIVE;
	
	elements.push_back(this);
}

//-------------------------------------------------------
void handManagerNode::update(){
	
	//TODO: CRASH - CHECK THIS 
	leftHand1.update();
	rightHand1.update();
	
	seqPin.update();
	if( seqPin.isDone()){
		
		if( seqPin.currentSection.name == "poke" ){
			if( myPerson.vel.y > 0.0 ){
				myPerson.burstBalloon();
				lastTimeBurst = ofGetElapsedTimef();
				seqPin.next();
			}else{
				seqPin.setSection("backOutPoke");
			}
		}else if( seqPin.currentSection.name == "open"){
			if( myPerson.vel.y > 0.0 ){
				seqPin.next();
			}else{
				seqPin.setSection("backOutOpen");			
			}
		}else{
			seqPin.hide();
			seqPin.restartAndWait();			
		}
		
	}
	
	seqFan.update();
	if( seqFan.isDone()){
		if( seqFan.currentSection.name == "close" ){
			seqFan.hide();
			seqFan.restartAndWait();			
		}else{
			seqFan.next();		
		}
		if( seqFan.currentSection.name == "blow" ){
			if( myPerson.vel.y < 0.0 ){
				myPerson.blowBalloon();
				lastTimeBurst = ofGetElapsedTimef();
			}else{
				seqPin.setSection("backOut");			
			}
		}	
	
	}	
	
}

//-------------------------------------------------------
bool handManagerNode::doPinPrick(){
	if( bRise && !myPerson.isBursting() && !myPerson.isBlowing() ){
		seqPin.restart();
		seqPin.play();
		seqPin.show();
		globalSnd.play("BALLOON_POP", 0.8, 1.0, ofMap(myPerson.pos.x, -ofGetWidth()/2, ofGetWidth()/2, -1, 1, true) );
		return true;
	}
	return false;
}

//-------------------------------------------------------
bool handManagerNode::doFanBlow(){
	if( !bRise && !myPerson.isBursting() && !myPerson.isBlowing() ){
		seqFan.restart();
		seqFan.play();
		seqFan.show();
		globalSnd.play("BALLOON_FAN", 0.8, 1.0, ofMap(myPerson.pos.x, -ofGetWidth()/2, ofGetWidth()/2, -1, 1, true) );
		return true;
	}
	return false;
}

//-------------------------------------------------------
void handManagerNode::updateCamToScreenCoords(){
	
	GLdouble model_view[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, model_view);

	GLdouble projection[16];
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
		
	GLdouble X, Y, Z;		
	
	if( seqPin.currentSection.name == "open" || seqPin.currentSection.name == "poke"  ){
		storePersonPosPin = myPerson.pos;
	}
	
	if( seqFan.currentSection.name == "open"  ){
		storePersonPosFan = myPerson.pos;
	}

	if( seqPin.bPlay ){
		gluProject(storePersonPosPin.x - 50, storePersonPosPin.y + 45, storePersonPosPin.z, model_view, projection, viewport, &X, &Y, &Z);
		myPersonScreenPosPin.set(X, ofGetHeight()-Y, Z);
		seqPin.setPosition(myPersonScreenPosPin.x, myPersonScreenPosPin.y, 0);		
	}

	if( seqFan.bPlay ){
		gluProject(storePersonPosFan.x-50, storePersonPosFan.y - 180, storePersonPosFan.z, model_view, projection, viewport, &X, &Y, &Z);
		myPersonScreenPosFan.set(X, ofGetHeight()-Y, Z);
		seqFan.setPosition(myPersonScreenPosFan.x, myPersonScreenPosFan.y, 0);		
	}
		
}

//-------------------------------------------------------
void handManagerNode::draw(){
	ofPushStyle();
		
		leftHand1.draw();
		rightHand1.draw();
						
		seqPin.draw();		
		seqFan.draw();		

	ofPopStyle();
}		
