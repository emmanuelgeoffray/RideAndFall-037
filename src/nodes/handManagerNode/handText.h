/*
 *  handText.h
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

#pragma once
#include "ofMain.h"
#include "_projectGlobals.h"

class handText{
	public:
		
	handText();
	void loadImage(string path, ofPoint anchorPercent, ofPoint rectPercent, bool bRightText = false);
	void setColor( simpleColor color );
	void setTypeFace(ofTrueTypeFont * ttfPtr);
	void setPosition(float x, float y, float z);
	void setText(string textIn);
	void draw();
	
	void setScale(float x, float y, float z){
		scale.x = x;
		scale.y = y;
		scale.z = z;
	}
	
	ofTrueTypeFont * ttf;
	simpleColor textColor;

	bool bAttachRight;

	ofPoint anchor;
	ofPoint rectAnchor;
	ofImage handTest;
	
	ofPoint scale;
	ofPoint pos;
	string currentText;
};