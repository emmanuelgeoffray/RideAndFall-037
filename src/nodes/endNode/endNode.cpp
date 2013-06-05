/*
 *  endNode.cpp
 *  ofxFernCameraDemo
 *
 *  Created by theo on 20/02/2010.
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

#include "endNode.h"
#include "imageElement.h"

extern imageElement endGraphic;

//-----------------------------------
void endNode::setup(ofPoint position){
	
	setupName("endNode");
	setPosition(position.x, position.y, position.z);
	
	vector <riseFallString> str = textStackFromXml::loadFromXml("assets/text/riseFallText.xml", "endNode");
	
	if( bRise ){
		nodeText.setup( ofPoint(pos.x, pos.y-200, pos.z + 1), str, false, true);
	}else{
		nodeText.setup( ofPoint(pos.x, pos.y+150, pos.z + 1), str, true, true);
	}
	
//	sunMoon.loadImage("assets/nodes/node15-end/end.png");
//	
	if( bRise ){
		endGraphic.setPosition(pos.x, pos.y+1000, pos.z);
	}else{
		endGraphic.setPosition(pos.x, pos.y-1000, pos.z);
	}
	
	pct = 0.0;
	
	createdInRise = bRise;
	elements.push_back( &nodeText );
	
	killState   = NODE_UNKILLABLE;
	state		= NODE_INACTIVE;
}


//-----------------------------------
//If you want to return this element (So that all drawing happens inside this class's draw() do this
//otherwise return your elements vector so that each element is depth sorted and drawn seperately
vector <baseElement *> endNode::getElements(){
	vector <baseElement *> elementList = nodeText.getElements();
	
	for(int i = 0; i < elements.size(); i++){
		elementList.push_back(elements[i]);
	}
	return elementList;
} 

//----------------
void endNode::checkState(float targetX, float targetY, float targetZ){
	ofVec3f dist(targetX-pos.x, targetY-pos.y, 0);
	
	if( nodeText.getState() == TEXT_STACK_INACTIVE && fabs(targetY - nodeText.pos.y) < 500 ){
		nodeText.activate();			
	}
		
	if( fabs(dist.y) < 100 ){
		if( state == NODE_INACTIVE ){
			setState(NODE_ACTIVATING);
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
bool endNode::shouldPersonWait(){
	return ( state == NODE_ACTIVATING );
}


//-----------------------------------
void endNode::update(){
	
	nodeText.update(0.04);
	
	if( state == NODE_ACTIVATING){
		pct += 0.08;
		if( pct > 1.0 ){
			pct = 1.0;
			setState(NODE_ACTIVE);
		}
	}
		
}	

//-----------------------------------
void endNode::draw(){

}