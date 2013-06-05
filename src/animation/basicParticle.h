/*
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
 */

#pragma once

#include "ofMain.h"


class basicParticle{
	
	public:
		
	basicParticle();
	void setFriction(float frictionAmnt);

	void setVelocity(float x, float y);
	void setVelocity(ofVec2f velocity);

	void setPosition(float x, float y);
	void setPosition(ofVec2f newPos);
	
	void update();
	
	void addForce(float x, float y);
	void addForce(ofVec2f force);
	
	void draw();
	
	int type;
	
	ofVec2f pos;
	ofVec2f  vel;
		
	float friction;
	float radius;
};







	
