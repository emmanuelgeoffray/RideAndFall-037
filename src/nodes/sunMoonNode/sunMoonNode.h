/*
 *  sunMoonNode.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 17/02/2010.
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
#include "imageCrossFadeElement.h"
#include "stackText.h"
#include "textStackFromXml.h"

class sunMoonNode : public baseNode{

	public:
		sunMoonNode();
		
		virtual void setup(ofPoint position);
		
		//If you want to return this element (So that all drawing happens inside this class's draw() do this
		//otherwise return your elements vector so that each element is depth sorted and drawn seperately
		virtual vector <baseElement *> getElements();

		virtual void update();
		virtual void draw();	
		
		void checkState(float targetX, float targetY, float targetZ);
		bool shouldPersonWait();	
				
		float pct;
		float scaleFactor;

		stackText nodeText;
		imageCrossFadeElement sunMoon;

};