/*
 *  particle.h
 *  openFrameworks
 *
 *  Created by theo on 9/16/09.
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

class particle{
	public: 
	virtual void setup(ofPoint position, ofPoint velocity, ofPoint gravity, float dragIn, float radius);
	virtual void setStyle( ofStyle style );
	virtual void update();
	virtual void draw();
	
	ofStyle myStyle;

	float rad;
	float drag;
	ofVec3f pos;
	ofVec3f vel;
	ofVec3f gra;
	
	bool bFront;

};
