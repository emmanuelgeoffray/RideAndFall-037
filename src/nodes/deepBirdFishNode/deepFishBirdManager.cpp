/*
 *  deepFishBirdManager.cpp
 *  ofxFernCameraDemo
 *
 *  Created by theo on 18/02/2010.
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

#include "deepFishBirdManager.h"

//-----------------------------------------------------------------------------------------
void deepFishBirdManager::setup(ofPoint pos1, ofPoint pos2, int numFishBirds, string birdTexPath, string fishTexPath){
	
	bNeedToBurst= false;
	bChanged	= false;
	bBird		= true;
	fishBird.loadImage(birdTexPath);
	birdFish.loadImage(fishTexPath);
	
	fb.assign(numFishBirds, birdFishParticle());
	
	ofPoint birthPos;
	lastTimeSoundPlayed = 0.0;
	
	for(int i = 0; i < fb.size(); i++){

		fb[i].visible = false;
		fb[i].imgPtr = &fishBird;
		fb[i].scale.x = 18.0;
		fb[i].scale.y = 8.0;	
		float randScale = ofRandom(0.5, 0.8);
		fb[i].scale.x *= randScale;
		fb[i].scale.y *= randScale;		
		
		float angle;
				
		if( i % 2 == 0 ){
			birthPos = pos1;
			angle  = ofRandom(-HALF_PI*0.1, HALF_PI*0.1);
		}else{
			birthPos = pos2;		
			angle  = PI + ofRandom(-HALF_PI*0.1, HALF_PI*0.1);			
		}
		
		fb[i].birthAngle = angle;		

		float x = birthPos.x + cos(angle) * ofRandom(120, 160);
		float y = birthPos.y + sin(angle) * ofRandom(120, 160);
		float z = birthPos.z - 10;
		
		fb[i].init( ofPoint(x, y, z),  1.0, ofPoint(), ofPoint(), (float)i/fb.size() );
		fb[i].homePoint = fb[i].pos;		
	}
}

//-----------------------------------------------------------------------------------------
vector <baseElement *> deepFishBirdManager::getElements(){
	vector <baseElement * > elementList;
	
	for(int i = 0; i < fb.size(); i++){
		elementList.push_back(&fb[i]);
	}
	
	return elementList;
}

//-----------------------------------------------------------------------------------------
void deepFishBirdManager::doBurst(){
	bNeedToBurst = true;
}

//-----------------------------------------------------------------------------------------
void deepFishBirdManager::update(){

	bool playSound = false;
		
	for(int i = 0; i < fb.size(); i++){
	
		if( fb[i].state == BF_WAITING_FIRST ){
			fb[i].visible = false;
		}
		
		if( bRise ){
			fb[i].imgPtr = &fishBird;
		}else{
			fb[i].imgPtr = &birdFish;
		}			

		float driftAmnt = ofMap( ofSignedNoise(0.0, ofGetElapsedTimef() * 0.07), -1, 1, 100, 400, true);	
		fb[i].update(driftAmnt);
		
		if( fb[i].state == BF_CHASING_RANDOM ){
			fb[i].setTargetPointer(&myPerson.pos);
			fb[i].state = BF_CHASING_PERSON;
		}
		
		if( fb[i].isChasingPerson() && fb[i].distToPoint(myPerson.pos) > driftAmnt * 1.33 && fabs(myPerson.vel.y) < 0.1 ){
			fb[i].goHome();
		}

		if( ( fb[i].state == BF_WAITING_FIRST || fb[i].state == BF_WAITING ) && bNeedToBurst ){
			fb[i].doBurst(ofGetHeight()*0.5);
			fb[i].visible = true;
						
			//TODO: Temp fix as it keeps triggering
			playSound = true;
		}
		
		if( fb[i].state == BF_WAITING ){
			if( fb[i].distToPoint(myPerson.pos) < 230 ){
				fb[i].state = BF_CHASING_PERSON;
				fb[i].setTargetPointer(&myPerson.pos);
			}
		}
	}
	
	if( bNeedToBurst ){
		bNeedToBurst = false;
	}
	
	if( playSound && ofGetElapsedTimef() - lastTimeSoundPlayed  > 5.0 ){
		if( bRise ){
			globalSnd.play("BURST_BIRDS", 0.8, 1.0, 0.0);
		}else{
			globalSnd.play("BURST_FISH", 0.8, 1.0, 0.0);
		}
		lastTimeSoundPlayed = ofGetElapsedTimef();
	}

	
}
