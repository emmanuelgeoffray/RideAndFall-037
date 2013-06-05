/*
 *  actionElement.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 13/01/2010.
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
#include "particle.h"

#include "colorLoader.h"
extern 	colorGroup globalColors;


class actionElement : public baseElement{
	
	public:
	
		actionElement(){

		}
		
		void loadImage( string imagePath, ofPoint anchorPercent ){
			img.loadImage(imagePath);
			img.setAnchorPercent(anchorPercent.x, anchorPercent.y);
		}
		
		static bool radIsZero(particle & A){
			return A.rad == 0.0;
		}
		
		void update(){
		
			for(int i = 0; i < tmpPs.size(); i++){
				tmpPs[i].update();
				
				ofVec3f vel = tmpPs[i].vel;
				float mag = vel.length();
				
				float angle = atan2(vel.y, vel.x);
				angle += HALF_PI * 0.15 * ofSignedNoise(i, ofGetElapsedTimef() * 0.1);
				
				ofVec3f newAngle(cos(angle), sin(angle));
				newAngle *= mag;
				
				tmpPs[i].vel = newAngle;
				tmpPs[i].rad *= tmpPs[i].drag;
				
				if( mag <= 0.6 ){
					tmpPs[i].vel = ofVec3f(0,0,0);
					tmpPs[i].rad = 0.0;
				}
				
			}
			
			tmpPs.erase(tmpPs.begin(), partition(tmpPs.begin(), tmpPs.end(), actionElement::radIsZero));
		}	
		
		void triggerParticles(){
			if( tmpPs.size() == 0 ){
				
				printf("------------- MAKING PARTICLES!\n");
				
				for(int i = 0; i < 100; i++){
					particle p;
					
					float velX = 0.5 + ofSignedNoise(i * 10, 0.3);
					float velY = 0.5 + ofNoise(5 * i, 0.999);
					float velZ = 0;
					
					p.setup(pos + ofPoint(0, 150, -50), ofPoint(velX, velY, velZ) * 16.0, ofPoint(), 0.957, ofRandom(5, 30));
					tmpPs.push_back(p);
				}
			}
		}
	
		void draw(){
			ofPushStyle();

				ofEnableAlphaBlending();
				simpleColor tmp = globalColors.getColor("red");

				ofPushStyle();
					for(int i = 0; i < tmpPs.size(); i++){
						ofPushMatrix();
							ofTranslate(tmpPs[i].pos.x, tmpPs[i].pos.y, tmpPs[i].pos.z);
							
							ofSetColor(0, 0, 0);
							ofCircle(0, 0, tmpPs[i].rad);
							
							ofTranslate(0, 0, 2);
							
							ofSetColor(tmp.r, tmp.g, tmp.b);						
							ofCircle(0, 0, tmpPs[i].rad * 0.8);

						ofPopMatrix();
					}
				ofPopStyle();
			
				ofSetColor(255, 255, 255, 255);
				
				ofPushMatrix();
					ofTranslate(pos.x, pos.y, pos.z);
					if( rot.x != 0.0 )ofRotate(rot.x, 1, 0, 0);
					if( rot.y != 0.0 )ofRotate(rot.y, 0, 1, 0);
					if( rot.z != 0.0 )ofRotate(rot.z, 0, 0, 1);
					
					img.draw(0, 0, (float)img.width * scale.x, (float)img.height * scale.y);					
					
				ofPopMatrix();
				
			ofPopStyle();
		}
		
		ofImage img;		
		vector <particle> tmpPs;
};
