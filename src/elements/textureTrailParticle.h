/*
 *  textureTrailParticle.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 01/02/2010.
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

class texSpineJoint{
	public:
		ofVec3f pos;
		ofVec3f norm;
		float pct;
};

class textureTrailParticle : public baseElement{
	public:
	
		textureTrailParticle();
		void init(ofPoint position, float size, ofPoint velocity = 0, ofPoint gravity = 0);		
		void update(float forceMag, float timeFactor, ofPoint attractor, float angleRange);
		void updateTrail(ofPoint & prevPoint, ofPoint currentPoint, int numPer, int maxNum);
		void draw();

		vector <texSpineJoint> trail;
		
		int maxN;
		
		simpleColor color;
		ofImage * imgPtr;
		ofPoint startPoint;
		ofPoint target;
		ofPoint homePoint;
		ofPoint prevPoint;
		ofPoint shift;
		ofPoint vel, gra;
		float rad, drag;
		float unique;
};