/*
 *  textFlipElement.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 16/02/2010.
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
#include "textBlockElement.h"

class textFlipElement : public textBlockElement{
	
	public: 
	float flipPct;
	bool bFlipping;
	string defferedText;
	int flipSide;
	
	textFlipElement(){
		flipPct = 0.0;
		bFlipping = false;
		flipSide = 0;
	}
	
	void setTextFlip(string textIn){
		defferedText = textIn;
		bFlipping	= true;
		flipPct		= 0.0;
	}
	
	bool isFlipping(){
		return bFlipping;
	}
	
	void update(float rate){
		if( bFlipping ){
		
			flipPct += rate;
			
			if( flipPct < 0.5 && flipPct+rate >= 0.5 ){
				setText(defferedText);
				simpleColor tmp = color;
				color = typeColor;
				typeColor = tmp;
			}
			
			if( flipPct <= 0.5 ){
				rot.x = ofMap(flipPct, 0, 0.5, 0, 90, true);
			}else{
				rot.x = ofMap(flipPct, 0.5, 1, 270, 360, true);
			}
			
			if( flipPct >= 1.0 ){
				flipPct = 1.0;
				bFlipping = false;
				rot.x = 0;
				if( flipSide == 0 ){
					flipSide = 1;
				}else{
					flipSide = 0;
				}
			}
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
						ofRect(0, 0, width, height);
																							
						ofSetColor(typeColor.r, typeColor.g, typeColor.b, typeColor.a);
						ttf->drawString(text, -textW/2, textH/2 * 0.7);
			
				ofPopMatrix();
				
			}
		
		ofPopStyle();
		
	}
	
};