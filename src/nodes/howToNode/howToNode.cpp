/*
 *  howToNode.cpp
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

#include "howToNode.h"
#include "ofxFutureUtils.h"

#include "magManager.h"
extern magManager tracker;

//--------------------------------------

void howToNode::setup(ofPoint position){
	setupName("howToNode");
	setPosition(position.x, position.y, position.z);
	
	vid1.loadMovie("assets/backcover/video/makingRiseFall.mov"); 
	vid1.setAnchorPercent(0.5, 0.5);
	
	howToBgImage.loadImage("assets/backcover/backgrounds/makingOf.jpg");
	howToBgImage.setAnchorPercent(0.5, 0.5);
	
	playImg.loadImage("assets/backcover/backgrounds/play.jpg");
	playImg.setAnchorPercent(0.5, 0.5);
	
	howToBg.setImageReference(howToBgImage, false);
	howToBg.setPosition(0,0,0);
	howToBg.setScale(1.0, 1.0, 1.0);
	howToBg.color.a = 0.0;
	
	moviePos.set(71 + 500/2, -264 + 375/2, 0);
		
	elements.clear();
	elements.push_back(this);
	
	bPlaying = false;
	timeActive = 0.0;
	pct		 = 0.0;
	
	state  	 = NODE_INACTIVE;
}


//--------------------------------------
//If you want to return this element (So that all drawing happens inside this class's draw() do this
//otherwise return your elements vector so that each element is depth sorted and drawn seperately
vector <baseElement *> howToNode::getElements(){
	return elements;
} 
		
//--------------------------------------
void howToNode::setState(nodeState stateIn){
	if( state != stateIn){
		state = stateIn;
		fireFirstTimeStateReached(state);
	}
}

//--------------------------------------
void howToNode::setPosition(float x, float y, float z){
	pos.set(x, y, z);
//	movieFrame.pos  = pos + ofPoint( 200, ofGetHeight()/2 + vid1.getHeight(), 0 );
//	movieFrame.home = pos + ofPoint(200, 200, 0);// + ofPoint( (float)ofGetWidth()/2, (float)ofGetHeight()/2, 0 );
}

//--------------------------------------
void howToNode::updateAppInfo(appInfo infoIn){
	info = infoIn;
}

//--------------------------------------
void howToNode::update(){
	vid1.update();
	
	if( state == NODE_ACTIVATING ){
		timeActive = 0.0;
		pct += 0.06;
		if( pct >= 1.0 ){
			pct = 1.0;
			timeActive = 0.0;
			setState(NODE_ACTIVE);
		}	
		
		ofSoundSetVolume( ofMap(pct, 0, 1, 1.0, 0.5, true) );
		ofSoundUpdate();		
	}
	
	if( state == NODE_ACTIVE ){
		timeActive += ofGetLastFrameTime();
	}
		
	if( state == NODE_DEACTIVATING ){
		pct -= 0.06;
		if( pct < 0.0 ){
			pct = 0.0;
			setState(NODE_INACTIVE);
			timeActive  = 0.0;
		}		
		
		ofSoundSetVolume( ofMap(pct, 0, 1, 1.0, 0.5, true) );		
		ofSoundUpdate();
	}

	howToBg.color.a = 255.0 * pct;
	
}


//----------------
void howToNode::checkState(float targetX, float targetY, float targetZ){

}

//---------------- 
bool howToNode::shouldPersonWait(){
	return false;
}

//--------------------------------------
void howToNode::draw(){
	ofPushStyle();

		ofPushMatrix();
			ofTranslate(pos.x, pos.y, 0);
			
			ofSetRectMode(OF_RECTMODE_CENTER);
			ofSetColor(2, 15, 18, 255);
			ofRect(0, 0, ofGetWidth()*2, ofGetHeight()*2);
			ofSetRectMode(OF_RECTMODE_CORNER);

			howToBg.draw();

			ofTrueTypeFont * ttf = globalType.getFontPtr("DIN_REG_S");
			
			globalColors.setColor("lightRise");
			int lineHeight = 34;
			ofPushMatrix();
				ofTranslate(-523, 133, 0);
				ttf->drawString(ofToString(info.numNodesActive), 0, 0);
				ttf->drawString(ofToString(info.numNodesToDiscover), 0, lineHeight);
				ttf->drawString(ofToString(info.numObjectsDrawn), 0, lineHeight*2);
				ttf->drawString(ofToString(info.numObjectsNotDrawn), 0, lineHeight*3);
			ofPopMatrix();
			
			lineHeight = 28;
			ofPushMatrix();
				ofTranslate(-113, -37, 0);
				ttf->drawString(ofToString(info.trackingPos.x, 0), 0, 0);
				ttf->drawString(ofToString(info.trackingPos.x, 0), 0, lineHeight);
				ttf->drawString(ofToString(info.magRot.z, 0), 0, lineHeight*2);
			ofPopMatrix();		

			ttf->drawString(ofToString(info.appFps, 2), -201, -91);
			
			ofVec2f vel = info.personVel;
			float mag = ofMap( vel.length(), 0, 7, 0.0, 1.0, true );
			vel.normalize();

			ofVec2f grav = info.worldGravity;
			grav.normalize();			

			ofSetCircleResolution(4);
			ofPushMatrix();
				ofTranslate(-75, 131, 0);
				ofCircle(0, 0, 2);
				ofLine(0, 0, grav.x * 15.0, -grav.y * 15.0);
			ofPopMatrix();
			
			ofPushMatrix();
				ofTranslate(-75, 182, 0);
				ofCircle(0, 0, 2);				
				ofLine(0, 0, vel.x * mag * 15.0, -vel.y * mag * 15.0);
			ofPopMatrix();
			
			tracker.debugDraw(-548, -192, 320, 240);
			
			ofSetColor(255, 255, 255, 255);
			vid1.draw(moviePos.x, moviePos.y, 500, 375);
			if( !bPlaying ){
				playImg.draw(moviePos.x, moviePos.y, 500, 375);
			}
			
		ofPopMatrix();
	
	ofPopStyle();
}


