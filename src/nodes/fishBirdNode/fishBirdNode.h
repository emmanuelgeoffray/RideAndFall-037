/*
 *  fishBirdNode.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 12/02/2010.
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
#include "magnetElement.h"
#include "fishBirdManager.h"
#include "textStackFromXml.h"
#include "stackText.h"

class fishBirdNode : public baseNode{

	public:
		fishBirdNode();
	
		virtual void setup(ofPoint position);
		
		//If you want to return this element (So that all drawing happens inside this class's draw() do this
		//otherwise return your elements vector so that each element is depth sorted and drawn seperately
		virtual vector <baseElement *> getElements();

		virtual void update();
		virtual void draw();	
		
		void checkState(float targetX, float targetY, float targetZ);
		bool shouldPersonWait();	
		
		imageElement bg;
		
		bool introFinished;
		
		float prePct;
		float pct;
		
		int textCount;
		
		ofImage textureImage;
		ofImage textureImage2;
				
		float attractPct;
		float timeReleased;
		
		string birdPathReg;
		string birdPathFull;

		string fishPathReg;
		string fishPathFull;		
		
		float scaleFactor;

		stackText nodeText;
		
		imageElement img;
		imageElement img2;
		
		fishBirdManager fbManager;		
		
		vector <magnetElement> magnets;
};