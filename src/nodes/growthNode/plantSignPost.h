/*
 *  plantSignPost.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 18/02/2010.
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
#include "leafElement.h"
#include "angleLine.h"
#include "ofMain.h"
#include "_projectGlobals.h"
#include "ofxFutureUtils.h"

#include "textBlockElement.h"

class plantSignPost : public angleLine{
	public:
	
	plantSignPost(){
		ttfPtr = NULL;
	}
	
	void setup(ofTrueTypeFont * ttf, ofPoint anchorPt, float baseAngle, float lineLength, ofPoint attachPercent){
		ttfPtr = ttf;

		setRoot(anchorPt);
		setAngle(baseAngle);
		setLength(lineLength);
		setAngleRange(35.0);
		
		scale = 1.0;
	
		textBlock.setup(ttf, "SOMETHING", NULL, attachPercent, true);
	}
	
	void setText(string text){
		textBlock.setText(text);
	}
	
	void setBackgroundColor(simpleColor color){
		textBlock.setBackgroundColor(color);
	}
	
	void setTextColor(simpleColor color){
		textBlock.setTypeColor(color);
	}
	
	void update(float rate, float scaleAmnt){
		scale = scaleAmnt;
		
		angleLine::update(rate);
		
		//printf("angle is %f tip is %f %f %f\n", angle, tip.x, tip.y, tip.z);
		
		ofPoint tipPt = getTip();
		textBlock.setPosition(tipPt.x, tipPt.y, tipPt.z);		
	}
	
	
	void draw(){
		ofPushStyle();
			ofSetLineWidth(3.0);			
			globalColors.setColor("darkFall");
			glBegin(GL_LINES);
				glVertex3fv(root.getPtr());
				glVertex3fv(tip.getPtr());												
			glEnd();
			
			textBlock.setScale(scale, scale, 1.0);
			textBlock.draw();
		ofPopStyle();
	}
	
	textBlockElement textBlock;	
	ofTrueTypeFont * ttfPtr;
	
};	
