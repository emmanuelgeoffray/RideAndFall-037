/*
 *  windElement.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 06/02/2010.
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
#include "ofVec3f.h"
#include "baseElement.h"


class windElement : public baseElement{
	public:
	
		void init(ofPoint position);
		void update(float windAngle, float windForce);
		void draw();
		
		void move(ofPoint amnt);
			
		vector <ofPoint> trail; 
		vector <ofPoint> trailUp; 
		vector <ofPoint> trailDown; 

		ofPoint prevPoint;
		ofPoint vel, gra;
		float rad, drag;
		float unique;
		
		float preT;
		float amnt;
		float ourAngle;
		ofPoint interPoint;
};