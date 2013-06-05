/*
 *  angleLine.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 11/02/2010.
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
#include "ofxFutureUtils.h"

class angleLine{
	public:
	
		angleLine(){
			rootPtr = NULL;
			active = false;
			length = 0.0;
			angle  = 0.0;
			angleRange = 0.0;
			angleShift = 0.0;
			angleVel = 0.0;
			angleDrag = 0.95;
			uniqueVal = -1.0;
			scale = 0.0;
			targetSpeed = 0.0;
			targetLength = 0.0;
			targetDrag = 0.0;
			bFirst    = false;
		}	
		
		void setFirst(){
			bFirst = true;
		}
		
		void init(){
			uniqueVal = ofRandom(0, 10000);
			targetDrag = ofRandom(0.90, 0.93);
			angleDrag = ofRandom(0.93, 0.96);
		}
		
		void setAngleRange(float angleDeg){
			angleRange = DEG_TO_RAD * angleDeg;
		}
		
		void setRoot( ofPoint * ptr){
			rootPtr = ptr;
		}
		
		void setRoot( ofPoint pt ){
			root = pt;
		}
		
		void setAngle( float angIn ){
			angle = DEG_TO_RAD * angIn;
			targetAngle = angle;
		}

		void setTargetAngle( float angIn ){
			targetAngle = DEG_TO_RAD * angIn;
		}

		void setLength( float lenIn ){
			targetLength = lenIn;
		}
		
		float getAngle(){
			return angle * RAD_TO_DEG;
		}
		
		ofPoint getTip(){
			return tip;
		}	
		
		void restoreAngle(){
			angle = ogAngle;
		}

		void restoreAngleToTarget(){
			targetAngle = ogAngle;
		}
		
		void setAndStoreAngle( float angIn ){
			ogAngle = DEG_TO_RAD * angIn;
			angle   = DEG_TO_RAD * angIn;
			targetAngle = angle;
		}
			
		void update(float rate = 1.0){
			if( uniqueVal == -1.0 ){
				uniqueVal = ofRandom(0, 10000);
			}
			
			if( rootPtr != NULL ){
				root.x = rootPtr->x;
				root.y = rootPtr->y;
				root.z = rootPtr->z;
			}
			
			angleVel *= angleDrag;
			angleVel += ofAngleDifferenceRadians(angle, targetAngle) * 0.015;
			angle += angleVel;
			
			if( angleRange > 0.0 ){
				angleShift = angleRange * ofSignedNoise(ofGetElapsedTimef() * 0.1 * rate, uniqueVal);
			}
			
			targetSpeed *= targetDrag;
			targetSpeed += (scale*targetLength-length)*0.02;
			
			length += targetSpeed;
			
			tip.x = root.x + cos(angle+angleShift) * length;
			tip.y = root.y + sin(angle+angleShift) * length;	
			tip.z = root.z;		
			
			if( length != 0.0 ){
				active = true;
			}else{
				active = false;
			}
		}
		
		float targetLength;
		float ogAngle;
		float angleVel;
		float angleDrag;
		float targetAngle;
		float uniqueVal;
		float angleRange;
		float targetSpeed;
		float targetDrag;
		
		float angleShift;
		bool active;
		float angle;
		ofPoint root;
		ofPoint tip;
		
		float scale;
		
		bool bFirst;
		
		protected:
		float length;

		ofPoint * rootPtr;
};

