/*
 *  textBlockElement.h
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

#pragma once 
#include "baseElement.h"
#include "ofMain.h"
#include "colorLoader.h"

class textBlockElement : public baseElement{
	protected:
	
	bool flip;
	ofImage * background;
	ofTrueTypeFont * ttf;
	ofPoint anchor;
	string text;
	

	public:
	
	ofPoint ogPoint;
	float paddingW, paddingH;
		
	int mode;
	float width;
	float height;
	float textW;
	float textH;

	simpleColor color;
	simpleColor typeColor;
	
	textBlockElement(){
		background = NULL;
		ttf		   = NULL;
		paddingW    = 1.2;
		paddingH    = 1.4;
				
	}
	
	void setup(ofTrueTypeFont * typePtr, string textIn,  ofImage * bgImage = NULL, ofPoint anchorPercent = ofPoint(0.5, 0.5), bool bFlip = true){
		ttf			= typePtr;
		background	= bgImage;
		flip		= bFlip;
		anchor      = anchorPercent;
		
		width		= 10;
		height		= 8;
		mode		= 1;
		
		setText(textIn);
		
		if(ttf != NULL ){
			hasAlpha = true;
		}
		if( background != NULL && background->type == OF_IMAGE_COLOR_ALPHA ){
			hasAlpha = true;
		}
	}
	
	void setText(string textIn){
		text = textIn;
		if( ttf != NULL ){
			textW = ttf->stringWidth(text);
			textH = ttf->stringHeight(text);
		
			width	= textW * paddingW;
			height	= textH * paddingH;
		}
	}
	
	void setTypeColor(simpleColor tColor){
		typeColor = tColor;
	}
	
	void setBackgroundColor(simpleColor bColor){
		color = bColor;
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
			
			
			if( ttf != NULL ){
				ofPushMatrix();
					ofTranslate(pos.x, pos.y, pos.z);
					
					if( flip ){
						ofScale(1, -1, 1);
					}
					if( scale.x != 1.0 || scale.y != 1.0 || scale.z != 1.0 ){
						ofScale(scale.x, scale.y, scale.z);
					}
					
					if( rot.x != 0.0 )ofRotate(rot.x, 1, 0, 0);
					if( rot.y != 0.0 )ofRotate(rot.y, 0, 1, 0);
					if( rot.z != 0.0 )ofRotate(rot.z, 0, 0, 1);					
					
					ofSetColor(color.r, color.g, color.b, color.a);
					ofSetRectMode(OF_RECTMODE_CENTER);
					ofRect((0.5-anchor.x)*width, (0.5-anchor.y)*height, width, height);
										
					ofSetColor(typeColor.r, typeColor.g, typeColor.b, typeColor.a);
					ttf->drawString(text, (0.5-anchor.x)*width -textW/2, (0.5-anchor.y)*height + textH/2 * 0.65);
										
				ofPopMatrix();
			}
			
//			ofPushMatrix();
//				ofTranslate(pos.x, pos.y, pos.z);
//				if( rot.x != 0.0 )ofRotate(rot.x, 1, 0, 0);
//				if( rot.y != 0.0 )ofRotate(rot.y, 0, 1, 0);
//				if( rot.z != 0.0 )ofRotate(rot.z, 0, 0, 1);
//				
//				if( flip ){
//					img.draw(0, 0, (float)img.width * scale.x, (float)img.height * -scale.y);
//				}else{
//					img.draw(0, 0, (float)img.width * scale.x, (float)img.height * scale.y);				
//				}
//				
//			ofPopMatrix();
		
		ofPopStyle();
		
	}
	
};