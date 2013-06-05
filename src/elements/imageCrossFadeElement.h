/*
 *  imageCrossFadeElement.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 17/02/2010.
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

class imageCrossFadeElement : public baseElement{
	public:
	
	
	ofImage img1;
	ofImage img2;
	
	float fade;
	
	bool flip;	
	simpleColor color;
	
	void loadImage(string path1, string path2, ofPoint anchorPercent = ofPoint(0.5, 0.5), bool bFlip = true){
		
		img1.loadImage(path1);
		img1.setAnchorPercent(anchorPercent.x, anchorPercent.y);

		img2.loadImage(path2);
		img2.setAnchorPercent(anchorPercent.x, anchorPercent.y);
		
		fade = 0.0;
		
		flip = bFlip;
		
		if( img1.type == OF_IMAGE_COLOR_ALPHA ){
			hasAlpha = true;
		}
	}
	
	void setFade(float fadePct){
		fade = ofClamp(fadePct, 0, 1);
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
			
			ofPushMatrix();
				ofTranslate(pos.x, pos.y, pos.z);
				if( rot.x != 0.0 )ofRotate(rot.x, 1, 0, 0);
				if( rot.y != 0.0 )ofRotate(rot.y, 0, 1, 0);
				if( rot.z != 0.0 )ofRotate(rot.z, 0, 0, 1);
				
				float revFade = powf((1.0-fade), 0.2);
				
				if( flip ){
					
					//if( fade < 1.0 ){
						ofSetColor(color.r, color.g, color.b, color.a*revFade);
						img1.draw(0, 0, (float)img1.width * scale.x, (float)img1.height * -scale.y);
					//}
					
					//if( fade > 0.0 ){
						ofSetColor(color.r, color.g, color.b, color.a * fade);
						img2.draw(0, 0, (float)img2.width * scale.x, (float)img2.height * -scale.y);
					//}
					
				}else{
					//if( fade < 1.0 ){
						ofSetColor(color.r, color.g, color.b, color.a*revFade);
						img1.draw(0, 0, (float)img1.width * scale.x, (float)img1.height * scale.y);	
					//}
					//}else if( fade > 0 ){
						ofSetColor(color.r, color.g, color.b, color.a * fade);					
						img2.draw(0, 0, (float)img2.width * scale.x, (float)img2.height * -scale.y);
					//}
				}
				
			ofPopMatrix();
		
		ofPopStyle();
		
	}
	
};