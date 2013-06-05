/*
 *  myPersonElement.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 07/02/2010.
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
#include "colorLoader.h"

class myPersonElement : public baseElement{

	public: 
	//------------------------------------------------------
	void setup(ofPoint position, ofPoint velocity, ofPoint gravity, float dragIn, float radius);
	void update(ofPoint gravity, float dragAmnt, float updateSpeed);
	
	void increaseScale(float amnt, float scaleDuration);
	void decreaseScale(float amnt, float scaleDuration);
	
	void drawAtScreenPos(float x, float y, float z);
	void draw();
	
	bool isBlowing();
	bool isBursting();
	
	void blowBalloon();
	void burstBalloon();

	bool flip;
	ofImage img;
	simpleColor color;		

	ofPoint shift;

	ofImage hand[2];
	ofImage device[2];
	
	int which;
	
	bool isScaling;
	float scaleTillTime;
	float maxDeviceScale;
	float minDeviceScale;
	float targetScale;
	
	float blowPercent;
	float burstPercent;
	float burstScale;
	float deviceScale;

	float rad;
	float drag;
	ofVec3f vel;
	ofVec3f gra;
	ofVec3f home;
};
