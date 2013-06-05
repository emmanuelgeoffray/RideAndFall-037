/*
 *  baseElement.h
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
#include "ofMain.h"

class baseElement{
	
	public:
	
		baseElement(){
			scale = ofVec3f(1,1,1);
			hasAlpha = false;
			visible = true;
			
			//these don't have to be set
			//but it helps us know how much space to make account for.
			//should be relative to position ( pos )
			minBoundsY	= 0.0;
			maxBoundsY	= 0.0;
			minBoundsX  = 0.0;
			maxBoundsX  = 0.0;		
		}
		
		virtual vector <baseElement *> getElements(){
			vector <baseElement *> elements;
			elements.push_back(this);
			return elements;
		} 
		
		virtual void setPosition(float x, float y, float z){
			pos.x = x;
			pos.y = y;
			pos.z = z;
		}

		virtual void setRotation(float x, float y, float z){
			rot.x = x;
			rot.y = y;
			rot.z = z;
		}	

		virtual void setScale(float x, float y, float z){
			scale.x = x;
			scale.y = y;
			scale.z = z;
		}	
				
		virtual void update(){
		
		}	
		
		virtual void draw(){
		
		}
		
		float minBoundsY;
		float maxBoundsY;
		float minBoundsX;
		float maxBoundsX;		
		
		bool visible;
		bool hasAlpha;
		
		ofPoint pos;
		ofPoint rot;
		ofPoint scale;
};
