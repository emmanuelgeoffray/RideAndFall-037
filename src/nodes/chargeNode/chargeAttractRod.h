/*
 *  chargeAttractRod.h
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

#pragma once
#include "ofMain.h"
#include "baseElement.h"
#include "ofxFutureUtils.h"
#include "_projectGlobals.h"

static float lastTimeChargeSoundPlayed;

class charge{

	public:
		charge(){
			myForce		= 0.0;
			percent		= ofRandom(0.0, 0.24);
			percentVel	= 0.0;
			drag		= 0.97;
		}

		void update(){

			percentVel *= drag;
			percentVel += myForce;
			
			percent += percentVel;
			
			if( percent > 1.0 ){
				percent = 1.0;
				percentVel = 0.0;
				
				if( ofGetElapsedTimef() -lastTimeChargeSoundPlayed > ofRandom(0.07, 0.5) ){
					float volPct = ofMap(fabs(myPerson.pos.y - pt.y), 70, ofGetHeight()/1.5, 1.0, 0.0, true); 
						
					if( volPct > 0.0 ){
						globalSnd.play("CHARGE_REACHED_TIP", ofRandom(0.3, 0.7)*volPct, 1.0, pt.x<0?-0.9:0.9 );
						lastTimeChargeSoundPlayed = ofGetElapsedTimef();
					}
				}
				
			}else if ( percent < 0.0 ){
				percent += 1.0;
				percentVel = 0.0;
				
				if( ofGetElapsedTimef() -lastTimeChargeSoundPlayed > ofRandom(0.07, 0.5) ){
					float volPct = ofMap(fabs(myPerson.pos.y - pt.y), 70, ofGetHeight()/1.5, 1.0, 0.0, true); 
						
					if( volPct > 0.0 ){
						globalSnd.play("CHARGE_REPELLED", ofRandom(0.3, 0.7)*volPct, 1.0, pt.x<0?-0.9:0.9 );
						lastTimeChargeSoundPlayed = ofGetElapsedTimef();
					}
				}
				
			}

		}

		float drag;
		float myForce;
		float percent;
		float percentVel;
		ofPoint prePt;
		ofPoint pt;
};

class chargeAttractRod : public baseElement{
	public:
	
	chargeAttractRod();
	
	void setAngle(float ang);
	void setFlip(bool bFlip);
	void setDrag(float dragIn);
	
	void setLength(float dist);	
	void setForce(float force);
	
	ofPoint getPercentAlongRod(float pct);
	
	void updateForce();
	void update();
	void updateToTarget(float ourTargetAngle);
	void updateToHome();

	void draw();
	
	vector <ofPoint> pts;
	vector <charge> charges;
	
	float thickness;

	bool flip;
	float scale;
	float drag;
	float angleVel;
	float length;
	float angle;
	float targetAngle;
	simpleColor colorBall;
	simpleColor color;
};