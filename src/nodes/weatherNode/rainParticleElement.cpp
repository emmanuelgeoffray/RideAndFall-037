/*
 *  rainParticleElement.cpp
 *  ofxFernCameraDemo
 *
 *  Created by theo on 12/02/2010.
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

#include "rainParticleElement.h"

rainParticleElement::rainParticleElement(){
	distTravelled = 0;
	imageState = 0;
}

void rainParticleElement::reset(){
	distTravelled = 0;
	vel = ofVec3f(0,0,0);
}

void rainParticleElement::setVelocity(float x, float y, float z){
	vel.set(x, y, z);
	setRotation(0, 0, 90 + (RAD_TO_DEG*atan2f(vel.y, vel.x)));
}

void rainParticleElement::update(){
	if( !visible )return;
		
	pos += vel;
	distTravelled += vel.length();
	
}
