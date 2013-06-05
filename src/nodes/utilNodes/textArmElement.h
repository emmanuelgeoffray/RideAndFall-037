/*
 *  textArmElement.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 05/02/2010.
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
#include "textBlockElement.h"
#include "angleLine.h"

typedef enum{
	TEXT_ARM_HIDDEN,
	TEXT_ARM_IN,
	TEXT_ARM_SHOWING,
	TEXT_ARM_OUT
}textArmState;

class textArmElement : public baseElement{
	public:
	
	textArmElement();
	
	//---------------------------------------------------------------------------------------------------
	void setupType(ofTrueTypeFont * ttfPtr, simpleColor backColor, simpleColor textColor, bool flip, ofPoint anchor);
	void setupArm(float startAngle, float angleRange, float length, float textAngleOffset, simpleColor lineColorIn);
	
	void show(string text, float durationToShow);
	void hide();
	bool isBusy();
	
	textArmState getState();
	
	void update(float rateIn);
	void draw();
	
	textArmState state;
	
	simpleColor lineColor, bgColor, typeColor;
	textBlockElement textBlock;
	
	float duration;
	float showTill;
	
	float textAngle;
	float pct;
	
	ofTrueTypeFont * ttf;
	vector <angleLine> arm;
};