/*
 *  handManagerNode.h
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
#include "ofMain.h"
#include "baseNode.h"
#include "seqManagerElement.h"
#include "imageReferenceElement.h"

class scaleElement: public imageReferenceElement{
	public:
		scaleElement(){
			drag = 0.98;
			bReleased = false;
			vel = ofVec3f(0,0,0);
			customScale = 0.0;
		}
		
		void release(){
			bReleased = true;
			customScale = 0.0;
		}

		void release(float speed){
			if( bReleased ) return;
			
			customScale = 0.0;
			bReleased = true;
			ofVec3f d = target-pos;
			d.normalize();
			vel = d * speed;
		}
		
		void setUniqueVal(float uniqueVal){
			unique = uniqueVal;
		}
		
		void setDrag(float dragAmnt){
			drag = dragAmnt;
		}
		
		void setTargetPosition(float x, float y, float z){
			target.set(x, y, z);
		}

		void setScale(float x, float y, float z){
			 scale.set(x, y, z);
			 ogScale = scale;
		}
		
		void update(float attractForce, float scaleRate, float scaleAmnt){
			float amntScale = customScale * scaleAmnt * ofClamp( ofSignedNoise(scaleRate * ofGetElapsedTimef(), unique), 0, 1);
			scale.set( ogScale.x * amntScale, ogScale.y * amntScale, 1.0);
			
			if( bReleased ){
				vel *= drag;
				ofVec3f d = target-pos;
				d.normalize();
			
				vel += d  * attractForce;
				pos += vel;
				visible = true;
			}else{
				visible = false;
			}
			
			if( customScale < 1.0 ){
				customScale += 0.05;
			}else{
				customScale = 1.0;
			}
		}
		
		float customScale;
		bool bReleased;
		float drag;
		float unique;
		ofPoint ogScale;
		ofPoint target;
		ofPoint vel;
};

class scaleFieldNode : public baseNode{

	public:
		scaleFieldNode();
	
		virtual void setup(ofPoint position);
		
		void updateCamToScreenCoords();

		virtual vector <baseElement *> getElements();
		
		virtual void update();
		virtual void draw();	
		
		seqManagerElement seq;
		
		ofPoint storePersonPos;
		ofPoint handStorePoint;
		ofPoint myPersonScreenPos;		
		bool needToCalcStringPos;
	
		ofImage plusImage;
		ofImage minusImage;
		vector <scaleElement> particles;

};
