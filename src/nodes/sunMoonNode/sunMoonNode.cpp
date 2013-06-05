/*
 *  sunMoonNode.cpp
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

#include "sunMoonNode.h"

//-------------------------------------------------------
sunMoonNode::sunMoonNode(){
	maxBoundsY = 200;
	minBoundsY = -200;
}

//-----------------------------------
void sunMoonNode::setup(ofPoint position){
	
	setupName("sunMoonNode");
	setPosition(position.x, position.y, position.z);
	
	vector <riseFallString> str = textStackFromXml::loadFromXml("assets/text/riseFallText.xml", "sunMoonNode");
	
	if( bRise ){
		nodeText.setup( ofPoint(pos.x, pos.y+250, pos.z -4), str, false, true);
	}else{
		nodeText.setup( ofPoint(pos.x, pos.y+100, pos.z -4), str, true, true);
	}
	
	sunMoon.loadImage("assets/nodes/node01-sunMoon/rise/sunMoon1.png", "assets/nodes/node01-sunMoon/fall/sunMoon1.png");
	sunMoon.setPosition(pos.x, pos.y, pos.z);
	
	elements.push_back( &nodeText );
	
	pct = 0.0;
	
	killState   = NODE_UNKILLABLE;
	state		= NODE_INACTIVE;
}


//-----------------------------------
//If you want to return this element (So that all drawing happens inside this class's draw() do this
//otherwise return your elements vector so that each element is depth sorted and drawn seperately
vector <baseElement *> sunMoonNode::getElements(){
	vector <baseElement *> elementList = nodeText.getElements();
	
	for(int i = 0; i < elements.size(); i++){
		elementList.push_back(elements[i]);
	}
	elementList.push_back(this);
	return elementList;
} 

//----------------
void sunMoonNode::checkState(float targetX, float targetY, float targetZ){
	ofVec3f dist(targetX-pos.x, targetY-pos.y, 0);
	
	if( state == NODE_INACTIVE && bRise && targetY > pos.y - 200 ){
		nodeText.activate();			
		globalSnd.play("SUN_MOON", 0.8, 1.0, 0.0);		
	}else if( state == NODE_INACTIVE && !bRise && targetY > pos.y + 400 ){
		nodeText.activate();	
		globalSnd.play("SUN_MOON", 0.8, 1.0, 0.0);
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
bool sunMoonNode::shouldPersonWait(){
	return ( state == NODE_ACTIVATING );
}


//-----------------------------------
void sunMoonNode::update(){
		
	nodeText.update(0.04);
	
	if( state == NODE_ACTIVATING ){
		pct += 0.08;
		if( pct > 1.0 ){
			pct = 1.0;
			setState(NODE_ACTIVE);
		}
	}
	
	sunMoon.setFade(1.0-riseFallPct);
	
}	

//-----------------------------------
void sunMoonNode::draw(){
	ofPushStyle();
		ofEnableAlphaBlending();
		ofSetColor(255, 255, 255, 255);
		sunMoon.draw();			
	ofPopStyle();
}