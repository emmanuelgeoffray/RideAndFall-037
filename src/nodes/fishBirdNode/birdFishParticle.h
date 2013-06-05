/*
 *  birdFishParticle.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 14/02/2010.
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

#pragma once
#include "textureTrailParticle.h"

typedef enum{
	BF_WAITING_FIRST,
	BF_WAITING,
	BF_BURST,
	BF_CHASING_PERSON,
	BF_CHASING_RANDOM,
	BF_NODE_TARGET,
	BF_RETURNING_HOME,
}birdFishState;

class birdFishParticle : public textureTrailParticle{
	public:
	
	birdFishParticle(){
		state		= BF_WAITING_FIRST;
		targetPtr	= NULL;
		indexPct	= 0.0;
		birthAngle  = 0.0;
	}
	
	void init(ofPoint position, float size, ofPoint velocity, ofPoint gravity, float pct){
		birthPos	= position;
		unique		= ofRandom(0, 10000);
		indexPct	= pct;
		textureTrailParticle::init( position,  size,  velocity, gravity);
	}
	
	bool isChasingPerson(){
		return state == BF_CHASING_PERSON;
	}
	
	void doBurst(float burstDist){
		if( state == BF_WAITING_FIRST ||  state == BF_WAITING ){
			homePoint.x = birthPos.x + cos(birthAngle) * burstDist;
			homePoint.y = birthPos.y + sin(birthAngle) * burstDist;	
			visible		= true;
			state		= BF_BURST;		
		}
	}
	
	void goHome(){
		homePoint = birthPos;
		state = BF_RETURNING_HOME;
	}
	
	void setTargetPointer(ofPoint * pos){
		targetPtr = pos;
		homePoint = *targetPtr;
	}
	
	float distToPoint(ofPoint & posIn){
		ofVec3f delta = posIn-pos;
		return delta.length();
	}
	
	void update(float shiftAmntPerson = 240){
	
		if( state == BF_RETURNING_HOME && distToPoint(homePoint) < 40 ){
			state = BF_WAITING;
		}
		
		if( targetPtr != NULL && ( state >= BF_CHASING_PERSON && state <= BF_NODE_TARGET ) ){
			homePoint = *targetPtr;
		}
		
		if( state == BF_BURST && distToPoint(homePoint) < 40 ){
			state = BF_CHASING_RANDOM;
		}

		float rate = 0.2;
		float shiftAmnt = 24.0;
		
		if( state >= BF_CHASING_PERSON && state <= BF_NODE_TARGET ) {
			shiftAmnt = shiftAmntPerson;
			rate = 0.45;
		}
		
		if( state == BF_CHASING_RANDOM ){
			shiftAmnt = 800.0;
		}

		ofPoint shiftPos = ofPoint( ofSignedNoise(unique, ofGetElapsedTimef() * rate) * shiftAmnt, ofSignedNoise(-unique, ofGetElapsedTimef() * rate) * shiftAmnt );

		
		if( visible ){
			float uRate = ofNoise(unique, ofGetElapsedTimef() * 0.1, 0.1) * 0.3;
			textureTrailParticle::update(0.88, 0.3 + uRate , homePoint + shiftPos, 35.0 );
		}

	}
	
	float birthAngle;
	float indexPct;
	ofPoint birthPos;
	ofPoint * targetPtr;
	birdFishState state;
};