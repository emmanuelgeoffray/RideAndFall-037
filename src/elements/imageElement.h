/*
 *  imageElement.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 27/01/2010.
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
#include "colorLoader.h"
#include "ofImage.h"

class imageElement : public baseElement{
	public:
	
	bool flip;
	ofImage img;
	simpleColor color;
	
	void loadImage(string path, ofPoint anchorPercent = ofPoint(0.5, 0.5), bool bFlip = true){
		img.loadImage(path);
		img.setAnchorPercent(anchorPercent.x, anchorPercent.y);
		
		flip = bFlip;
		
		if( img.type == OF_IMAGE_COLOR_ALPHA ){
			hasAlpha = true;
		}
	}
	
	void draw(){
		if( !visible ){
			//printf("warning not visible\n");
			return;
		}
		ofPushStyle();
		//ofSetColor(255, 255, 255);
		
			if( hasAlpha ){
				ofEnableAlphaBlending();
			}
			
			ofSetColor(color.r, color.g, color.b, color.a);
			
			ofPushMatrix();
				ofTranslate(pos.x, pos.y, pos.z);
				if( rot.x != 0.0 )ofRotate(rot.x, 1, 0, 0);
				if( rot.y != 0.0 )ofRotate(rot.y, 0, 1, 0);
				if( rot.z != 0.0 )ofRotate(rot.z, 0, 0, 1);
				
				if( flip ){
					img.draw(0, 0, (float)img.width * scale.x, (float)img.height * -scale.y);
				}else{
					img.draw(0, 0, (float)img.width * scale.x, (float)img.height * scale.y);				
				}
				
			ofPopMatrix();
		
		ofPopStyle();
		
	}
	
};