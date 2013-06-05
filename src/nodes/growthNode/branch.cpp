/*
 *  branch.cpp
 *  ofxFernCameraDemo
 *
 *  Created by theo on 11/02/2010.
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

#include "branch.h"

//------------------------------------------------------------------------------
branch::branch(){
	drawPct = 0.0;
	leafHealthPct = 1.0;
	level = 0;
}

//------------------------------------------------------------------------------
void branch::setLevel(int levelIn){
	level = levelIn;
}

//------------------------------------------------------------------------------
float branch::getRootAngle(){
	if( lines.size() ){
		return lines[0].getAngle();
	}
	return 0.0;
}

//------------------------------------------------------------------------------
void branch::update(float factor, float drawPctIn){
	
	if( drawPct < 0.2 && drawPctIn >= 0.2 ){
		if( lines.size() ){
			ofPoint tipPos = lines.back().getTip();
			globalSnd.play("BRANCH_GROWING", ofRandom(0.8, 0.9), 1.0, ofMap(tipPos.x, -(float)ofGetWidth()/2, (float)ofGetWidth()/2, -1, 1, true) );
		}
	}
		
	if( ourLeafState == LEAF_DYING && leafHealthPct > 0.01 ){
		leafHealthPct *= 0.98;
		if( leafHealthPct <= 0.01 ){
			leafHealthPct = 0.0;
		}
	}
	if( ourLeafState == LEAF_GROWING && leafHealthPct < 1.0 ){
		leafHealthPct += 0.03;
		if( leafHealthPct >= 1.0 ) leafHealthPct = 1.0;
		
		for(int i = 0; i < lines.size(); i++){
			for(int k = 0; k < lines[i].leafs.size(); k++){
				if( lines[i].leafs[k].visible == false ){
					lines[i].leafs[k].visible = true;
				}
				lines[i].leafs[k].shrink = ofLerp(lines[i].leafs[k].shrink, 1.0, 0.1); 
			}
		}
	}
	
	if( lines.size() && level == 0 && drawPctIn >= 1.0){
		if( leafHealthPct < 0.7 && ourLeafState == LEAF_DYING ){
							
			updateAngle(270);
			for(int i = 0; i < children.size(); i++){
				children[i].updateAngle(270);
			}
		}
	}
	
	drawPct = drawPctIn;
	
	for(int i = 0; i < lines.size(); i++){				
		lines[i].update(factor, leafHealthPct);
		lines[i].scale = powf(ofClamp(drawPct*4, 0, 1), 0.2);
		
		if( leafHealthPct > 0.3 && ourLeafState != LEAF_DYING ){
			lines[i].restoreAngleToTarget();
		}
	
	}
	for(int k = 0; k < children.size(); k++){
		children[k].update(factor, MAX(0, ofLerp(children[k].drawPct, drawPct-1.0, 0.1)) );
	}
	
}

//------------------------------------------------------------------------------
void branch::updateAngle(float target){				
	for(int i = 0; i < lines.size(); i++){
		lines[i].setTargetAngle( target );
	}
}

//------------------------------------------------------------------------------
void branch::killLeaves(){
	ourLeafState = LEAF_DYING;
	for(int k = 0; k < children.size(); k++){
		children[k].killLeaves();
	}
}

//------------------------------------------------------------------------------
void branch::growLeaves(){
	ourLeafState = LEAF_GROWING;
	for(int k = 0; k < children.size(); k++){
		children[k].growLeaves();
	}
}

//------------------------------------------------------------------------------
bool branch::leavesDead(){
	return (leafHealthPct == 0.0);
}

//------------------------------------------------------------------------------
void branch::releaseLeaves(vector <leafElement> & fallingLeaves){
	
	for(int i = 0; i < lines.size(); i++){
		for(int k = 0; k < lines[i].leafs.size(); k++){
			if( ofRandom(0, 6) >= 4 )continue;
			
			fallingLeaves.push_back(lines[i].leafs[k]);
			fallingLeaves.back().vel.y = ofRandom(4.0, 10.0);
			fallingLeaves.back().vel.x = ofRandom(-4.0, 4.0);						
			fallingLeaves.back().drag = ofRandom(0.967, 0.988);
			fallingLeaves.back().timeToLive = ofGetElapsedTimef() + ofRandom(2.0, 5.0);
			fallingLeaves.back().pos.z += ofRandom(-3, 3);
			fallingLeaves.back().scale.x *= 0.7;
			fallingLeaves.back().scale.y *= 0.7;
			
			lines[i].leafs[k].shrink = 0.0;
			lines[i].leafs[k].visible = false;
		}
	}
	
	if( drawPct > 1.0){
		for(int d = 0; d < children.size(); d++){
			children[d].releaseLeaves(fallingLeaves);
		}
	}
}

//------------------------------------------------------------------------------
void branch::draw(){
	for(int k = 0; k < children.size(); k++){
		children[k].draw();
	}

	for(int i = 0; i < lines.size(); i++){
		if( !lines[i].active )continue;
		
		ofSetLineWidth(3.0);			
		globalColors.setColor("darkFall");
		glBegin(GL_LINES);
			glVertex3fv(lines[i].root.getPtr());
			glVertex3fv(lines[i].tip.getPtr());												
		glEnd();
		
		ofSetLineWidth(1.0);
		ofPushMatrix();
			ofTranslate(lines[i].root.x, lines[i].root.y, lines[i].root.z);
			globalColors.setColor("midBlue");
			ofFill();
			ofCircle(0, 0, lines[i].circleSize *  ofClamp(drawPct, 0, 1) );
		ofPopMatrix();
	}

	for(int i = 0; i < lines.size(); i++){
		if( !lines[i].active )continue;
		lines[i].drawLeafs( ofMap( ofClamp(drawPct, 0, 1), 0.4, 1.0, 0.0, 1.0, true) );
	}
	
}
