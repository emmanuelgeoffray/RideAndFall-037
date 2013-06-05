/*
 *  timeNode.cpp
 *  ofxFernCameraDemo
 *
 *  Created by theo on 18/02/2010.
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

#include "timeNode.h"

//-------------------------------------------------------
timeNode::timeNode(){
	maxBoundsY = 250;
	minBoundsY = -250;
}

//-------------------------------------------------------

void timeNode::setup(ofPoint position){
	elements.clear();
	
	setupName("timeNode");
	setPosition(position.x, position.y, position.z);

	seqRise.loadSequence(globalSeqLoader.getSequencePtr("TIME_SEQ_RISE"), ofPoint(0.5, 0.5), true);
	seqRise.setPosition(pos.x, pos.y, pos.z);

	seqFall.loadSequence(globalSeqLoader.getSequencePtr("TIME_SEQ_FALL"), ofPoint(0.5, 0.5), true);
	seqFall.setPosition(pos.x, pos.y, pos.z);
	
	waitFrame = ofGetFrameNum() + 2;
		
//	textBox.loadImage("assets/nodes/node10-time/both/nodeText.png", ofPoint(0.5, 0.0, 0.0), true);
//	textBox.setPosition(pos.x, pos.y-125, pos.z-4);

	//TODO: DELETE
	testArm[0].setupType(globalType.getFontPtr("DIN_REG_M"), globalColors.getColor("lightRise"), globalColors.getColor("red"), true, ofPoint(0.0, 0.5));
	testArm[0].setupArm(-90+40, 40, 140, 0, globalColors.getColor("darkBlue"));	
	testArm[0].setPosition(pos.x, pos.y - 100, pos.z -5);
	testArm[0].hide();

	testArm[1].setupType(globalType.getFontPtr("DIN_REG_M"), globalColors.getColor("lightRise"), globalColors.getColor("red"), true, ofPoint(1.0, 0.5));
	testArm[1].setupArm(-90-40, 40, 140, 0, globalColors.getColor("darkBlue"));	
	testArm[1].setPosition(pos.x, pos.y - 100, pos.z -5);
	testArm[1].hide();
	
	timeStrings.clear();
	timeStrings.push_back("In the dead of night,");
	timeStrings.push_back("A faint glowing light.");
	timeStrings.push_back("An intriguing smile,");	
	timeStrings.push_back("We log another mile");	
	
	bEndPlayed = false;
	bLeft = true;
	state = NODE_INACTIVE;
}

//------------------------------------------------------- 
vector <baseElement *> timeNode::getElements(){
	vector <baseElement *>  elementList;
	
	elementList.push_back(&testArm[0]);
	elementList.push_back(&testArm[1]);
	elementList.push_back(this);
	
	return elementList;
}

//---------------- 
bool timeNode::shouldPersonWait(){
	return (!seqRise.isDone() && seqRise.bPlay);
}

//-------------------------------------------------------
void timeNode::update(){

	testArm[0].update(0.2);
	testArm[1].update(0.2);

	if( ofGetFrameNum() >= waitFrame ){
		bLoadFullTextures = false;
		bLoadRegTextures  = false;
	}
	
	seqRise.update();
	seqFall.update();
	
	if( seqRise.isDone() && !bEndPlayed ){
		globalSnd.play("TIME_END_TICK", ofRandom(0.8, 0.9), 1.0, 0.0);
		bEndPlayed = true;			
		
		if( timeStrings.size() ){
			
			testArm[timeStrings.size()%2].show(timeStrings.front(), 6);
			
			timeStrings.erase(timeStrings.begin(), timeStrings.begin()+1);
		}
		
		if( seqRise.whichSection == 1 ){
			bLoadFullTextures = true;
			bLoadRegTextures  = false;
			waitFrame = ofGetFrameNum() + 2;
		}
		
		if( seqRise.lastSection() ){
			bLoadFullTextures = false;
			bLoadRegTextures  = true;
				
			waitFrame = ofGetFrameNum() + 2;
		}
	}
	
	if( fabs(myPerson.pos.y - pos.y) < 50 && bLeft ){
		
		if( !seqRise.bPlay ){
			seqRise.play();
			seqFall.play();
			globalSnd.play("TIME_START_TICK", ofRandom(0.8, 0.9), 1.0, 0.0);												
		}else if( seqRise.isDone() ){
			if( seqRise.lastSection() ){
				seqRise.restart();
				seqRise.play();				
				globalSnd.play("TIME_RESET_CLOCK_TO_BEGINING", ofRandom(0.8, 0.9), 1.0, 0.0);
				
				bEndPlayed = false;
				seqFall.restart();				
				seqFall.play();				
			}else{
				seqRise.next();				
				seqFall.next();		
				bEndPlayed = false;
				globalSnd.play("TIME_START_TICK", ofRandom(0.8, 0.9), 1.0, 0.0);									
			}
		}
		
		bLeft = false;
	}
	
	if( bRise ){
		seqRise.color.a = ofLerp(seqRise.color.a, 255, 0.2);
		seqFall.color.a = ofLerp(seqFall.color.a, 0.0, 0.1);
	}else{
		seqRise.color.a = ofLerp(seqRise.color.a, 0.0, 0.1);
		seqFall.color.a = ofLerp(seqFall.color.a, 255.0, 0.2);
	}
	
	if( !bLeft && fabs(myPerson.pos.y - pos.y ) > 100 ){
		bLeft = true;
	}

}

//-------------------------------------------------------
void timeNode::draw(){
	ofPushStyle();
		seqRise.draw();
		seqFall.draw();
	ofPopStyle();
}		
