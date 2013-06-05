/*
 *  camManager.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 31/01/2010.
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

class camManager{
	public:	
	
		camManager(){
			bLimitPosMax = false;
			bLimitPosMin = false;
			bLimitRotMax = false;
			bLimitRotMin = false;	
		}
	
		void setPosLimitMax(float maxX, float maxY, float maxZ){
			bLimitPosMax = true;
			maxPos.set(maxX, maxY, maxZ);
		}

		void setPosLimitMin(float minX, float minY, float minZ){
			bLimitPosMin = true;
			minPos.set(minX, minY, minZ);
		}

		void setRotLimitMax(float maxX, float maxY, float maxZ){
			bLimitRotMax = true;
			maxRot.set(maxX, maxY, maxZ);
		}

		void setRotLimitMin(float minX, float minY, float minZ){
			bLimitRotMin = true;
			minRot.set(minX, minY, minZ);
		}

		void moveX(float amnt){
			targetPos.x += amnt;
			checkLimits();
		}

		void moveY(float amnt){
			targetPos.y += amnt;
			checkLimits();
		}

		void moveZ(float amnt){
			targetPos.z += amnt;
			checkLimits();
		}

		void rotateX(float amnt){
			targetRot.x += amnt;
			checkLimits();
		}

		void rotateY(float amnt){
			targetRot.y += amnt;
			checkLimits();
		}

		void rotateZ(float amnt){
			targetRot.z += amnt;
			checkLimits();
		}

		void setPos(ofPoint t){
			targetPos = t;
			checkLimits();
			camPos = targetPos;
		}
		
		void setPosX(float x){
			targetPos.x = x;
			checkLimits();
			camPos.x = targetPos.x;			
		}

		void setPosY(float y){
			targetPos.y = y;
			checkLimits();
			camPos.y = targetPos.y;				
		}		
		
		void setPosZ(float z){
			targetPos.z = z;
			checkLimits();
			camPos.z = targetPos.z;				
		}
	
		void setTargetPos(ofPoint t){
			targetPos = t;
			checkLimits();
		}
		
		void setTargetPosX(float x){
			targetPos.x = x;
			checkLimits();
		}

		void setTargetPosY(float y){
			targetPos.y = y;
			checkLimits();
		}		
		
		void setTargetPosZ(float z){
			targetPos.z = z;
			checkLimits();
		}
		
		void setTargetRot(ofPoint r){
			targetRot = r;
			checkLimits();
		}
		
		void setTargetRotX(float x){
			targetRot.x = x;
			checkLimits();
		}

		void setTargetRotY(float y){
			targetRot.y = y;
			checkLimits();
		}		
		
		void setTargetRotZ(float z){
			targetRot.z = z;
			checkLimits();
		}
		
		void checkLimits(){
			if( bLimitPosMax ){
				if(targetPos.x > maxPos.x)targetPos.x = maxPos.x;
				if(targetPos.y > maxPos.y)targetPos.y = maxPos.y;
				if(targetPos.z > maxPos.z)targetPos.z = maxPos.z;
			}
			if( bLimitPosMin ){
				if(targetPos.x < minPos.x)targetPos.x = minPos.x;
				if(targetPos.y < minPos.y)targetPos.y = minPos.y;
				if(targetPos.z < minPos.z)targetPos.z = minPos.z;
			}
			if( bLimitRotMax ){
				if(targetRot.x > maxRot.x)targetRot.x = maxRot.x;
				if(targetRot.y > maxRot.y)targetRot.y = maxRot.y;
				if(targetRot.z > maxRot.z)targetRot.z = maxRot.z;
			}
			if( bLimitRotMin ){
				if(targetRot.x < minRot.x)targetRot.x = minRot.x;
				if(targetRot.y < minRot.y)targetRot.y = minRot.y;
				if(targetRot.z < minRot.z)targetRot.z = minRot.z;
			}			
		}
		
		void update(float tweenPct = 0.2, float tweenPctAng = 0.1){
			camPos *= (1.0-tweenPct);
			camPos += targetPos * tweenPct;
			
			camRot.x = ofLerpDegrees(camRot.x, targetRot.x, tweenPctAng);
			camRot.y = ofLerpDegrees(camRot.y, targetRot.y, tweenPctAng);
			camRot.z = ofLerpDegrees(camRot.z, targetRot.z, tweenPctAng);
		}
		
		ofPoint getPosition(){
			return camPos;
		}

		ofPoint getRotation(){
			return camRot;
		}
		
		ofPoint maxPos;
		ofPoint minPos;
		ofPoint maxRot;
		ofPoint minRot;
		
		bool bLimitPosMax;
		bool bLimitPosMin;
		bool bLimitRotMax;
		bool bLimitRotMin;		
		
		ofPoint camPos;
		ofPoint camRot;
		
		ofPoint targetPos;
		ofPoint targetRot;
};