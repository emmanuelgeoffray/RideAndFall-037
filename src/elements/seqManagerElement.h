/*
 *  seqManagerElement.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 29/01/2010.
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

#include "seqManager.h"
#include "advImageSequence.h"

class seqManagerElement : public seqManager, public baseElement{
	public:
	
	simpleColor color;
	
	void loadSequence(string path, ofPoint anchorPercent = ofPoint(0.5, 0.5), bool bFlip = true){

		imgSeq.loadSeq(path, "png");
		seqManager::setup(&imgSeq, 30.0);
		seqManager::setAnchorPercent(anchorPercent.x, anchorPercent.y);
		
		flip    = bFlip;
		hasAlpha =	true;
	}
	
	void loadSequence(advImageSequence * seqPtr, ofPoint anchorPercent = ofPoint(0.5, 0.5), bool bFlip = true){

		seqManager::setup(seqPtr, 30.0);
		seqManager::setAnchorPercent(anchorPercent.x, anchorPercent.y);
		
		flip    = bFlip;
		hasAlpha =	true;
	}
	

	void update(){
		seqManager::update();
	}
	
	void show(){
		visible = true;
	}
	
	void hide(){
		visible = false;
	}
	
	void draw(){
		if( !visible ){
			//printf("warning not visible\n");
			return;
		}
		ofPushStyle();
		
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
					seqManager::draw(0, 0, (float)seqManager::width * scale.x, (float)seqManager::height * -scale.y);
				}else{
					seqManager::draw(0, 0, (float)seqManager::width * scale.x, (float)seqManager::height * scale.y);				
				}
				
			ofPopMatrix();
		
		ofPopStyle();
		
	}
	
	protected:
		advImageSequence imgSeq;
		bool flip;
};