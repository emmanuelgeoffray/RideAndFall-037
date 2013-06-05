/*
 *  magManager.cpp
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

#include "magManager.h"

//#define THREAD_ME

//-------------------------------------------------
void magManager::setup(string modelFileFront, string modelFileBack, int cameraWidth, int cameraHeight){

	camW = cameraWidth;
	camH = cameraHeight;

	printf("Notice - Opening Fern Tracker\n");
	fern.initTracker(modelFileFront, camW, camH);
	fern.setMode('5');

	fernBack.initTracker(modelFileBack, camW, camH);
	fernBack.setMode('5');

	mag.lastFrameSeen = ofGetFrameNum();

	printf("Notice - Tracker opened\n");
	
	colorImg.allocate(camW, camH);
	img.allocate(camW, camH);
	
	mag.setup(camW, camH);
	
	state = MAG_NONE;
	magStateStr = "MAG_NONE";			
	
	framesFront = 0;
	framesBack = 0;
	bFront = true;
	
	#ifdef THREAD_ME 
		startThread(false, false);
	#endif
	
	flagChanged = false;
}

//-------------------------------------------------
void magManager::threadedFunction(){
	while(true){
			
		ofSleepMillis(40);

		if( !flagChanged ) continue;
		
		if( lock() ){
			processFrames();
			unlock();
		}
	}
}

void magManager::processFrames(){

	colorImg.mirror(false, true);
	img = colorImg;
	img.convertToRange(30, 240);
	
	bool found = false;
	if( bFront ){
		fern.update(img);
					
		if( fern.last_frame_ok ){
			corners = fern.getTrackedCorners();		
			geomNeedsUpdating = true;	
			framesFront++;		
			framesBack = 0;		
			found = true;
			lastTimeSeen = ofGetElapsedTimef();			
		}
	}else{
		fernBack.update(img);
					
		if( fernBack.last_frame_ok ){
			corners = fernBack.getTrackedCorners();		
			geomNeedsUpdating = true;	
			framesBack++;
			framesFront=0;
			found = true;
			lastTimeSeen = ofGetElapsedTimef();
		}
	}		
	
	if( state == MAG_NONE && found == false ){
		bFront = !bFront;
	}

	flagChanged = false;
}

//-------------------------------------------------
void magManager::updatePixels( unsigned char * pixels ){
	
	//TODO: move this to time
	//TODO: SERIOUSLY!!!!!!
//	int numFramesNotSeen = ofGetFrameNum() - mag.lastFrameSeen;
//	
//	if( ( bFront && numFramesNotSeen > 30) || ( !bFront && numFramesNotSeen > 20 )  ){
//		bFront = !bFront;
//	}
//	

	#ifdef THREAD_ME 
		if( lock() ){
	#endif
		colorImg = pixels;
		flagChanged = true;
		
	#ifdef THREAD_ME
			unlock();
		}else{
			printf("unable to lock!\n");
		}
	#else
		processFrames();
	#endif
	
}

//----------------------------------------------
void magManager::updateTracking(){
	updateGeometry();
	
	float timeSinceMagSeen = ofGetElapsedTimef()-lastTimeSeenGoodMag;
	
	if( timeSinceMagSeen <= 2.0 ){
	
		if( state == MAG_BACK && framesFront > 2 ){
			state = MAG_FRONT;
			magStateStr = "MAG_FRONT";
		}
		else if( state == MAG_FRONT && framesBack > 3 ){
			state = MAG_BACK;
			magStateStr = "MAG_BACK";			
		}else if( framesFront >= 1 ){
			state = MAG_FRONT;
			magStateStr = "MAG_FRONT";			
		}else if( framesBack >= 1 ){
			state = MAG_BACK;
			magStateStr = "MAG_BACK";			
		}
	
	}else{
		if( timeSinceMagSeen > 3.0 ){
			state = MAG_NONE;
			magStateStr = "MAG_NONE";						
		}	
	}
	
}

//----------------------------------------------
void magManager::updateGeometry(){
	if( !geomNeedsUpdating )return;
	
	vector <ofPoint> tmpCorners;

	#ifdef THREAD_ME 
		if( lock() ){
	#endif
	
		tmpCorners = corners;
		geomNeedsUpdating = false;
		
	#ifdef THREAD_ME 
			unlock();
		}
	#endif 
	
	mag.updateFromCorners(tmpCorners);
	
	if( mag.area > MIN_AREA_TO_BE_MAG ){
		lastTimeSeenGoodMag = lastTimeSeen;
	}
}

//----------------------------------------------
magazine magManager::getMagazine(){
	return mag;
}

//-------------------------------------------------
void magManager::debugDraw(float x, float y, float w, float h){
	if( img.width <= 0 )return;
	
	ofPushStyle();
	
		if(state != MAG_NONE ){
			if( state == MAG_FRONT ){
				fern.drawOverlay(x, y, w, h);
			}else{
				fernBack.drawOverlay(x, y, w, h);
			}
		}
		
		ofSetColor(255, 255, 255, 255);
		img.draw(x, y, w, h);
		
		mag.debugDraw(x, y, w, h);		
		ofDrawBitmapString("bFront = "+ofToString((int)bFront), x + 10, y + h-27);
		ofDrawBitmapString("state  = "+magStateStr, x + 10, y + h-10);
		
	ofPopStyle();
	
//	ofDrawBitmapString(ofToString(area, 2), 10, 30);
//	ofDrawBitmapString(ofToString(z, 2), 10, 48);	
//	ofDrawBitmapString(ofToString(rotAng, 2), 10, 68);

}

