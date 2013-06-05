/*
 *  cloudElement.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 12/02/2010.
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
#include "imageElement.h"
#include "ofImage.h"

class cloudElement : public baseElement{
	public:
	
	bool flip;
	float fadePct;
	ofImage * img1;
	ofImage * img2;
	
	simpleColor color;
	
	cloudElement(){
		img1 = NULL;
		img2 = NULL;
		fadePct = 0.0;
	}
	
	void setImageReference(ofImage & imgRef1, ofImage & imgRef2,  bool bFlip = true){
		img1 = &imgRef1;
		img2 = &imgRef2;
				
		flip = bFlip;
		
		hasAlpha = true;
	}
	
	void draw(){
		if( !visible ){
			//printf("warning not visible\n");
			return;
		}
		if( img1 == NULL || img2 == NULL ) return;
		
		ofPushStyle();
		
			if( hasAlpha ){
				ofEnableAlphaBlending();
			}
			
			
			ofPushMatrix();
				ofTranslate(pos.x, pos.y, pos.z);
				if( rot.x != 0.0 )ofRotate(rot.x, 1, 0, 0);
				if( rot.y != 0.0 )ofRotate(rot.y, 0, 1, 0);
				if( rot.z != 0.0 )ofRotate(rot.z, 0, 0, 1);

				ofSetColor(color.r, color.g, color.b, color.a);
				
				if( flip ){
					img1->draw(0, 0, (float)img1->width * scale.x, (float)img1->height * -scale.y);
				}else{
					img1->draw(0, 0, (float)img1->width * scale.x, (float)img1->height * scale.y);				
				}
				
				if( fadePct > 0 ){
					ofSetColor(color.r, color.g, color.b, color.a * fadePct);				

					if( flip ){
						img2->draw(0, 0, (float)img2->width * scale.x, (float)img2->height * -scale.y);
					}else{
						img2->draw(0, 0, (float)img2->width * scale.x, (float)img2->height * scale.y);				
					}

				}
			ofPopMatrix();
		
		ofPopStyle();
		
	}
	
};