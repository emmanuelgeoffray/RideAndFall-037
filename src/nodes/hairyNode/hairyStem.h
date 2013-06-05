/*
 *  hairyStem.h
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
#include "_projectGlobals.h"
#include "ofxChipmunk.h"

class hairyStem{
	public:

	~hairyStem();
	
	void init(int num, ofxChipmunk & world, float startX, float startY, float endX, float endY);
	void addPins(ofxChipmunk & world);
	void updatePoints();
	void draw();
	
	float unique;	
	ofPoint tip;
	ofPoint preTip;
	ofPoint tipVel;
	float tipSize;
	float blockLen;
	vector <ofxChipmunkRect> circle;
	vector <ofPoint> pointCache;
	vector <float> rotation;
	ofPoint attachPoint;
};
