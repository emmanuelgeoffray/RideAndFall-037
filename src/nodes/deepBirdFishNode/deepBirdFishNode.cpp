/*
 *  deepBirdFishNode.cpp
 *  ofxFernCameraDemo
 *
 *  Created by theo on 12/02/2010.
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

#include "deepBirdFishNode.h"

//-------------------------------------------------------
deepBirdFishNode::deepBirdFishNode(){
	maxBoundsY = 300;
	minBoundsY = -300;
}

//-----------------------------------
void deepBirdFishNode::setup(ofPoint position){
	
	setupName("deepBirdFishNode");
	setPosition(position.x, position.y, position.z);
	
	string texPathR = "assets/nodes/node02-birdFish/both/colorTexHigh.jpg";
	string texPathF = "assets/nodes/node02-birdFish/both/colorTex4.jpg";
	
	subNest1.setup( ofPoint(pos.x-ofGetWidth()/1.9, pos.y - 150, pos.z), 10, 340, texPathR, texPathF);
	subNest2.setup( ofPoint(pos.x+ofGetWidth()/1.9, pos.y + 150, pos.z), 10, 240, texPathR, texPathF );

	birdPathReg = "assets/nodes/node02-birdFish/both/fishBird/birdHigh-reg.png";
	birdPathFull = "assets/nodes/node02-birdFish/both/fishBird/birdHigh-full.png";
	fishPathReg = "assets/nodes/node02-birdFish/both/fishBird/fishLow-reg.png";
	fishPathFull = "assets/nodes/node02-birdFish/both/fishBird/fishLow-full.png";	

	fbManager.setup(subNest1.pos, subNest2.pos, 12, birdPathReg, fishPathReg);
			
	killState   = NODE_UNKILLABLE;
	state		= NODE_INACTIVE;
}


//-----------------------------------
//If you want to return this element (So that all drawing happens inside this class's draw() do this
//otherwise return your elements vector so that each element is depth sorted and drawn seperately
vector <baseElement *> deepBirdFishNode::getElements(){

	vector <baseElement *> elementList = fbManager.getElements();

	elementList.push_back(&subNest1);
	elementList.push_back(&subNest2);
	
	return elementList;
} 

//----------------
void deepBirdFishNode::checkState(float targetX, float targetY, float targetZ){
	ofVec3f dist(myPerson.pos.x-pos.x, myPerson.pos.y-pos.y, 0);
		
	if( fabs(myPerson.pos.y - subNest1.pos.y ) < 100 ){
		
		printf("myPerson.pos is %f - subNest.pos is %f\n", myPerson.pos.y, subNest1.pos.y);
		fbManager.doBurst();
	
//		if( state == NODE_INACTIVE ){
//			setState(NODE_ACTIVATING);
//			fbManager.doBurst();
//		}
//		if( state == NODE_RISE && !bRise ){
//			setState(NODE_ACTIVATING_FALL);
//		}	
//		if( state == NODE_FALL && bRise ){
//			setState(NODE_ACTIVATING_RISE);
//		}				
	}
}

//---------------- 
bool deepBirdFishNode::shouldPersonWait(){
	return false;//( state == NODE_ACTIVATING );
}

//-----------------------------------
void deepBirdFishNode::update(){
	
	float hScreenW = 0.5 * ofGetWidth();
	
	subNest1.update(hScreenW*1.2, hScreenW*1.5, 3.9, 1.95);
	subNest2.update(hScreenW*1.2, hScreenW*1.5, 3.9, 1.95);
	
	if( bLoadFullTextures ){
		fbManager.fishBird.loadImage(birdPathFull);
		fbManager.birdFish.loadImage(fishPathFull);
	}else if( bLoadRegTextures ){
		fbManager.fishBird.loadImage(birdPathReg);
		fbManager.birdFish.loadImage(fishPathReg);	
	}
	
	fbManager.update();
}	

//-----------------------------------
void deepBirdFishNode::draw(){
//	ofPushStyle();
//	ofPopStyle();
}