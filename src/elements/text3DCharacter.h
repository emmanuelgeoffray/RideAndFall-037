/*
 *  text3DCharacter.h
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
#include "colorLoader.h"
#include "baseElement.h"

class text3DCharacter : public baseElement{
	
	public:
	
		text3DCharacter(){
			scale		= ofVec3f(1,1,1);
			hasAlpha	= true;
			visible		= true;
			ttf			= NULL;
			bFlip		= true;
		}
		
		void setup(ofTrueTypeFont * ttfPtr, string character){
			ttf = ttfPtr;
			setCharacter(character);
		}
		
		void setAnchorPoint(ofPoint pt){
			anchor = pt;
		}
		
		void setCharacter(string character){
			myStr = character;
			if( ttf != NULL ){
				charWidth = ttf->stringWidth(myStr);
			}
		}
		
		void setAngleOffset(float offset, float radius){
			angleOffset;
		}
		
		void update(float currentAngle, float radiusX, float radiusZ){
			pos.x = anchor.x + cos( DEG_TO_RAD * (currentAngle + angleOffset) ) * radiusX;
			pos.y = anchor.y;
			pos.z = anchor.z + sin( DEG_TO_RAD * (currentAngle + angleOffset) ) * radiusZ;
			
			rot.y = 90 - (currentAngle + angleOffset);
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

		virtual void draw(){
			if( ttf == NULL )return;
			
			ofPushMatrix();
				ofTranslate(pos.x, pos.y, pos.z);
				if( bFlip || scale.x * scale.y * scale.z != 1.0 ){
					ofScale(scale.x, scale.y * ( bFlip?-1.0:1.0 ) , scale.z);
				}
				if( rot.x != 0.0 )ofRotate(rot.x, 1, 0, 0);
				if( rot.y != 0.0 )ofRotate(rot.y, 0, 1, 0);
				if( rot.z != 0.0 )ofRotate(rot.z, 0, 0, 1);
				
				ofSetColor(color.r, color.g, color.b, color.a);
				ttf->drawString(myStr, 0, 0);
			
			ofPopMatrix();
		}
		
		string myStr;
		ofTrueTypeFont * ttf;
		float charWidth;
		float angleOffset;
		
		bool bFlip;
		
		simpleColor color;
		
		ofPoint anchor;
};
