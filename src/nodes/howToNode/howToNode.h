/*
 *  howToNode.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 21/02/2010.
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
#include "textBlockElement.h"
#include "textBlockTrackingElement.h"
#include "imageRectParticleElement.h"
#include "imageReferenceElement.h"

struct appInfo{

	appInfo(){
		appFps				= 30.0;
		numNodesActive		= 0;
		numNodesToDiscover	= 0;
		numObjectsDrawn		= 0;
		numObjectsNotDrawn	= 0;
	}
	
	 float		appFps;
	 ofPoint	worldGravity;
     ofPoint	personVel;
     ofPoint	trackingPos;
     ofPoint	magRot;
     int		numNodesActive;
     int		numNodesToDiscover;
     int		numObjectsDrawn;
	 int		numObjectsNotDrawn;
};

class howToNode : public baseNode{

	public:
		virtual void setup(ofPoint position);
		
		//If you want to return this element (So that all drawing happens inside this class's draw() do this
		//otherwise return your elements vector so that each element is depth sorted and drawn seperately
		virtual vector <baseElement *> getElements();
		
		void setPosition(float x, float y, float z);
		virtual void setState(nodeState stateIn);		
		virtual void update();
		virtual void draw();
		
		void playMovie(){
			if( !bPlaying ){
				vid1.setPosition(0.0);			
				vid1.play();
				bPlaying = true;
			}
		}
		void resetMovie(){
			if( bPlaying ){
				vid1.setPosition(0.0);
				vid1.stop();
				bPlaying = false;
			}
		}
		
		void updateAppInfo(appInfo infoIn);

		//----------------
		void checkState(float targetX, float targetY, float targetZ);
		bool shouldPersonWait();
		
		ofVideoPlayer vid1;
		bool bPlaying;
		
		float timeActive;
		float pct;
		
		appInfo info;
		
		ofPoint moviePos;
		ofImage howToBgImage;
		ofImage playImg;
		
		imageReferenceElement howToBg;
		imageRectParticleElement movieFrame;
};