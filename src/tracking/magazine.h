/*
 *  magazine.h
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
#include "ofMain.h"
#include "ofxFutureUtils.h"


class magazine{
	public:
		magazine(){
			angleX			= 0;
			angleY			= 0;
			angleZ			= 0;
			area			= 0;
			lastFrameSeen	= 0;
			corners.assign(4, ofPoint());
			center = ofVec3f(0.0,0,0);
			trackAreaW		= 640.0;
			trackAreaH		= 480.0;			
		}
		
		void setup(int w, int h){
			trackAreaW = w;
			trackAreaH = h;
		}
		
		void smoothIntoMe( magazine target, float amnt ){
			angleX = ofLerpDegrees(angleX, target.angleX, amnt);
			angleY = ofLerpDegrees(angleY, target.angleY, amnt); 
			angleZ = ofLerpDegrees(angleZ, target.angleZ, amnt); 
			
			while(angleZ > 360.0){
				angleZ -= 360.0;
			}
			while(angleZ < 0){
				angleZ += 360.0;
			}
			
			for(int i = 0; i < corners.size(); i++){
				corners[i] = corners[i]*(1.0-amnt) + target.corners[i]*amnt;
			}
			
			center = center*(1.0-amnt) + target.center*amnt;
			area   = area*(1.0-amnt)   + target.area*amnt;
			area   = area*(1.0-amnt)   + target.area*amnt;
			
			trackAreaW = target.trackAreaW;
			trackAreaH = target.trackAreaH;			
			lastFrameSeen = target.lastFrameSeen;
		}

		void updateFromCorners(vector <ofPoint> newCorners){
			
			//NOTE: Here we should check if the shape is parralel 
			// ie if the shape is not parrallel we don't update!
			
			ofVec2f long1 = newCorners[3] - newCorners[0];
			ofVec2f long2 = newCorners[2] - newCorners[1];

			ofVec2f short1 = newCorners[1]-newCorners[0];
			ofVec2f short2 = newCorners[2]-newCorners[3];

			bool alignedLong  = long1.align(long2, 30);
			bool alignedShort = short1.align(short2, 30);
			
			float angleBetween = long1.angle(short1);
			
			//printf("angleBetween is %f\n", angleBetween);
	
			if( alignedLong && alignedShort && fabs(angleBetween) > 60 && fabs(angleBetween) <= 120 ){
				corners = newCorners;			
			}
			
			area = polygonArea(corners);
			
			center = ofVec3f(0.0,0,0);
			if( area > 0.0 ){	
				for(int i = 0; i < corners.size(); i++){
					center.x += corners[i].x;
					center.y += corners[i].y;				
				}
				center.x /= 4.0;
				center.y /= 4.0;
			}
						
			ofVec2f p2 = corners[1];
			ofVec2f p1 = corners[0];
			ofVec2f diff = p2 - p1;
			
			angleZ = atan2(diff.y, diff.x) * RAD_TO_DEG;
			
			while(angleZ > 360.0){
				angleZ -= 360.0;
			}
			while(angleZ < 0){
				angleZ += 360.0;
			}
			
			float sideL		= ofVec3f(corners[0]-corners[3]).length();
			float sideR		= ofVec3f(corners[1]-corners[2]).length();
			float shortT	= ofVec3f(corners[0]-corners[1]).length();
			float shortB	= ofVec3f(corners[2]-corners[3]).length();
		
			if( sideR != 0.0 ){
				angleY = ofMap( sideL / sideR, 0.8, 1.25, -50, 50, true);
			}
			
			if( shortT != 0.0 ){
				angleX = ofMap( shortB / shortT, 0.8, 1.25, -50, 50, true);
			}		
			
			lastFrameSeen = ofGetFrameNum();
		}
		
		void debugDraw(float x, float y, float w, float h){
			if( trackAreaW == 0.0 ){
				trackAreaW = 640.0;
				trackAreaH = 480.0;
			}	
		
			ofPushMatrix();
				ofTranslate(x, y, 0);
				
				ofScale(w/trackAreaW, h/trackAreaH);
				ofPushStyle();
					
					ofSetColor(22, 22, 66);
					
					ofDrawBitmapString("angleX = "+ofToString(angleX, 0), 10, 20);
					ofDrawBitmapString("angleY = "+ofToString(angleY, 0), 10, 38);
					ofDrawBitmapString("angleZ = "+ofToString(angleZ, 0), 10, 56);
					ofDrawBitmapString("area   = "+ofToString(area, 0), 10, 74);
					ofDrawBitmapString("frame  = "+ofToString(int(ofGetFrameNum()-lastFrameSeen)), 10, 92);
					
					ofNoFill();
					ofCircle(center.x, center.y, 5);
					
					ofBeginShape();
						for(int i = 0; i < corners.size(); i++){
							ofVertex(corners[i].x, corners[i].y);
							ofDrawBitmapString(ofToString(i), corners[i].x, corners[i].y);
						}
					ofEndShape(true);
					
				ofPopStyle();
			ofPopMatrix();
		}
				
		unsigned long lastFrameSeen;
		
		float angleX;
		float angleY;
		float angleZ;
		
		float trackAreaW;
		float trackAreaH;
		
		float area;

		ofPoint center;
		vector <ofPoint> corners;
		
};
