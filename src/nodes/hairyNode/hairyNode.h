/*
 *  hairyNode.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 11/02/2010.
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
#include "imageElement.h"
#include "imageParticleElement.h"
#include "ofxChipmunk.h"
#include "hairyBeast.h"
#include "stackText.h"
#include "textStackFromXml.h"
#include "hairyStem.h"

class hairyNode : public baseNode{

	public:
		hairyNode();
	
		virtual void setup(ofPoint position);
		
		//If you want to return this element (So that all drawing happens inside this class's draw() do this
		//otherwise return your elements vector so that each element is depth sorted and drawn seperately
		virtual vector <baseElement *> getElements();
		
		~hairyNode();
		
		//----------------
		virtual void checkState(float targetX, float targetY, float targetZ);
		virtual bool shouldPersonWait();
				
		virtual void update();
		void drawEye(float size, float vx, float vy);
		virtual void draw();	
		
		int textCount;
						
		float pct;
		float radius;
		float innerSize;
		float lastTime;
	
		stackText nodeText;
		
		vector <imageElement> textBlocks;
		vector <float> textBlocksPct;
		bool prevState;
		int blockCount;
		int minBlockCount;
		
		vector <hairyBeast> hairys;

		ofxChipmunk world;
		ofxChipmunkCircle base;
		vector <hairyStem> particles;
};