/*
 *  handTextManager.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 19/02/2010.
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
#include "handText.h"
#include "colorLoader.h"

typedef enum{
	TEXT_TRIGGER_HIDDEN,
	TEXT_TRIGGER_HIDDING,
	TEXT_TRIGGER_POS,
	TEXT_TRIGGER_TIME
}textTriggerState;

class handTextManager{
	public:
	
	void setup(ofPoint position, ofTrueTypeFont * ttfPtr, simpleColor typeColor, string image, ofPoint anchor, ofPoint textBoxAnchor, bool bLeftReveal);		
	bool isTextBusy();
	void triggerSideText(string text, float duration);
	void triggerSideText(string text, float minY, float maxY);
	
	void setHidePosX(float newHidePos){
		hidePosX = newHidePos;
	}

	//TODO: this doesn't work yet. 
//	void setPosY(float yPos){
//		leftHand.pos.y = yPos;
//	}
	
	void update();
	void updateCamToScreenCoords();
	void draw();

	textTriggerState leftState;
	float leftTriggerTime;
	float minTriggerY, maxTriggerY;

	float hidePosX;
	float shiftPosX;
	
	bool bLeft;

	handText leftHand;
	
	ofPoint pos;
};