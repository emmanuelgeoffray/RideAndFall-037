/*
 *  plantNode.cpp
 *  ofxFernCameraDemo
 *
 *  Created by theo on 16/02/2010.
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

#include "plantNode.h"

//-----------------------------------
plantNode::plantNode(){
	maxBoundsY = 1000;
	minBoundsY = -1000;
}

//-----------------------------------
void plantNode::setup(ofPoint position){	
	setupName("plantNode");
	setPosition(position.x, position.y, position.z);
	
	subNodes.clear();
	subNodes.assign(3, plantSubNode());
	
	subNodes[0].setup(pos + ofPoint(-300, 0, 0), "A leafy welcome", 0.01, 0.05); 
	subNodes[1].setup(pos + ofPoint(500, ofRandom(-150, 150), -200), "As we fall away.", 0.01, 0.05); 
	subNodes[2].setup(pos + ofPoint(-700, ofRandom(350, 650), -900) , "Waves goodbye,",  0.02, 0.05); 
	
	subNodes[0].signPostOffset = 0.0;
	subNodes[1].signPostOffset = -1.2;
	subNodes[2].signPostOffset = -2.0;
	
	
	vector <riseFallString> str = textStackFromXml::loadFromXml("assets/text/riseFallText.xml", "plantNode");
	nodeText.setup( ofPoint(pos.x, pos.y+250, pos.z + 30), str, false);
		
	killState = NODE_UNKILLABLE;
	state	  = NODE_INACTIVE;
}

//-----------------------------------

//If you want to return this element (So that all drawing happens inside this class's draw() do this
//otherwise return your elements vector so that each element is depth sorted and drawn seperately
vector <baseElement *> plantNode::getElements(){
	vector <baseElement *> elementList;
	
	for(int i = 0; i < subNodes.size(); i++){
		vector <baseElement *> tmp = subNodes[i].getElements();
		for(int j = 0; j < tmp.size(); j++){
			elementList.push_back(tmp[j]);
		}
	}
	
	//elementList.push_back(&nodeText);	
	elementList.push_back(this);
	return elementList;
} 

//----------------
void plantNode::checkState(float targetX, float targetY, float targetZ){
	ofVec3f dist(targetX-pos.x, targetY-pos.y, 0);
	
	if( fabs(dist.y) < 90 ){
	
		if( state == NODE_INACTIVE && bRise ){
			nodeText.activate();
			setState(NODE_ACTIVATING_RISE);		
			globalSnd.play("PLANT_START_GROW", 0.8, 1.0, 0.0);		
			for(int i = 0; i < subNodes.size(); i++){
				subNodes[i].setState(NODE_ACTIVATING_RISE);
			}
		}
		if( state == NODE_RISE && !bRise ){
			globalSnd.play("PLANT_WILT", 0.8, 1.0, 0.0);				
			setState(NODE_ACTIVATING_FALL);
			for(int i = 0; i < subNodes.size(); i++){
				subNodes[i].setState(NODE_ACTIVATING_FALL);
			}
		}	
		if( state == NODE_FALL && bRise ){
			globalSnd.play("PLANT_START_GROW", 0.8, 1.0, 0.0);				
			setState(NODE_ACTIVATING_RISE);					
			for(int i = 0; i < subNodes.size(); i++){
				subNodes[i].setState(NODE_ACTIVATING_RISE);
			}
		}				
	}
}

//---------------- 
bool plantNode::shouldPersonWait(){
	return (state == NODE_ACTIVATING || state == NODE_ACTIVATING_FALL || state == NODE_ACTIVATING_RISE );
}

//----------------
bool plantNode::isKillable(){
	return false;
	
//	int numLeaves = 0;
//	
//	for(int k = 0; k < subNodes.size(); k++){
//		numLeaves += subNodes[k].fallingLeaves.size();
//	}
//	
//	return (numLeaves == 0);
}

//-----------------------------------
void plantNode::update(){

	nodeText.update(0.04);
	
	for(int k = 0; k < subNodes.size(); k++){
		subNodes[k].update();
	}
	
	if( state == NODE_ACTIVATING_RISE ){
		bool bDoneActivating = true;
		for(int k = 0; k < subNodes.size(); k++){
			if( subNodes[k].state == NODE_RISE ){
				setState(NODE_RISE);
			}
		}
	}

	if( state == NODE_ACTIVATING_FALL ){
		bool bDoneActivating = true;
		for(int k = 0; k < subNodes.size(); k++){
			if( subNodes[k].state == NODE_FALL ){
				setState(NODE_FALL);
			}
		}
	}

}


//-----------------------------------
void plantNode::draw(){
	ofPushStyle();
	
	ofPopStyle();			
}		

		