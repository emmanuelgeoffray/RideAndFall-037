/*
 *  windElement.cpp
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

#include "windElement.h"
#include "ofxFutureUtils.h"

void windElement::init(ofPoint position){
	pos			= position;
	interPoint  = pos;
	ourAngle	= 0;
	amnt        = 0;
	preT		= ofGetElapsedTimef();
	unique	    = ofRandom(20, 1001010);
}

void windElement::move(ofPoint amnt){
	for(int i = 0; i < trail.size(); i++){
		trail[i] += amnt;
		trailUp[i] += amnt;
		trailDown[i] += amnt;		
	}
	pos += amnt;
	interPoint += amnt;
}

void windElement::update(float windAngle, float windForce){
	
	ourAngle = ofLerpDegrees(ourAngle, windAngle, 0.15); 
	
	float xFactor = cos(ourAngle);
	float yFactor = sin(ourAngle);
	
	amnt += ofGetElapsedTimef() - preT;
	
	float dist  = 9.0 * sin( unique + amnt * ( 2.0 + ofClamp(windForce, 0, 5) ) );
	preT		= ofGetElapsedTimef();

	float perpX = cos(HALF_PI + ourAngle) * dist;
	float perpY = sin(HALF_PI + ourAngle) * dist;

	interPoint += ofPoint(xFactor, yFactor) * windForce;

	pos = interPoint + ofPoint( perpX, perpY );
	ofPoint posUp   = pos + ofPoint(-yFactor, xFactor) * 10.0;
	ofPoint posDown = pos + ofPoint(yFactor, -xFactor) * 10.0;

	trailUp.push_back(posUp);	
	trail.push_back(pos);	
	trailDown.push_back(posDown);	

	if( trail.size() > 16 ){
		trail.erase(trail.begin(), trail.begin()+1);
		trailUp.erase(trailUp.begin(), trailUp.begin()+1);
		trailDown.erase(trailDown.begin(), trailDown.begin()+1);
	}
				
}


void windElement::draw(){
	glEnable(GL_DEPTH_TEST);

	if( trail.size() > 3 ){
	
		float val = 40 + ofNoise(unique, ofGetElapsedTimef() * 0.2) * 70;
	
		ofSetColor(val, val, val, 100);
		
		glBegin(GL_LINE_STRIP);
			for(int i = 0; i < trail.size(); i++){
				glVertex3fv( trail[i].getPtr());
			}
		glEnd();

		glBegin(GL_LINE_STRIP);
			for(int i = 0; i < trailDown.size(); i++){
				glVertex3fv( trailDown[i].getPtr());
			}
		glEnd();
		
		glBegin(GL_LINE_STRIP);
			for(int i = 0; i < trailUp.size(); i++){
				glVertex3fv( trailUp[i].getPtr());
			}
		glEnd();								
		
	}
	
	glDisable(GL_DEPTH_TEST);			

}
