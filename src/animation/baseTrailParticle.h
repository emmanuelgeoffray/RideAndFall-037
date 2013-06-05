/*
 *  baseTrailParticle.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 27/12/2009.
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
#include "baseElement.h"

class spineJoint{
	public:
		ofPoint pos;
		ofPoint norm;
		float pct;
};

class baseTrailParticle : public baseElement{
	public:
	
		void init(ofPoint position, float size, ofPoint velocity = 0, ofPoint gravity = 0){
			pos		= position;
			rad		 = size;
			vel		 = velocity;
			gra		 = gravity;
			drag	 = 0.978;
			unique   = ofRandomuf();
			prevPoint = pos;
			startPoint = pos;
			trail.clear();
		}
		
		void update(float forceMag, float timeFactor = 0.1, ofPoint attractor = 0, float attractPct = 0.0){
			vel *= drag;
			
			float xforce = ofSignedNoise((float)ofGetFrameNum() * timeFactor, 1000, unique * 100.0);
			float yforce = ofSignedNoise((float)ofGetFrameNum() * timeFactor, -1000, unique * 100.0);
			float zforce = ofSignedNoise((float)ofGetFrameNum() * timeFactor, 33, unique * 100.0);
			
			ofVec3f force(xforce, yforce, zforce);
			force.normalize();
			
			if( attractPct > 0.0 ){
				ofVec3f vec = attractor - pos;
				vec.normalize();
				
				force *= (1.0 - attractPct);
				force += vec * attractPct;
			}
			
			vel += force * forceMag;
			pos += vel;			
	
			ofVec3f dist = pos - prevPoint;
			
			//if( dist.lengthSquared() >= 1.0 ){
				updateTrail(prevPoint, pos+vel, 1, 20);	
				prevPoint = pos;			
			//}
		
		}
		
		void updateTrail(ofPoint & prevPoint, ofPoint currentPoint, int numPer, int maxNum){
		
			for(int i = 0; i < numPer; i++){
				float pct = (float)i/numPer;
				spineJoint jt;
				jt.pos = currentPoint*pct + prevPoint*(1.0-pct);
				trail.push_back(jt);
			}
			
			if( trail.size() > maxNum ){
				trail.erase(trail.begin(), trail.begin()+(trail.size()-maxNum));
			}
		
			if( trail.size() > 3 ){
						
				ofVec3f delta;
				ofVec3f norm;
								
				for(int i = 0; i < trail.size(); i++){
					
					if( i == 0 ){
						delta = trail[1].pos - trail[0].pos;
					}else if( i == trail.size()-1 ){
						delta = trail[trail.size()-1].pos - trail[trail.size()-2].pos;
					}
					else{
						delta = trail[i+1].pos - trail[i-1].pos;
					}
						
					trail[i].pct =  (float)i/trail.size();
					delta.normalize();

					trail[i].norm.set(  -delta.y,  delta.z, -delta.y );
				}
			}
			
			
			
		}
		
		void draw(){
			glEnable(GL_DEPTH_TEST);
		
			if( trail.size() > 3 ){
			
				ofPushMatrix();
				
				ofSetColor(255, 0, 255, 255);
				
				glBegin(GL_TRIANGLE_STRIP);
					for(int i = 0; i < trail.size(); i++){
						glVertex3fv( (trail[i].pos - trail[i].norm * 5.0).getPtr());
						glVertex3fv( (trail[i].pos + trail[i].norm * 5.0).getPtr());
					}
				glEnd();
				
//FOR CROSS LIKE SHAPE
//				ofPoint perp;							
//				ofSetColor(170, 0, 200, 255);
//				
//				glBegin(GL_TRIANGLE_STRIP);
//					for(int i = 0; i < trail.size(); i++){
//					
//						perp.set(trail[i].norm.z, trail[i].norm.x, -trail[i].norm.y);
//						
//						glVertex3fv( (trail[i].pos + perp*5.0).v);
//						glVertex3fv( (trail[i].pos - perp*5.0).v);
//					}
//				glEnd();
	
//FOR FAKE TWO SIDED
//				ofSetColor(20, 100, 230, 255);
//				
//				ofPoint shift;
//				glBegin(GL_TRIANGLE_STRIP);
//					for(int i = 0; i < trail.size(); i++){
//						shift.set( -trail[i].norm.z, -trail[i].norm.x, trail[i].norm.y );
//						glVertex3fv( (trail[i].pos + shift + trail[i].norm * 5.0).v);
//						glVertex3fv( (trail[i].pos + shift - trail[i].norm * 5.0).v);
//					}
//				glEnd();
				
				ofPopMatrix();
			}
			
			glDisable(GL_DEPTH_TEST);			

		}

		vector <spineJoint> trail;
	
		ofPoint startPoint;
		ofPoint target;
		ofPoint homePoint;
		ofPoint prevPoint;
		ofPoint vel, gra;
		float rad, drag;
		float unique;
};
