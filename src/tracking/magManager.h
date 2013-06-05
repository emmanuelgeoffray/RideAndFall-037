/*
 *  magManager.h
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
#include "ofxOpenCv.h"
#include "ofxFern.h"
#include "ofThread.h"

#include "magazine.h"

typedef enum{
	MAG_NONE,
	MAG_FRONT,
	MAG_BACK
}magState;

#define MIN_AREA_TO_BE_MAG (15*15)

class magManager : public ofThread{

	public:
		
		void setup(string modelFile, string modelFileBack, int cameraWidth, int cameraHeight);
		
		void updatePixels(unsigned char * pixels);
		void updateTracking();

		void debugDraw(float x, float y, float w, float h);

		//TODO: this is a terrible way to do it. 
//		float getPctFront(int numFramesToFull = 50 ){
//			return ofClamp((float)framesFront/numFramesToFull, 0, 1.0);
//		}
//		float getPctBack(int numFramesToFull = 50 ){
//			return ofClamp((float)framesBack/numFramesToFull, 0, 1.0);
//		}
		
		magState getMagState(){
			return state;
		}
		
		bool isFront(){
			return (state == MAG_FRONT);
		}

		bool isBack(){
			return (state == MAG_BACK);
		}

		bool isNoMag(){
			return (state == MAG_NONE);
		}
		
		magazine getMagazine();
			
		protected:
		
			void updateGeometry();
		
			void processFrames();
			void threadedFunction();
			
			int camW, camH;
			magazine mag;
			
			float lastTimeSeen;
			float lastTimeSeenGoodMag;
			
			bool geomNeedsUpdating;

			ofxFern fern;
			ofxFern fernBack;
			
			magState state;
			string magStateStr;
			
			bool bFront;

			float timeSinceFront;
			float timeSinceBack;

			int framesFront;
			int framesBack;
			
			float trackingPct;
			
			ofxCvColorImage colorImg;
			ofxCvGrayscaleImage img;
			
			bool flagChanged;
			vector <ofPoint> corners;
		
};