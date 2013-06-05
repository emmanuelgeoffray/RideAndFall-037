/*
 *  nestSubNode.cpp
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

#include "nestSubNode.h"

//-----------------------------------
void nestSubNode::setup(ofPoint position, float minRadius, float maxRadius, string tex1Path, string tex2Path){
	
	setupName("nestSubNode");
	setPosition(position.x, position.y, position.z);
	
	textureImage.loadImage(tex1Path);
	textureImage2.loadImage(tex2Path);
			
	pct		= 0.0;
	prePct	= 0.0;
	
	int numLevels	= 20;
	int numPer		= 38;

	magnets.assign(numLevels*numPer, magnetElement());
	for(int k = 0; k < numLevels; k++){
		
		float rad = ofMap(k, 0, numLevels-1, maxRadius, minRadius);
		float angleOff = 0;
		if( k % 2 != 0 ){
			angleOff += TWO_PI / ( 2.0 * numPer );
		}
		
		for(int j = 0; j < numPer; j++){
			int i = k * numPer + j;	
			
			if( k % 2 == 0 ){
				i = k * numPer + ((numPer-1)-j);	
			}
			
			float angle = angleOff + ofMap(j, 0, numPer-1, -PI, PI);
			
			magnets[i].setTexturePtr( &(textureImage.getTextureReference()));
			magnets[i].setPosition(pos.x + cos(angle) * rad, pos.y + sin(angle)*rad, pos.z);
			
			float scaleAmnt = ofRandom(0.9, 1.3);
			magnets[i].width  *= scaleAmnt *0.6;
			magnets[i].height *= scaleAmnt *0.6;
			magnets[i].angle   = ofRandom( -PI, PI);
			
			magnets[i].color.set(ofRandom(160, 255));
		}
	}
	
	killState   = NODE_UNKILLABLE;
	state		= NODE_INACTIVE;
}

//----------------
void nestSubNode::checkState(float targetX, float targetY, float targetZ){
	ofVec3f dist(targetX-pos.x, targetY-pos.y, 0);
		
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

//-----------------------------------
void nestSubNode::update(float minDist, float maxDist, float nearScale, float farScale){
	
	//TODO: optimize the magnet code
	
	ofPoint personPos = myPerson.pos;	
	if( bRise ){
		personPos.y += 140.0;
	}else{
		personPos.y -= 140.0;
	}

	ofVec3f d;
	for(int i = 0; i < magnets.size(); i++){
		d= personPos - magnets[i].pos;		
		magnets[i].stretch = ofMap(d.lengthSquared(), minDist*minDist, maxDist*maxDist, nearScale, farScale, true);
		float angle = -45 + (RAD_TO_DEG*atan2f(d.y, d.x));
		magnets[i].update( angle );
	}
	
}	

//-----------------------------------
void nestSubNode::draw(){
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
		
	ofPopStyle();
}