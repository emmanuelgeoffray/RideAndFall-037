/*
 *  fishBirdNode.cpp
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

#include "fishBirdNode.h"
#include "handTextManager.h"

extern handTextManager leftHand1;
extern handTextManager rightHand1;

//-------------------------------------------------------
fishBirdNode::fishBirdNode(){
	maxBoundsY = 300;
	minBoundsY = -700;
}

//-----------------------------------
void fishBirdNode::setup(ofPoint position){
	
	setupName("fishBirdNode");
	setPosition(position.x, position.y, position.z);
	
	textureImage.loadImage("assets/nodes/node02-birdFish/rise/colorTex.jpg");
	textureImage2.loadImage("assets/nodes/node02-birdFish/fall/colorTex.jpg");
	
	bg.loadImage("assets/nodes/node02-birdFish/both/sun02.png", ofPoint(0.5, 0.5), true);
	bg.setPosition(pos.x, pos.y, pos.z);


	birdPathReg = "assets/nodes/node02-birdFish/both/fishBird/bird-reg.png";
	birdPathFull = "assets/nodes/node02-birdFish/both/fishBird/bird-full.png";
	fishPathReg = "assets/nodes/node02-birdFish/both/fishBird/fish-reg.png";
	fishPathFull = "assets/nodes/node02-birdFish/both/fishBird/fish-full.png";	
	
	fbManager.setup(pos, 15, birdPathReg, fishPathReg);
	
	introFinished = false;
		
	pct		= 0.0;
	prePct	= 0.0;
	
	textCount = 0;
	
	int numLevels	= 20;
	int numPer		= 30;

	magnets.assign(numLevels*numPer, magnetElement());
	for(int k = 0; k < numLevels; k++){
		
		float rad = ofMap(k, 0, numLevels-1, 260, 70);
		float angleOff = ofRandom(0, TWO_PI);;
		if( k % 2 != 0 ){
			angleOff += TWO_PI / ( 2.0 * numPer );
		}
		
		for(int j = 0; j < numPer; j++){
			int i = k * numPer + j;	
			
			if( k % 2 == 0 ){
				i = k * numPer + ((numPer-1)-j);	
			}
			
			float angle = angleOff + ofMap(j, 0, numPer, 0, TWO_PI);
			
			magnets[i].setTexturePtr( &(textureImage.getTextureReference()));
			magnets[i].setPosition(pos.x + cos(angle) * rad, pos.y + sin(angle)*rad, pos.z);
			
			float scaleAmnt = ofRandom(0.9, 1.3);
			magnets[i].width  *= scaleAmnt *0.6;
			magnets[i].height *= scaleAmnt *0.6;
			
			magnets[i].color.set(ofRandom(160, 255));
		}
	}
	
	vector <riseFallString> str = textStackFromXml::loadFromXml("assets/text/riseFallText.xml", "birdNodeBegin");
	if( bRise ){
		nodeText.setup( ofPoint(pos.x+280, pos.y - 600, pos.z - 10), str, false, true);
	}else{
		nodeText.setup( ofPoint(pos.x+280, pos.y + 200, pos.z - 1), str, true, true);
	}
	
	killState   = NODE_UNKILLABLE;
	state		= NODE_INACTIVE;
}

//-----------------------------------
//If you want to return this element (So that all drawing happens inside this class's draw() do this
//otherwise return your elements vector so that each element is depth sorted and drawn seperately
vector <baseElement *> fishBirdNode::getElements(){
	vector <baseElement *> elementList = nodeText.getElements();
	
	vector <baseElement *> fbList = fbManager.getElements();
	
	for(int i = 0; i < fbList.size(); i++){
		elementList.push_back(fbList[i]);
	}
	
	elementList.push_back(this);
	return elementList;
} 

//----------------
void fishBirdNode::checkState(float targetX, float targetY, float targetZ){
	ofVec3f dist(targetX-pos.x, targetY-pos.y, 0);

	if( nodeText.getState() == TEXT_STACK_INACTIVE && fabs( targetY - nodeText.pos.y) < 350 ){
		nodeText.activate();			
		printf("-------------------------------ACTIVATING!!!!!!!\n");
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
bool fishBirdNode::shouldPersonWait(){
	return ( state == NODE_ACTIVATING );
}


//-----------------------------------
void fishBirdNode::update(){
	ofVec3f d;
	
	if( bLoadFullTextures ){
		fbManager.fishBird.loadImage(birdPathFull);
		fbManager.birdFish.loadImage(fishPathFull);
	}else if( bLoadRegTextures ){
		fbManager.fishBird.loadImage(birdPathReg);
		fbManager.birdFish.loadImage(fishPathReg);	
	}	
	
	nodeText.update(0.016);
	
	if( nodeText.textDone() && state == NODE_ACTIVATING ){
		introFinished = true;
	}
	
	if( introFinished && pct < 1.0 ){
		pct += 0.06;
		if( pct > 1.0 ){
			pct = 1.0;
			setState(NODE_ACTIVE);	
			nodeText.deactivate();			
		}
	}
	
	if( textCount < 2 && state == NODE_ACTIVE && nodeText.getState() == TEXT_STACK_DEACTIVE ){
	
		if( textCount == 0 && !leftHand1.isTextBusy() ){

			string text = "A feathered friend,\nwith pointed beak.";
			if( !bRise ){
				text = "A different scale,\nshimmers in the night.";
			}
			leftHand1.triggerSideText(text, 3.0);
			textCount++;
		}else if( textCount == 1 && !leftHand1.isTextBusy() ){
			string text = "Takes a ride,\nOn the tale we seek.";
			if( !bRise ){
				text = "What swims beneath,\nmight soon take flight.";
			}
			textCount++;
			rightHand1.triggerSideText(text, 3.0);			
		}
		
	}
	
	ofPoint personPos = myPerson.pos;
	
	if( bRise ){
		personPos.y += 140.0;
	}else{
		personPos.y -= 140.0;
	}
	
	if( pct <= 1.0 ){
		scaleFactor = ofMap(pct, 0.0, 1.0, 0.35, 1.2, true);
	}else{
		//float distToCenter = ofVec2f(personPos.x-pos.x, personPos.y-pos.y).length();
		//scaleFactor = ofLerp(scaleFactor, ofMap(distToCenter, 300, 800, 1.2, 0.5, true), 0.1);
		scaleFactor = 1.2;
	}
	
	float ourScale = scaleFactor;
	
	bg.setScale(ourScale*0.9, ourScale*0.9, 1);
		
	for(int i = 0; i < magnets.size(); i++){
	
		magnets[i].updatePosViaStretch(ourScale, pos);
		
		d = personPos - magnets[i].pos;		
		magnets[i].stretch = scaleFactor * ofMap(d.lengthSquared(), 200000, 500000, 3.0, 1.95, true);
		float angle = -45 + (RAD_TO_DEG*atan2f(d.y, d.x));

		magnets[i].update( angle );
	}
	
	fbManager.update();
	
	if( pct > 0.25 ){
		fbManager.doBurst();
	}
	
	
}	

//-----------------------------------
void fishBirdNode::draw(){
	ofPushStyle();
		ofEnableAlphaBlending();
		ofSetColor(255, 255, 255, 255);
				
		ofTexture * tex;
		
		if(bRise){
			tex = &textureImage.getTextureReference();
		}else{
			tex = &textureImage2.getTextureReference();
		}
		
		tex->bind();
		
		glBegin(GL_TRIANGLES);
		for(int i = 0; i < magnets.size(); i++){
			magnets[i].draw();
		}
		glEnd();

		tex->unbind();

		ofSetColor(255, 255, 255, 255);
		bg.draw();
		
	ofPopStyle();
}