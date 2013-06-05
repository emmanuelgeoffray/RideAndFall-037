/*
 *  srcDstTrailParticle.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 29/01/2010.
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
#include "baseTrailParticle.h"

class srcDstTrailParticle : public baseTrailParticle{
	public:
	
	
		//void init(ofPoint startPt, ofPoint endPt, float size, ofPoint velocity = 0, ofPoint gravity = 0){
		void init(ofPoint startPt, ofPoint endPt, float size, ofPoint velocity, ofPoint gravity){
			baseTrailParticle::init(startPt, size, velocity, gravity);
			homePoint = endPt;
		}
		
		void update(float forceMag, float timeFactor = 0.1, float attractPct = 0.3 ){
			baseTrailParticle::update(forceMag, timeFactor, homePoint, attractPct);
		}

};
