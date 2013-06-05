/*
 *  homeParticle.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 13/01/2010.
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

#include "ofVec3f.h"
#include "ofMain.h"
#include "particle.h"

class homeParticle : public particle{

	public: 
	//------------------------------------------------------
	void setup(ofPoint position, ofPoint velocity, ofPoint gravity, float dragIn, float radius){
		pos		= position;
		vel		= velocity;
		gra		= gravity;
		drag	= dragIn;
		rad		= radius;
		home = pos;
	}
	
	//------------------------------------------------------
	void update(float attactionForce){
		ofVec3f delta = home - pos;
		
		vel += delta * attactionForce;
	
		vel *= drag;
		vel += gra;
		pos += vel;
	}


	ofVec3f home;
};