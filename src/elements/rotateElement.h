/*
 *  rotateElement.h
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
#include "baseElement.h"
#include "ofMain.h"
#include "particle.h"
#include "ofxFutureUtils.h"

class rotateElement : public baseElement{
	
	public:
	
		rotateElement(){
			drag = 0.937;
			pLen = 100.0;
		}
		
		void loadImage( string imagePath, ofPoint anchorPercent ){
			img.loadImage(imagePath);
			img.setAnchorPercent(anchorPercent.x, anchorPercent.y);
		}
		
		void setProperties(float startAngle, float pivotLength){
			angle = startAngle;
			pLen  = pivotLength;
		}	
				
		void update(float _targetAngle, float force){
					
			float diff = _targetAngle - angle;	
			while(diff > 180)  diff -= 180;
			while(diff < -180) diff += 180;
			
			if( diff > 0.0 ) diff = 1.0;
			else diff = -1.0;
			
			angleVel *= drag;
			angleVel += diff * force;
					
			angle		= ofLerpDegrees(angle, angle+angleVel, 1.0);
			
		}	
		
		void draw(){
				
			ofPushMatrix();
				ofTranslate(pos.x, pos.y, pos.z);
				
				ofSetColor(255, 0, 255, 255);				
				ofCircle(0, 0, 5);

				ofSetColor(255, 255, 255, 255);				
				ofRotate(angle, 0, 0, 1);
			
				ofTranslate(0.0, pLen, 0.0);
				if( rot.x != 0.0 )ofRotate(rot.x, 1, 0, 0);
				if( rot.y != 0.0 )ofRotate(rot.y, 0, 1, 0);
				if( rot.z != 0.0 )ofRotate(rot.z, 0, 0, 1);
				img.draw(0, 0, (float)img.width * scale.x, (float)img.height * -scale.y);	
								
			ofPopMatrix();
			
		}

		float drag;
		float pLen;
		float angleVel;
		float angle;
		float targetAngle;
		ofImage img;
};