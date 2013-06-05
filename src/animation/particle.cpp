/*
 *  particle.cpp
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

#include "particle.h"

//------------------------------------------------------
void particle::setup(ofPoint position, ofPoint velocity, ofPoint gravity, float dragIn, float radius){
	pos		= position;
	vel		= velocity;
	gra		= gravity;
	drag	= dragIn;
	rad		= radius;

}


//------------------------------------------------------
void particle::setStyle( ofStyle style ){
	myStyle = style;
}


//------------------------------------------------------
void particle::update(){
	vel *= drag;
	vel += gra;
	pos += vel;
}


//------------------------------------------------------
void particle::draw(){
	ofSetStyle(myStyle);
	
	ofPushMatrix();
		ofTranslate(pos.x, pos.y, pos.z);
		
		ofNoFill();
		ofCircle(0, 0, rad * 0.5);		
		ofFill();
		ofCircle(0, 0, rad * 0.5);		
		
		ofSetColor(myStyle.color.r, myStyle.color.g, myStyle.color.b, myStyle.color.a * 0.4); 
		ofCircle(0, 0, rad);
	
	ofPopMatrix();
}