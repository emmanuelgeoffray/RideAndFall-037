/*
 *  camApp.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 26/12/2009.
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
#include "ofx3DWorld.h"
#include "baseTrailParticle.h"
#include "actionElement.h"
#include "rotateElement.h"
#include "imageElement.h"
#include "simpleSceneGraph.h"
#include "colorLoader.h"
#include "typeLoader.h"
#include "trailGroupManager.h"
#include "myPersonElement.h"
#include "_projectNodes.h"
#include "camManager.h"
#include "imageReferenceElement.h"
#include "handTextManager.h"
#include "backgroundSoundManager.h"
#include "ofxChipmunk.h"
#include "textArmElement.h"

#include "windElement.h"
#include "magazine.h"

typedef enum{
	CAM_APP_NORMAL, 
	CAM_APP_TITLE,
	CAM_APP_HOWTO
}camAppState;

class camApp : public ofBaseApp{

	public:
		camApp();
		~camApp();
		
		void loadResources();
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased (int key);

		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		
		bool makeNewNode(float distFromNodeToMakeNew );
		
		void setHowTo();
		void setNormal();
		
		void titleUpdate();
		void riseFallUpdate();		
		void doPinPrick();
		void handleRiseFall();
		void updateAppInfo();
	
		void updateControls(magazine mag);
		
	
	protected:
		
		bool resourcesLoaded;
		void nextNode(bool makeAbove);
		void limitPersonMovement(ofPoint relPos, float minX, float maxX, float minY, float maxY);
		
		magazine mag;
		
		float nodeCreatedTime;
				
	public:
		float fov;
		ofx3DWorld world;
		GLint hits;
		ofPoint mouse;
		string hitsString;
		simpleSceneGraph sceneGraph;
		camManager camMan;
						
		float camShiftAmntY;
		
		ofImage background;
		ofImage background2;

		ofImage backgroundEnd;
		ofImage backgroundEnd2;

		ofImage handImg;
		ofImage exitImage;
		
		float magRotZStartHowTo;
		
		float percentOver;
		float fallPct;
		
		int selection;
		bool bZoomed;
		int count;
		
		weatherNode weather;
		handManagerNode hand;
		titleNode title;
		howToNode howTo;
		
		camAppState state;
		bool bRestoreToIntro;
		bool bStartedWithKeys;
		
		float nextTime;
			
		vector <string> nodeOrder;
		vector <ofImage> rainImage;
		
		float dragAmnt;
		ofPoint myGravity;
		ofPoint howToStorePos;
		float updateSpeed;
		
		
		baseNode * currentNode;
		vector <baseNode *> nodes;
				
		
};


