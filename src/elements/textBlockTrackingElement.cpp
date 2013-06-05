/*
 *  textBlockTrackingElement.cpp
 *  ofxFernCameraDemo
 *
 *  Created by theo on 05/02/2010.
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

#include "textBlockTrackingElement.h"

textBlockTrackingElement::textBlockTrackingElement(){
	pointerW = 20;
	pointerH = 30;
	dropShadow = false;
}

//-------------------------------------------------------
void textBlockTrackingElement::draw(){

	pointerH = height * 0.6;
	pointerW = pointerH * 0.75;
	
	float bgX = -pointerW/2;
	float bgY = -height - pointerH;
	
	float textStartX = bgX + (width-textW)/2;

	if( !visible ){
		//printf("warning not visible\n");
		return;
	}
	ofPushStyle();
	
		if( hasAlpha ){
			ofEnableAlphaBlending();
		}
		
		
		if( ttf != NULL ){
			ofPushMatrix();
				ofTranslate(pos.x, pos.y, pos.z);
				if( flip ){
					ofScale(1, -1, 1);
				}				
				if( rot.x != 0.0 )ofRotate(rot.x, 1, 0, 0);
				if( rot.y != 0.0 )ofRotate(rot.y, 0, 1, 0);
				if( rot.z != 0.0 )ofRotate(rot.z, 0, 0, 1);					
				
//				if( dropShadow ){
//					ofSetColor(color.r * 0.2, color.g * 0.2, color.b * 0.2, color.a * 0.68);
//					
//					ofPushMatrix();
//						ofTranslate(-5, 5, -5);
//						ofRect(bgX, bgY, width, height);
//						ofTriangle( 0, -pointerH, pointerW, -pointerH, 0, 0);
//					ofPopMatrix();
//				}
//				
				ofSetColor(color.r, color.g, color.b, color.a);
				ofRect(bgX, bgY, width, height);
				ofTriangle( 0, -pointerH, pointerW, -pointerH, 0, 0);
									
				ofSetColor(typeColor.r, typeColor.g, typeColor.b, typeColor.a);
				ttf->drawString(text, textStartX, -pointerH-(height-ttf->stringHeight("S"))*0.9);
									
			ofPopMatrix();
			
//FOR DEBUGGING THE POS POSITION
//			ofPushMatrix();
//				ofSetColor(255, 0, 255);
//				ofTranslate(pos.x, pos.y, pos.z);
//				ofCircle(0, 0, 5);
//			ofPopMatrix();
		}
		
	ofPopStyle();
	
}
