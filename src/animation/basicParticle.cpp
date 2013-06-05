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
 
#include "basicParticle.h"

basicParticle::basicParticle(){
	friction = 1.0;
	radius = ofRandom(8, 15);
	type = ofRandom(0,1.999);
}

void basicParticle::setFriction(float frictionAmnt){
	friction = frictionAmnt;
}

void basicParticle::setPosition(float x, float y){
	setPosition(ofVec2f(x, y));
}

void basicParticle::setPosition(ofVec2f newPos){
	pos = newPos;
}

void basicParticle::setVelocity(float x, float y){
	setVelocity(ofVec2f(x, y));
}

void basicParticle::setVelocity(ofVec2f velocity){
	vel = velocity;
}

void basicParticle::update(){
	vel *= friction;	
	pos += vel;
}

void basicParticle::addForce(float x, float y){
	addForce(ofVec2f(x, y));
}

void basicParticle::addForce(ofVec2f force){
	vel += force;
}

void basicParticle::draw(){
//	ofPushStyle();
//	if( type ){
//		ofNoFill();
//	}
	ofCircle(pos.x, pos.y, radius);
//	ofPopStyle();
}


