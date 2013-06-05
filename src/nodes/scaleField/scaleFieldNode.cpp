/*
 *  scaleFieldNode.cpp
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

#include "scaleFieldNode.h"

//-------------------------------------------------------
scaleFieldNode::scaleFieldNode(){
	maxBoundsY = 150;
	minBoundsY = -150;
}

//-------------------------------------------------------
void scaleFieldNode::setup(ofPoint position){
			
	setupName("scaleFieldNode");
	setPosition(position.x, position.y, position.z);

	seq.loadSequence(globalSeqLoader.getSequencePtr("SCALE_SEQ"), ofPoint(0.5, 1.0), false);
	seq.setPosition(position.x-(ofGetWidth()/2.0)-10, position.y, position.z);
	seq.setRotation(0, 0, 90);
	
	plusImage.loadImage("assets/nodes/node04-scaleField/rise/graphic.png");
	minusImage.loadImage("assets/nodes/node04-scaleField/fall/graphic.png");
	plusImage.setAnchorPercent(0.5, 0.5);
	minusImage.setAnchorPercent(0.5, 0.5);
	
	state = NODE_INACTIVE;
	
	particles.assign(190, scaleElement());
	for(int i = 0; i < particles.size(); i++){
		particles[i].setImageReference(plusImage, true);
		particles[i].color = globalColors.getColor("red");
		
		particles[i].setDrag(ofRandom(0.91, 0.95));
		particles[i].setUniqueVal(ofRandom(0, 40.0));
		
		float scaleAmnt = ofRandom(0.1, 0.9);
		float zShift    = ofMap(scaleAmnt, 0.3, 0.9, 10, 90, true);
		
		particles[i].setScale(scaleAmnt, scaleAmnt, 1);
		
		float xAmnt  = ofRandom(0, ofGetWidth());
		float yAmnt  = ofMap( xAmnt, 0, ofGetWidth()/4, 60, 280, true);
		
		particles[i].setTargetPosition( seq.pos.x + 150 + xAmnt, pos.y + ofRandom(-yAmnt, yAmnt), pos.z + zShift);
		particles[i].setPosition( seq.pos.x + 150, pos.y, pos.z-10);

		elements.push_back(&particles[i]);
	}
	
	elements.push_back(this);
}

//------------------------------------------------------- 
vector <baseElement *> scaleFieldNode::getElements(){
	return elements;
}

//-------------------------------------------------------
void scaleFieldNode::update(){
	
	
	seq.update();
	
	if( fabs(myPerson.pos.y - pos.y) < 500 ){
		
		if( seq.getPercent() > 0.3){
			for(int i = 0; i < particles.size(); i++){
				particles[i].release(ofRandom(3, 20));
			}
		}
		if( !seq.bPlay ){
			seq.play();
			globalSnd.play("SCALE_HAND_REVEAL", ofRandom(0.8, 0.9), 1.0, -0.96);
		}
	}
	
	bool doneScale = false;
	
	for(int i = 0; i < particles.size(); i++){
		particles[i].update(2.0, 1.3, 1.2);
		if( bRise ){
			particles[i].setImageReference(plusImage, true);
		}else{
			particles[i].setImageReference(minusImage, true);
		}
		
		if( !doneScale && particles[i].bReleased && particles[i].scale.x > 0.5 ){
			float dx = fabs( myPerson.pos.x - particles[i].pos.x );
			float dy = fabs( myPerson.pos.y - particles[i].pos.y );
			
			if( dx < 60 && dy < 60 ){
				if( bRise){ 
					myPerson.increaseScale(0.1, 1.5);
					globalSnd.play("SCALE_PERSON_HIT_PLUS", ofRandom(0.6, 0.9), 1.0, ofMap(myPerson.pos.x, -ofGetWidth()/2, ofGetWidth()/2, -1, 1, true));
				}
				else{
					myPerson.decreaseScale(0.2, 1.5);
					globalSnd.play("SCALE_PERSON_HIT_MINUS", ofRandom(0.6, 0.9), 1.0, ofMap(myPerson.pos.x, -ofGetWidth()/2, ofGetWidth()/2, -1, 1, true));
				}
				doneScale = true;
			}
		}
	}
			
//	if( seq.isDone()){
//		
//		if( seq.currentSection.name == "poke" ){
//			myPerson.burstBalloon();
//		}
//		
//		seq.next();
//	}
//	

}

//-------------------------------------------------------
//void scaleFieldNode::updateCamToScreenCoords(){
//	
//	GLdouble model_view[16];
//	glGetDoublev(GL_MODELVIEW_MATRIX, model_view);
//
//	GLdouble projection[16];
//	glGetDoublev(GL_PROJECTION_MATRIX, projection);
//
//	GLint viewport[4];
//	glGetIntegerv(GL_VIEWPORT, viewport);
//		
//	GLdouble X, Y, Z;		
//	
//	if( seq.currentSection.name != "close"  ){
//		storePersonPos = myPerson.pos;
//	}
//	
//	if( seq.bPlay ){
//		gluProject(storePersonPos.x - 50, storePersonPos.y + 45, storePersonPos.z, model_view, projection, viewport, &X, &Y, &Z);
//		myPersonScreenPos.set(X, ofGetHeight()-Y, Z);
//		seq.setPosition(myPersonScreenPos.x, myPersonScreenPos.y, 0);		
//	}
//		
//}

//-------------------------------------------------------
void scaleFieldNode::draw(){
	ofPushStyle();
		seq.draw();
	ofPopStyle();
}		
